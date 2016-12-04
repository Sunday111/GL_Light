#pragma once

#include <cassert>
#include <cstdint>
#include <iterator>
#include <type_traits>

namespace XArrayView {

template<typename T, typename U>
struct Rebind
{
    typedef U Type;
};

template<typename T, typename U>
struct Rebind<const T, U>
{
    typedef const U Type;
};

}

template<typename T>
class ArrayView
{
public:
    ArrayView() :
        m_p(nullptr), m_size(0), m_stride(sizeof(T)) {}

    explicit ArrayView(T* p, int size, int stride = sizeof(T)) :
        m_p(p), m_size(size), m_stride(stride)
    {
        assert(size == 0 || (p != nullptr && size > 0));
        assert(stride >= sizeof(T));
    }

    template<typename U,
        typename = typename std::enable_if<std::is_convertible<U*, T*>::value, void>::type>
    ArrayView(const ArrayView<U>& other) :
        m_p(other.m_p), m_size(other.m_size), m_stride(other.m_stride)
    {
    }

    ArrayView(const ArrayView&) = default;

    int size() const { return m_size; }
    size_t usize() const { return static_cast<size_t>(size()); }

    int stride() const { return m_stride; }

    bool empty() const { return m_size == 0; }

    T& operator[](int i)
    {
        assert(m_p != nullptr && i < m_size);

        return *reinterpret_cast<T*>(reinterpret_cast<BytePtr>(m_p) +
                                     i * m_stride);
    }

    T& operator[](int i) const
    {
        assert(m_p != nullptr && i < m_size);

        return *reinterpret_cast<T*>(reinterpret_cast<BytePtr>(m_p) +
                                     i * m_stride);
    }


    ArrayView& operator=(const ArrayView&) = default;

private:
    template<typename U>
    friend class ArrayView;

    typedef typename XArrayView::Rebind<T, uint8_t>::Type* BytePtr;

    T* m_p;
    int m_size;
    int m_stride;
};

namespace XArrayView {

template<typename T>
ArrayView<typename std::remove_reference<typename T::reference>::type>
MakeArrayViewHelper(T first, T last, std::random_access_iterator_tag)
{
    typedef typename std::remove_reference<typename T::reference>::type U;

    if(first < last)
    {
        return ArrayView<U>(&(*first), static_cast<int>(last - first));
    }

    return ArrayView<U>();
}

}

// Makes array view to an element
template<typename T>
ArrayView<T> MakeArrayView(T* p)
{
    return ArrayView<T>(p, 1);
}

// Makes array view to set of elements
template<typename T>
ArrayView<T> MakeArrayView(T* p, int size)
{
    return ArrayView<T>(p, size);
}

// Makes array view to set of elements with optional stride
template<typename T>
ArrayView<T> MakeArrayView(T* p, int size, int stride)
{
    return ArrayView<T>(p, size, stride);
}

template<typename T>
ArrayView<typename std::remove_reference<typename T::reference>::type>
MakeArrayView(T first, T last)
{
    typename T::iterator_category iter_cat;

    return XArrayView::MakeArrayViewHelper(first, last, iter_cat);
}

template<typename T>
ArrayView<typename T::value_type> MakeArrayView(T& c)
{
    return MakeArrayView(std::begin(c), std::end(c));
}

template<typename T>
ArrayView<typename const T::value_type> MakeArrayView(const T& c)
{
    return MakeArrayView(std::begin(c), std::end(c));
}
