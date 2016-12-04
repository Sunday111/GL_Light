#pragma once

#include <memory>
#include <vector>

class IndexBlob
{
public:
    explicit IndexBlob(std::vector<int> indices) :
        m_indices(indices)
    {}

    const std::vector<int>& GetData() { return m_indices; }

private:
    const std::vector<int> m_indices;
};

using IndexBlobPtr = std::shared_ptr<IndexBlob>;
using IndexBlobConstPtr = std::shared_ptr<const IndexBlob>;