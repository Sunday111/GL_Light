// GLEW
#ifndef GLEW_STATIC
#define GLEW_STATIC
#endif
#include <GLEW/glew.h>

#include <cassert>
#include <SOIL/SOIL.h>

#include "Render/Texture.h"

namespace
{
    static int TextureDataTypeToSOILEnum(TextureDataType dataType)
    {
        switch (dataType)
        {
        case TextureDataType::Auto:     return SOIL_LOAD_AUTO;
        case TextureDataType::L:        return SOIL_LOAD_L;
        case TextureDataType::LA:       return SOIL_LOAD_LA;
        case TextureDataType::RGB:      return SOIL_LOAD_RGB;
        case TextureDataType::RGBA:     return SOIL_LOAD_RGBA;
        default:
            assert(!"Unrecognized type");
            return -1;
        }
    }

    static GLenum TextureDataTypeToGLenum(TextureDataType dataType)
    {
        switch (dataType)
        {
        case TextureDataType::RGB:      return GL_RGB;
        case TextureDataType::RGBA:     return GL_RGBA;
        default:
            assert(!"Unrecognized type");
            return 0;
        }
    }

    static GLenum ChannelsCountToGLenum(int channelsCnt)
    {
        switch (channelsCnt)
        {
        case 3: return GL_RGB;
        case 4: return GL_RGBA;
        default:
            assert(!"Don't know wtf");
            return 0;
        }
    }

    static GLenum TextureFilteringMethodToGLenum(TextureFilteringMethod filteringMethod)
    {
        switch (filteringMethod)
        {
        case TextureFilteringMethod::Nearest: return GL_NEAREST;
        case TextureFilteringMethod::Linear:  return GL_LINEAR;
        default:
            assert(!"Don't know wtf");
            return 0;
        }
    }

    static GLenum TextureFilteringTypeToGLenum(TextureFilteringType filteringType)
    {
        switch (filteringType)
        {
        case TextureFilteringType::Magnifying: return GL_TEXTURE_MAG_FILTER;
        case TextureFilteringType::Minifying:  return GL_TEXTURE_MIN_FILTER;
        default:
            assert(!"Don't know wtf");
            return 0;
        }
    }

    static GLenum TextureWrappingTypeToGLenum(TextureWrappingType wrappingType)
    {
        switch (wrappingType)
        {
        case TextureWrappingType::Repeat:         return GL_REPEAT;
        case TextureWrappingType::MirroredRepeat: return GL_MIRRORED_REPEAT;
        case TextureWrappingType::ClampToEdge:    return GL_CLAMP_TO_EDGE;
        case TextureWrappingType::ClampToBorder:  return GL_CLAMP_TO_BORDER;
        default:
            assert(!"Don't know wtf");
            return 0;
        }
    }

    static GLenum TextureAxisToGLenum(_TextureAxis::Base axis)
    {
        switch (axis)
        {
        case _TextureAxis::S: return GL_TEXTURE_WRAP_S;
        case _TextureAxis::T: return GL_TEXTURE_WRAP_T;
        case _TextureAxis::R: return GL_TEXTURE_WRAP_R;
        default:
            assert(!"Don't know wtf");
            return 0;
        }
    }
}

Texture2D::Texture2D(const char* fileName, int slot,
    TextureDataType dataType, std::ostream* logstream) :
m_channelsCnt(-1),
m_slot(slot),
m_dataType(dataType)
{
    m_data = SOIL_load_image(fileName, &m_width, &m_height,
        &m_channelsCnt, TextureDataTypeToSOILEnum(dataType));

    if (m_data == nullptr)
    {
        if (logstream != nullptr)
        {
            *logstream << "File " << fileName << " not found" << std::endl;
        }

        return;
    }

    glGenTextures(1, &m_id);

    const GLenum format = dataType == TextureDataType::Auto ?
        ChannelsCountToGLenum(m_channelsCnt) :
        TextureDataTypeToGLenum(dataType);

    Use();
    glTexImage2D(GL_TEXTURE_2D, 0, format, m_width, m_height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, m_data);
    glGenerateMipmap(GL_TEXTURE_2D);
    UnuseAny();
}

bool Texture2D::SetFiltering(TextureFilteringType type, TextureFilteringMethod method)
{
    const GLenum glMethod = TextureFilteringMethodToGLenum(method);

    if (glMethod == 0)
    {
        return false;
    }

    const GLenum glType = TextureFilteringTypeToGLenum(type);

    if (glType == 0)
    {
        return false;
    }

    Use();
    glTexParameteri(GL_TEXTURE_2D, glType, glMethod);
    UnuseAny();

    return true;
}

bool Texture2D::SetWrapping(Texture2DAxis axis, TextureWrappingType type)
{
    const GLenum glAxis = TextureAxisToGLenum(
        static_cast<_TextureAxis::Base>(axis));

    if (glAxis == 0)
    {
        return false;
    }

    const GLenum glType = TextureWrappingTypeToGLenum(type);

    if (glType == 0)
    {
        return false;
    }

    Use();
    glTexParameteri(GL_TEXTURE_2D, glAxis, glType);
    UnuseAny();

    return true;
}

void Texture2D::Use()
{
    assert(m_slot >= 0 && m_slot < 32 && "Invalid texture slot");
    glActiveTexture(GL_TEXTURE0 + m_slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::Use(int slot)
{
    const std::string msg("set texture " + std::to_string(m_id) +
        " to slot " + std::to_string(slot));

    //GLint currentTexture;
    //glGetIntegerv(GL_TEXTURE_BINDING_2D, &currentTexture);
    //if (currentTexture != static_cast<GLint>(m_id))
    //{
    //    glDebugMessageInsert(
    //        GL_DEBUG_SOURCE_APPLICATION,
    //        GL_DEBUG_TYPE_OTHER,
    //        GL_NO_ERROR,
    //        GL_DEBUG_SEVERITY_NOTIFICATION,
    //        -1, msg.c_str());
    //}
    assert(slot >= 0 && slot < 32 && "Invalid texture slot");
    glActiveTexture(GL_TEXTURE0 + slot);
    glBindTexture(GL_TEXTURE_2D, m_id);
}

void Texture2D::SetClampBorderColor(const Vector4f& color)
{
    Use();
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, color.data());
    UnuseAny();
}

void Texture2D::UnuseAny()
{
    glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::~Texture2D()
{
    if (m_data != nullptr)
    {
        SOIL_free_image_data(m_data);
    }
}