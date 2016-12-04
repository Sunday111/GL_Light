#pragma once
#include <cassert>
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
#include <type_traits>

#include "Scene/Model3d.h"

namespace obj
{
    struct VertexIndices
    {
        VertexIndices() :
            positionIndex(-1),
            textureCoordIndex(-1),
            normalIndex(-1)
        {}

        int positionIndex;
        int textureCoordIndex;
        int normalIndex;
    };

    struct Facet
    {
        std::vector<VertexIndices> m_vertices;
    };

    template<int size, class T>
    class Vec
    {
    public:
        template<int index>
        inline const T& At() const
        {
            static_assert(index > -1 && index < size, "Index out of range");
            return m_data[index];
        }

        template<int index>
        inline T& At()
        {
            return const_cast<T&>(static_cast<const Vec<size, T>*>(this)->At<index>());
        }

    private:
        T m_data[size];
    };

    enum class ParseErrorCode
    {
        CannotOpenFile,
        UnexpectedFormat,
        Ok
    };

    template<class T>
    class ObjParser;

    template<class T>
    class ObjModel
    {
    public:
        std::vector<Vec<4, T>> m_vertices;
        std::vector<Vec<3, T>> m_normals;
        std::vector<Vec<3, T>> m_textureCoords;
        std::vector<Facet> m_facets;
    };

    template<class T>
    class ObjParser
    {
        static_assert(std::is_floating_point<T>::value, "For floation point types only");

    public:
        static ParseErrorCode Parse(std::istream& stream,
            ObjModel<T>* model, std::ostream* logstream = nullptr)
        {
            std::string line;
            std::string word;

            while (!stream.eof())
            {
                getline(stream, line);

                std::stringstream lineStream(line);
                word.clear();
                lineStream >> word;

                const size_t wordSize = word.size();

                if (wordSize == 0 || word == "#")
                {
                    continue;
                }

                decltype(&ObjParser<T>::Parse_F) func = nullptr;

                ParseErrorCode result = GetParseFunction(word, &func, logstream);

                if (result != ParseErrorCode::Ok)
                {
                    return result;
                }

                assert(func != nullptr);

                result = (func)(word, lineStream, model, logstream);


                if (result != ParseErrorCode::Ok)
                {
                    return result;
                }
            }

            return ParseErrorCode::Ok;
        }
        static ParseErrorCode Parse(const std::string& fileName,
            ObjModel<T>* model, std::ostream* logstream = nullptr)
        {

            std::ifstream file(fileName);

            if (!file.good())
            {
                if (logstream != nullptr)
                {
                    *logstream << "Can't open file";
                }

                return ParseErrorCode::CannotOpenFile;
            }

            const int fileBufferSize = 32768;
            char fileBuffer[fileBufferSize];

            file.rdbuf()->pubsetbuf(fileBuffer, fileBufferSize);

            return Parse(file, model, logstream);
        }

    private:
        //Texture coords
        static ParseErrorCode Parse_VT(const std::string& word,
            std::stringstream& line, ObjModel<T>* model, std::ostream* logstream)
        {
            Vec<3, T> coords;

            line >> coords.At<0>();
            line >> coords.At<1>();

            if (line.fail())
            {
                if (logstream != nullptr)
                {
                    *logstream << "Failed to read vertex position from line "
                        << line.str();
                }

                return ParseErrorCode::UnexpectedFormat;
            }

            line >> coords.At<2>();

            if (line.fail())
            {
                coords.At<2>() = static_cast<T>(0);
            }

            if (model != nullptr)
            {
                model->m_textureCoords.push_back(coords);
            }

            assert(word.size() == 2);
            return ParseErrorCode::Ok;
        }

