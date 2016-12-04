#pragma once

#include <memory>
#include <ostream>
#include <type_traits>

#include "Base/Geom/Vector.h"

// GLEW
#define GLEW_STATIC
#include <GLEW/glew.h>

enum class TextureDataType
{
    //leaves the image in whatever format it was found
    Auto,

    //Luminous (greyscale)
    L,

    //Luminous (greyscale) with Alpha
    LA,

    //Red, Green, Blue
    RGB,

    //Red, Green, Blue, Alpha
    RGBA
};

enum class TextureFilteringType
{
    Magnifying,
    Minifying
};

enum class TextureFilteringMethod
{
    //selects the pixel which center is closest to the texture coordinate
    Nearest,

    /* takes an interpolated value from the texture coordinate's
     * neighboring texels, approximating a color between the texels
     */
    Linear
};

enum class TextureWrappingType
{
    Repeat,
    MirroredRepeat,
    ClampToEdge,
    ClampToBorder
};

namespace _TextureAxis
{
    using Base = uint8_t;

    enum TextureAxis : Base
    {
        S, //x
        T, //y
        R  //z
    };
}

enum class Texture2DAxis : _TextureAxis::Base
{
    S = _TextureAxis::S,
    T = _TextureAxis::T
};

class Texture2D
{
public:
    /* magnifying filtering for magnifying operations (when scaling)
       minifying filtering for minifying operations (for downwards)
     */
    explicit Texture2D(const char* fileName, int slot,
        TextureDataType dataType = TextureDataType::Auto,
        std::ostream* logstream = nullptr);
    Texture2D(const Texture2D&) = delete;
    ~Texture2D();

    bool SetFiltering(TextureFilteringType type, TextureFilteringMethod method);
    bool SetWrapping(Texture2DAxis axis, TextureWrappingType type);

    void Use();
    void Use(int slot);

    void SetClampBorderColor(const Vector4f& color);
    static void UnuseAny();

private:
    int m_channelsCnt;
    int m_width;
    int m_height;
    int m_slot;
    unsigned char* m_data;
    TextureDataType m_dataType;
    GLuint m_id;
};

using Texture2dPtr = std::shared_ptr<Texture2D>;