        //Normal
        static ParseErrorCode Parse_VN(const std::string& word,
            std::stringstream& line, ObjModel<T>* model, std::ostream* logstream)
        {
            Vec<3, T> normal;

            line >> normal.At<0>();
            line >> normal.At<1>();
            line >> normal.At<2>();

            if (line.fail())
            {
                if (logstream != nullptr)
                {
                    *logstream << "Failed to read normal from line "
                        << line.str();
                }

                return ParseErrorCode::UnexpectedFormat;
            }

            if (model != nullptr)
            {
                model->m_normals.push_back(normal);
            }

            assert(word.size() == 2);
            return ParseErrorCode::Ok;
        }

        //Vertex
        static ParseErrorCode Parse_V(const std::string& word,
            std::stringstream& line, ObjModel<T>* model, std::ostream* logstream)
        {
            Vec<4, T> pos;

            line >> pos.At<0>();
            line >> pos.At<1>();
            line >> pos.At<2>();

            if (line.fail())
            {
                if (logstream != nullptr)
                {
                    *logstream << "Failed to read vertex position from line "
                        << line.str();
                }

                return ParseErrorCode::UnexpectedFormat;
            }

            line >> pos.At<3>();

            if (line.fail())
            {
                pos.At<3>() = static_cast<T>(1);
            }

            if (model != nullptr)
            {
                model->m_vertices.push_back(pos);
            }

            assert(word.size() == 1);
            return ParseErrorCode::Ok;
        }

        //Facet
        static ParseErrorCode Parse_F(const std::string& word,
            std::stringstream& line, ObjModel<T>* model, std::ostream* logstream)
        {
            char c;

            Facet f;
            f.m_vertices.reserve(3);

        #pragma warning(push)
        #pragma warning(disable:4127)
            while (true)
        #pragma warning(pop)
            {
                VertexIndices vi;
                line >> vi.positionIndex;
                --vi.positionIndex;

                if (line.fail())
                {
                    break;
                }

                if (line.peek() == '/')
                {
                    line >> c;

                    if (line.peek() == '/')
                    {
                        //skip texture coords
                        line >> c;
                        line >> vi.normalIndex;
                        --vi.normalIndex;
                    }
                    else
                    {
                        line >> vi.textureCoordIndex;
                        --vi.textureCoordIndex;

                        if (line.peek() == '/')
                        {
                            line >> c;
                            line >> vi.normalIndex;
                            --vi.normalIndex;
                        }
                    }
                }

                f.m_vertices.push_back(vi);

                while (line.peek() == '/')
                {
                    line >> c;
                }
            }

            const size_t verticesCount = f.m_vertices.size();
            if (verticesCount < 3)
            {
                if (logstream != nullptr)
                {
                    *logstream << "Invalid facet found (" <<
                        verticesCount << " vertices only)";
                }

                return ParseErrorCode::UnexpectedFormat;
            }

            if (model)
            {
                model->m_facets.push_back(f);
            }

            assert(word.size() == 1);
            return ParseErrorCode::Ok;
        }

        //Object name
        static ParseErrorCode Parse_O(const std::string& word,
            std::stringstream&, ObjModel<T>*, std::ostream*)
        {
            assert(word.size() == 1);
            return ParseErrorCode::Ok;
        }

        //Group name
        static ParseErrorCode Parse_G(const std::string& word,
            std::stringstream&, ObjModel<T>*, std::ostream*)
        {
            assert(word.size() == 1);
            return ParseErrorCode::Ok;
        }

        //Smooth shading
        static ParseErrorCode Parse_S(const std::string& word,
            std::stringstream&, ObjModel<T>*, std::ostream*)
        {
            assert(word.size() == 1);
            return ParseErrorCode::Ok;
        }

        //Parse usemtl
        static ParseErrorCode Parse_U(const std::string& word,
            std::stringstream&, ObjModel<T>*, std::ostream*)
        {
            assert(word == "usemtl");
            return ParseErrorCode::Ok;
        }

        //Parse mtllib
        static ParseErrorCode Parse_M(const std::string& word,
            std::stringstream&, ObjModel<T>*, std::ostream*)
        {
            assert(word == "mtllib");
            return ParseErrorCode::Ok;
        }

        static ParseErrorCode GetParseFunction(std::string& word,
            decltype(&ObjParser<T>::Parse_F)* func, std::ostream* logstream)
        {
            assert(func != nullptr);

            ParseErrorCode result = ParseErrorCode::Ok;

            switch (tolower(word.front()))
            {
            case 'v':
                if (word.size() == 1)
                {
                    *func = ObjParser<T>::Parse_V;
                }
                else
                {
                    switch (word[1])
                    {
                    case 't':
                        *func = ObjParser<T>::Parse_VT;
                        break;
                    case 'n':
                        *func = ObjParser<T>::Parse_VN;
                        break;
                    default:
                        result = ParseErrorCode::UnexpectedFormat;
                        break;
                    }
                }
                break;

            case 'f':
                *func = ObjParser<T>::Parse_F;
                break;

            case 'o':
                *func = ObjParser<T>::Parse_O;
                break;

            case 'g':
                *func = ObjParser<T>::Parse_G;
                break;

            case 's':
                *func = ObjParser<T>::Parse_S;
                break;

            case 'u':
                *func = ObjParser<T>::Parse_U;
                break;

            case 'm':
                *func = ObjParser<T>::Parse_M;
                break;
            default:
                result = ParseErrorCode::UnexpectedFormat;
                break;
            }

            if (result != ParseErrorCode::Ok && (logstream != nullptr))
            {
                *logstream << "Unexpected word " << ' ' << word;
            }

            return result;
        }
    };

    template<class T>
    Model3dPtr Convert(const ObjModel<T>& model, const IMaterialPtr& mat)
    {
        VertexBlobField fields = VertexBlobField::Pos;

        if (model.m_normals.size() > 0)
        {
            fields |= VertexBlobField::Norm;
        }

        if (model.m_textureCoords.size() > 0)
        {
            fields |= VertexBlobField::TexCoords;
        }

        VertexBlobPtr vertexBlob = std::make_shared<VertexBlob>(fields,
            static_cast<int>(model.m_vertices.size()));
        auto vertexView = vertexBlob->GetFieldView<VertexBlobField::Pos>();

        for (size_t i = 0; i < model.m_vertices.size(); ++i)
        {
            vertexView[i].x() = model.m_vertices[i].At<0>();
            vertexView[i].y() = model.m_vertices[i].At<1>();
            vertexView[i].z() = model.m_vertices[i].At<2>();
        }

        std::vector<int> indices;
        indices.reserve(model.m_facets.size() * 3);

        auto normView = vertexBlob->GetFieldView<VertexBlobField::Norm>();
        auto texCoordsView = vertexBlob->GetFieldView<VertexBlobField::TexCoords>();

        for (const Facet& facet : model.m_facets)
        {
            for (size_t vertexIndex = 0; vertexIndex < facet.m_vertices.size(); ++vertexIndex)
            {
                const VertexIndices& vi = facet.m_vertices[vertexIndex];

                if (vertexIndex < 3)
                {
                    indices.push_back(vi.positionIndex);
                }

                if (vi.normalIndex >= 0)
                {
                    auto norm = model.m_normals[vi.normalIndex];
                    normView[vi.normalIndex].x() = norm.At<0>();
                    normView[vi.normalIndex].y() = norm.At<1>();
                    normView[vi.normalIndex].z() = norm.At<2>();
                }

                if (vi.textureCoordIndex >= 0)
                {
                    const auto& texCoord = model.m_textureCoords[vi.textureCoordIndex];
                    texCoordsView[vi.textureCoordIndex].x() = texCoord.At<0>();
                    texCoordsView[vi.textureCoordIndex].y() = texCoord.At<1>();
                }
            }
        }

        MeshDataPtr meshData = std::make_shared<MeshData>(
            vertexBlob, std::make_shared<IndexBlob>(indices));

        return std::make_shared<Model3d>(meshData, mat);
    }

}