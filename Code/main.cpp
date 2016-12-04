//STL
#include <iostream>
#include <iomanip>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GLEW/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_decompose.hpp>

// SOIL
#include <SOIL/SOIL.h>

#include "Base/Stopwatch.h"
#include "Base/Geom/VertexBlob.h"
#include "Render/VertexBufferObject.h"
#include "Render/ElementBufferObject.h"
#include "Render/Shaders/ShaderProgram.h"
#include "Render/Texture.h"
#include "Render/Camera.h"
#include "Scene/Materials/TexturedMaterial.h"
#include "Scene/Materials/ColoredMaterial.h"
#include "Scene/Model3d.h"
#include "Scene/Lights/LightsArray.h"

namespace
{
    // Window dimensions
    const GLuint WIDTH = 800, HEIGHT = 600;
    static Camera g_camera(
        static_cast<float>(WIDTH) / static_cast<float>(HEIGHT),
        0.1f, 100.f);
    static bool g_keys[1024]{ 0 };

    static Model3dPtr CreateTestCube(const IMaterialPtr& mat)
    {
        VertexBlobField fields = VertexBlobField::Empty;
        fields |= VertexBlobField::Pos;
        fields |= VertexBlobField::Norm;
        fields |= VertexBlobField::TexCoords;
        VertexBlobPtr vertexBlob = std::make_shared<VertexBlob>(fields, 36);

        if ((fields & VertexBlobField::Pos) != VertexBlobField::Empty)
        {
            ArrayView<Vector3f> posView = vertexBlob->GetFieldView<VertexBlobField::Pos>();

            posView[ 0] = Vector3f(-1.0f, -1.0f, -1.0f);
            posView[ 1] = Vector3f( 1.0f, -1.0f, -1.0f);
            posView[ 2] = Vector3f( 1.0f,  1.0f, -1.0f);
            posView[ 3] = Vector3f( 1.0f,  1.0f, -1.0f);
            posView[ 4] = Vector3f(-1.0f,  1.0f, -1.0f);
            posView[ 5] = Vector3f(-1.0f, -1.0f, -1.0f);

            posView[ 6] = Vector3f(-1.0f, -1.0f,  1.0f);
            posView[ 7] = Vector3f( 1.0f, -1.0f,  1.0f);
            posView[ 8] = Vector3f( 1.0f,  1.0f,  1.0f);
            posView[ 9] = Vector3f( 1.0f,  1.0f,  1.0f);
            posView[10] = Vector3f(-1.0f,  1.0f,  1.0f);
            posView[11] = Vector3f(-1.0f, -1.0f,  1.0f);

            posView[12] = Vector3f(-1.0f,  1.0f,  1.0f);
            posView[13] = Vector3f(-1.0f,  1.0f, -1.0f);
            posView[14] = Vector3f(-1.0f, -1.0f, -1.0f);
            posView[15] = Vector3f(-1.0f, -1.0f, -1.0f);
            posView[16] = Vector3f(-1.0f, -1.0f,  1.0f);
            posView[17] = Vector3f(-1.0f,  1.0f,  1.0f);

            posView[18] = Vector3f( 1.0f,  1.0f,  1.0f);
            posView[19] = Vector3f( 1.0f,  1.0f, -1.0f);
            posView[20] = Vector3f( 1.0f, -1.0f, -1.0f);
            posView[21] = Vector3f( 1.0f, -1.0f, -1.0f);
            posView[22] = Vector3f( 1.0f, -1.0f,  1.0f);
            posView[23] = Vector3f( 1.0f,  1.0f,  1.0f);

            posView[24] = Vector3f(-1.0f, -1.0f, -1.0f);
            posView[25] = Vector3f( 1.0f, -1.0f, -1.0f);
            posView[26] = Vector3f( 1.0f, -1.0f,  1.0f);
            posView[27] = Vector3f( 1.0f, -1.0f,  1.0f);
            posView[28] = Vector3f(-1.0f, -1.0f,  1.0f);
            posView[29] = Vector3f(-1.0f, -1.0f, -1.0f);

            posView[30] = Vector3f(-1.0f,  1.0f, -1.0f);
            posView[31] = Vector3f( 1.0f,  1.0f, -1.0f);
            posView[32] = Vector3f( 1.0f,  1.0f,  1.0f);
            posView[33] = Vector3f( 1.0f,  1.0f,  1.0f);
            posView[34] = Vector3f(-1.0f,  1.0f,  1.0f);
            posView[35] = Vector3f(-1.0f,  1.0f, -1.0f);
        }

        if ((fields & VertexBlobField::Norm) != VertexBlobField::Empty)
        {
            ArrayView<Vector3f> normView = vertexBlob->GetFieldView<VertexBlobField::Norm>();
            normView[ 0] = Vector3f(0.0f,  0.0f, -1.0f);
            normView[ 1] = Vector3f(0.0f,  0.0f, -1.0f);
            normView[ 2] = Vector3f(0.0f,  0.0f, -1.0f);
            normView[ 3] = Vector3f(0.0f,  0.0f, -1.0f);
            normView[ 4] = Vector3f(0.0f,  0.0f, -1.0f);
            normView[ 5] = Vector3f(0.0f,  0.0f, -1.0f);

            normView[ 6] = Vector3f(0.0f,  0.0f, 1.0f);
            normView[ 7] = Vector3f(0.0f,  0.0f, 1.0f);
            normView[ 8] = Vector3f(0.0f,  0.0f, 1.0f);
            normView[ 9] = Vector3f(0.0f,  0.0f, 1.0f);
            normView[10] = Vector3f(0.0f,  0.0f, 1.0f);
            normView[11] = Vector3f(0.0f,  0.0f, 1.0f);

            normView[12] = Vector3f(-1.0f,  0.0f, 0.0f);
            normView[13] = Vector3f(-1.0f,  0.0f, 0.0f);
            normView[14] = Vector3f(-1.0f,  0.0f, 0.0f);
            normView[15] = Vector3f(-1.0f,  0.0f, 0.0f);
            normView[16] = Vector3f(-1.0f,  0.0f, 0.0f);
            normView[17] = Vector3f(-1.0f,  0.0f, 0.0f);

            normView[18] = Vector3f(1.0f,  0.0f, 0.0f);
            normView[19] = Vector3f(1.0f,  0.0f, 0.0f);
            normView[20] = Vector3f(1.0f,  0.0f, 0.0f);
            normView[21] = Vector3f(1.0f,  0.0f, 0.0f);
            normView[22] = Vector3f(1.0f,  0.0f, 0.0f);
            normView[23] = Vector3f(1.0f,  0.0f, 0.0f);

            normView[24] = Vector3f(0.0f, -1.0f, 0.0f);
            normView[25] = Vector3f(0.0f, -1.0f, 0.0f);
            normView[26] = Vector3f(0.0f, -1.0f, 0.0f);
            normView[27] = Vector3f(0.0f, -1.0f, 0.0f);
            normView[28] = Vector3f(0.0f, -1.0f, 0.0f);
            normView[29] = Vector3f(0.0f, -1.0f, 0.0f);

            normView[30] = Vector3f(0.0f,  1.0f, 0.0f);
            normView[31] = Vector3f(0.0f,  1.0f, 0.0f);
            normView[32] = Vector3f(0.0f,  1.0f, 0.0f);
            normView[33] = Vector3f(0.0f,  1.0f, 0.0f);
            normView[34] = Vector3f(0.0f,  1.0f, 0.0f);
            normView[35] = Vector3f(0.0f,  1.0f, 0.0f);
        }

        if ((fields & VertexBlobField::TexCoords) != VertexBlobField::Empty)
        {
            ArrayView<Vector2f> textureCoordView = vertexBlob->GetFieldView<VertexBlobField::TexCoords>();
            textureCoordView[ 0] = Vector2f(0.0f, 0.0f);
            textureCoordView[ 1] = Vector2f(1.0f, 0.0f);
            textureCoordView[ 2] = Vector2f(1.0f, 1.0f);
            textureCoordView[ 3] = Vector2f(1.0f, 1.0f);
            textureCoordView[ 4] = Vector2f(0.0f, 1.0f);
            textureCoordView[ 5] = Vector2f(0.0f, 0.0f);

            textureCoordView[ 6] = Vector2f(0.0f, 0.0f);
            textureCoordView[ 7] = Vector2f(1.0f, 0.0f);
            textureCoordView[ 8] = Vector2f(1.0f, 1.0f);
            textureCoordView[ 9] = Vector2f(1.0f, 1.0f);
            textureCoordView[10] = Vector2f(0.0f, 1.0f);
            textureCoordView[11] = Vector2f(0.0f, 0.0f);

            textureCoordView[12] = Vector2f(1.0f, 0.0f);
            textureCoordView[13] = Vector2f(1.0f, 1.0f);
            textureCoordView[14] = Vector2f(0.0f, 1.0f);
            textureCoordView[15] = Vector2f(0.0f, 1.0f);
            textureCoordView[16] = Vector2f(0.0f, 0.0f);
            textureCoordView[17] = Vector2f(1.0f, 0.0f);

            textureCoordView[18] = Vector2f(1.0f, 0.0f);
            textureCoordView[19] = Vector2f(1.0f, 1.0f);
            textureCoordView[20] = Vector2f(0.0f, 1.0f);
            textureCoordView[21] = Vector2f(0.0f, 1.0f);
            textureCoordView[22] = Vector2f(0.0f, 0.0f);
            textureCoordView[23] = Vector2f(1.0f, 0.0f);

            textureCoordView[24] = Vector2f(0.0f, 1.0f);
            textureCoordView[25] = Vector2f(1.0f, 1.0f);
            textureCoordView[26] = Vector2f(1.0f, 0.0f);
            textureCoordView[27] = Vector2f(1.0f, 0.0f);
            textureCoordView[28] = Vector2f(0.0f, 0.0f);
            textureCoordView[29] = Vector2f(0.0f, 1.0f);

            textureCoordView[30] = Vector2f(0.0f, 1.0f);
            textureCoordView[31] = Vector2f(1.0f, 1.0f);
            textureCoordView[32] = Vector2f(1.0f, 0.0f);
            textureCoordView[33] = Vector2f(1.0f, 0.0f);
            textureCoordView[34] = Vector2f(0.0f, 0.0f);
            textureCoordView[35] = Vector2f(0.0f, 1.0f);
        }

        IndexBlobPtr indexBlob = std::make_shared<IndexBlob>(
            std::vector<int>
        {
             0,  1,  2,
             3,  4,  5,
             6,  7,  8,
             9, 10, 11,
            12, 13, 14,
            15, 16, 17,
            18, 19, 20,
            21, 22, 23,
            24, 25, 26,
            27, 28, 29,
            30, 31, 32,
            33, 34, 35
        });

        return std::make_shared<Model3d>(
            std::make_shared<MeshData>(vertexBlob, indexBlob), mat);
    }

    static Model3dPtr CreateTestCube(const Model3dPtr& another, const IMaterialPtr& meshStyle)
    {
        Model3dPtr result = std::make_shared<Model3d>(
            another->GetMeshData(),
            meshStyle);

        result->SetMatrix(another->GetMatrix());

        return result;
    }

    static void DoMovement(long long dt)
    {
        const float val = static_cast<float>(0.1f * static_cast<double>(dt) / 100000);

        if (g_keys[GLFW_KEY_S])
        {
            g_camera.SetPosition(g_camera.GetPosition() - g_camera.GetFront() * val);
        }

        if (g_keys[GLFW_KEY_W])
        {
            g_camera.SetPosition(g_camera.GetPosition() + g_camera.GetFront() * val);
        }

        if (g_keys[GLFW_KEY_A])
        {
            g_camera.SetPosition(g_camera.GetPosition() - g_camera.GetRight() * val);
        }

        if (g_keys[GLFW_KEY_D])
        {
            g_camera.SetPosition(g_camera.GetPosition() + g_camera.GetRight() * val);
        }

        if (g_keys[GLFW_KEY_SPACE])
        {
            g_camera.SetPosition(g_camera.GetPosition() + g_camera.GetUp() * val);
        }

        if (g_keys[GLFW_KEY_LEFT_SHIFT])
        {
            g_camera.SetPosition(g_camera.GetPosition() - g_camera.GetUp() * val);
        }
    }

    static void ProcessPolygonModeChange(const IMaterialPtr& material)
    {
        if (g_keys[GLFW_KEY_LEFT_CONTROL])
        {
            if (g_keys[GLFW_KEY_1])
            {
                material->SetPolygonMode(PolygonMode::Face);
            }
            else if (g_keys[GLFW_KEY_2])
            {
                material->SetPolygonMode(PolygonMode::Line);
            }
            else if (g_keys[GLFW_KEY_3])
            {
                material->SetPolygonMode(PolygonMode::Point);
            }
        }
    }

    static void ProcessFlashlight(LightsArrayPtr& lights, const SpotLightPtr& spotLight)
    {
        if (g_keys[GLFW_KEY_LEFT_CONTROL])
        {
            if (g_keys[GLFW_KEY_F] && lights->GetSpotLights().empty())
            {
                lights->AddSpotLight(spotLight);
            }

            if (g_keys[GLFW_KEY_G])
            {
                lights->ClearSpotLights();
            }
        }
    }

    void GLAPIENTRY DebugGl(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        userParam;
        const auto offset = std::setw(15);

        std::cout << "OpenGl message:" << std::endl;
        std::cout << offset << "id - " << std::fixed << id << std::endl;
        std::cout << offset << "source - ";

        switch (source)
        {
            case GL_DEBUG_SOURCE_API:
                std::cout << "DEBUG_SOURCE_API (Calls to the OpenGL API)";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                std::cout << "DEBUG_SOURCE_WINDOW_SYSTEM (Calls to a window-system API)";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                std::cout << "DEBUG_SOURCE_SHADER_COMPILER (A compiler for a shading language)";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                std::cout << "DEBUG_SOURCE_THIRD_PARTY (An application associated with OpenGL)";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                std::cout << "DEBUG_SOURCE_APPLICATION (Generated by the user of this application)";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                std::cout << "DEBUG_SOURCE_OTHER ";
                break;
            default:
                assert(false);
                break;
        }

        std::cout << std::endl;
        std::cout << offset << "type - ";

        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:
                std::cout << "DEBUG_TYPE_ERROR (An error, typically from the API)";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                std::cout << "DEBUG_TYPE_DEPRECATED_BEHAVIOR (Some behavior marked deprecated has been used)";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                std::cout << "DEBUG_TYPE_UNDEFINED_BEHAVIOR (Something has invoked undefined behavior)";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                std::cout << "DEBUG_TYPE_PORTABILITY (Some functionality the user relies upon is not portable)";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                std::cout << "DEBUG_TYPE_PERFORMANCE (Code has triggered possible performance issues)";
                break;
            case GL_DEBUG_TYPE_MARKER:
                std::cout << "DEBUG_TYPE_MARKER (Command stream annotation)";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                std::cout << "DEBUG_TYPE_PUSH_GROUP (Group pushing)";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                std::cout << "DEBUG_TYPE_POP_GROUP (Group popping)";
                break;
            case GL_DEBUG_TYPE_OTHER:
                std::cout << "DEBUG_TYPE_OTHER";
                break;
            default:
                assert(false);
                break;
        }

        std::cout << std::endl;
        std::cout << offset << "severity - ";

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                std::cout << "DEBUG_SEVERITY_HIGH (All OpenGL Errors, shader compilation / linking errors, or highly - dangerous undefined behavior)";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                std::cout << "DEBUG_SEVERITY_MEDIUM (Major performance warnings, shader compilation / linking warnings, or the use of deprecated functionality)";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                std::cout << "DEBUG_SEVERITY_LOW (Redundant state change performance warning, or unimportant undefined behavior)";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                std::cout << "DEBUG_SEVERITY_NOTIFICATION (Anything that isn't an error or performance issue)";
                break;
        }
        std::cout << std::endl;

        std::cout << offset << "message - " << ((length < 0) ? message : std::string(message, message + length)) << std::endl;

        std::cout << std::endl;
    }
}

void main()
{
    std::ostream& logstream = std::cout;
    logstream << "Starting GLFW context, OpenGL 3.3" << std::endl;
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
    if (window == nullptr)
    {
        logstream << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);
    // Set the required callback functions
    glfwSetKeyCallback(window, [](GLFWwindow* window,
        int key, int /*scancode*/, int action, int /*mode*/)
    {
        if (action == GLFW_PRESS)
            g_keys[key] = true;
        else if (action == GLFW_RELEASE)
            g_keys[key] = false;


        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }
    });

    glfwSetWindowSizeCallback(window, [](GLFWwindow* /*window*/, int w, int h)
    {
        glViewport(0, 0, w, h);
        g_camera.SetAspect(static_cast<float>(w)/ static_cast<float>(h));
    });

    glfwSetScrollCallback(window, [](GLFWwindow*, double, double yoffset)
    {
        const float dy = static_cast<float>(yoffset);

        float fov = g_camera.GetFOV();

        if (fov >= 1.0f && fov <= 45.0f)
            fov -= dy / 50.f;
        if (fov <= 1.0f)
            fov = 1.0f;
        if (fov >= 45.0f)
            fov = 45.0f;

        g_camera.SetFOV(fov);
    });

    /* Set this to true so GLEW knows to use a modern
     * approach to retrieving function pointers and extensions
     */
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    if (glewInit() != GLEW_OK)
    {
        logstream << "Failed to initialize GLEW" << std::endl;
        return;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(DebugGl, nullptr);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    std::vector<PointLightPtr> pointLights
    {
        std::make_shared<PointLight>(
            Vector3f(-5.f, -5.f, -5.f), // Position
            Vector3f(0.f, 0.f, 0.f),    // Ambient
            Vector3f(0.f, 1.f, 0.f),    // Diffuse
            Vector3f(0.f, 1.f, 0.f),    // Specular
            100.f),
        
        std::make_shared<PointLight>(
            Vector3f( 5.f, 5.f, 5.f), // Position
            Vector3f(0.f, 0.f, 0.f),  // Ambient
            Vector3f(1.f, 0.f, 0.f),  // Diffuse
            Vector3f(1.f, 0.f, 0.f),  // Specular
            100.f),
        
        std::make_shared<PointLight>(
            Vector3f(0.f, 0.f, 0.f), // Position
            Vector3f(0.f, 0.f, 0.f), // Ambient
            Vector3f(0.f, 0.f, 1.f), // Diffuse
            Vector3f(0.f, 0.f, 1.f), // Specular
            100.f)
    };

    std::vector<DirectionalLightPtr> directionalLights
    {
        std::make_shared<DirectionalLight>(
            Vector3f(0.f, -1.0f, 0.f),     // Direction
            Vector3f(0.f, 0.f, 0.f),       // Ambient
            Vector3f(0.5f, 0.5f, 0.5f),    // Diffuse
            Vector3f(0.5f, 0.5f, 0.5f)     // Specular
            )
    };

    SpotLightPtr flashLight =
        std::make_shared<SpotLight>
        (
            glm::radians(15.f),      // inner angle
            glm::radians(25.f),      // outer angle
            Vector3f(1.f, 0.f, 0.f), // Direction
            Vector3f(1.f, 0.f, 0.f), // Position
            Vector3f(1.f, 1.f, 1.f), // Ambient
            Vector3f(1.f, 1.f, 1.f), // Diffuse
            Vector3f(1.f, 1.f, 1.f)  // Specular
            );

    LightsArrayPtr lights =
        std::make_shared<LightsArray>(pointLights, directionalLights, std::vector<SpotLightPtr>(),
        "Data\\Shaders\\VS.glsl", "Data\\Shaders\\TexturedMaterialFS.glsl", &logstream);

    FragmentShaderPtr coloredMatFS = std::make_shared<FragmentShader>("Data\\Shaders\\NoLightColoredMaterialFS.glsl", nullptr, &logstream);
    ShaderProgramPtr coloredMaterialShader = std::make_shared<ShaderProgram>(
        lights->GetShader()->GetVertexShader(), coloredMatFS, &logstream);

    // Define the viewport dimensions
    glViewport(0, 0, WIDTH, HEIGHT);

    Texture2dPtr diffuseTexture = std::make_shared<Texture2D>("Data\\Textures\\container_diffuse.png", 0, TextureDataType::RGB, &logstream);
    diffuseTexture->SetFiltering(TextureFilteringType::Minifying, TextureFilteringMethod::Linear);
    diffuseTexture->SetFiltering(TextureFilteringType::Magnifying, TextureFilteringMethod::Linear);
    diffuseTexture->SetWrapping(Texture2DAxis::S, TextureWrappingType::ClampToBorder);
    diffuseTexture->SetWrapping(Texture2DAxis::T, TextureWrappingType::ClampToBorder);
    diffuseTexture->SetClampBorderColor(Vector4f(0.f, 0.f, 0.f, 0.f));

    Texture2dPtr specularTexture = std::make_shared<Texture2D>("Data\\Textures\\container_specular.png", 1, TextureDataType::RGB, &logstream);
    specularTexture->SetFiltering(TextureFilteringType::Minifying, TextureFilteringMethod::Linear);
    specularTexture->SetFiltering(TextureFilteringType::Magnifying, TextureFilteringMethod::Linear);
    specularTexture->SetWrapping(Texture2DAxis::S, TextureWrappingType::ClampToBorder);
    specularTexture->SetWrapping(Texture2DAxis::T, TextureWrappingType::ClampToBorder);
    specularTexture->SetClampBorderColor(Vector4f(0.f, 0.f, 0.f, 0.f));

    std::vector<Model3dPtr> models;
    IMaterialPtr material = std::make_shared<TexturedMaterial>(lights->GetShader());
    static_cast<TexturedMaterial*>(material.get())->SetAmbient(diffuseTexture);
    static_cast<TexturedMaterial*>(material.get())->SetDiffuse(diffuseTexture);
    static_cast<TexturedMaterial*>(material.get())->SetSpecular(specularTexture);

    {
        const size_t cnt = 36;
        Model3dPtr baseModel = CreateTestCube(material);
        const size_t _sqrt = static_cast<size_t>(std::sqrt(cnt));

        for (size_t i = 0; i < cnt; ++i)
        {
            models.emplace_back(CreateTestCube(baseModel, material));
        }

        for (size_t x = 0; x < _sqrt; ++x)
        {
            for (size_t y = 0; y < _sqrt; ++y)
            {
                Model3dPtr& m = models[(x*_sqrt) + y];
                glm::mat4 matrix = m->GetMatrix();
                m->SetMatrix(glm::translate(matrix,
                    glm::vec3(float(5 * (x+1)), float(5 * (y+1)), 0.f)));
            }
        }
    }

    std::vector<ColoredMaterialPtr> coloredMaterials;
    std::vector<Model3dPtr> lightSourceObjects;

    for (auto& pointLight : lights->GetPointLights())
    {
        ColoredMaterialPtr coloredMaterial =
            std::make_shared<ColoredMaterial>(coloredMaterialShader);

        coloredMaterial->SetColor(pointLight->GetDiffuse());
        coloredMaterials.push_back(coloredMaterial);

        auto obj = CreateTestCube(models.front(), coloredMaterial);

        obj->SetMatrix(glm::translate(glm::mat4(), glm::vec3(
            pointLight->GetPosition().x(),
            pointLight->GetPosition().y(),
            pointLight->GetPosition().z())));

        lightSourceObjects.push_back(obj);
    }

    g_camera.SetPosition(glm::vec3(-10.f, 10.f, -10.f));
    g_camera.SetFront(glm::normalize(glm::vec3(1.f, 1.f, -1.f)));

    Stopwatch<std::chrono::high_resolution_clock> stopwatch;

    double lastCursorPos[2];
    glfwGetCursorPos(window, lastCursorPos, lastCursorPos + 1);

    float yaw = g_camera.GetYaw();
    float pitch = g_camera.GetPitch();

    const float mouseSensitivity = 0.001f;

    float val = .0f;
    while (!glfwWindowShouldClose(window))
    {
        /* Check if any events have been activiated (key pressed,
        * mouse moved etc.) and call corresponding response functions
        */
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int wndSize[2];
        double currentCursorPos[2];
        glfwGetCursorPos(window, currentCursorPos, currentCursorPos + 1);

        const float cursorPosDelta[]
        {
            float(currentCursorPos[0] - lastCursorPos[0]),
            float(currentCursorPos[1] - lastCursorPos[1])
        };

        lastCursorPos[0] = currentCursorPos[0];
        lastCursorPos[1] = currentCursorPos[1];

        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
        {
            yaw += cursorPosDelta[0] * mouseSensitivity;
            pitch -= cursorPosDelta[1] * mouseSensitivity;

            if (pitch > M_PI / 2)
                pitch = static_cast<float>(M_PI / 2);
            if (pitch < -M_PI / 2)
                pitch = -static_cast<float>(M_PI / 2);

            g_camera.SetAngles(yaw, pitch);
        }

        glfwGetWindowSize(window, wndSize, wndSize + 1);

        const float normCoords[2]
        {
            static_cast<float>(currentCursorPos[0] / wndSize[0]),
            static_cast<float>(currentCursorPos[1] / wndSize[1])
        };

        const glm::mat4& projection = g_camera.GetProjection();

        auto dt = stopwatch.GetElapsedTime<std::chrono::nanoseconds>();
        val += static_cast<float>(dt) / 1.0e+10f;

        lights->PrepareContext();

        coloredMaterialShader->Use();
        glUniformMatrix4fv(coloredMaterialShader->GetUniformLocation("view"), 1, GL_FALSE, glm::value_ptr(g_camera.GetViewMatrix()));
        glUniformMatrix4fv(coloredMaterialShader->GetUniformLocation("projection"), 1, GL_FALSE, glm::value_ptr(projection));


        auto& ptLights = lights->GetPointLights();
        for (size_t pointLightIndex = 0; pointLightIndex < ptLights.size(); ++pointLightIndex)
        {
            const float distance = std::abs(300.f * (std::sin(val * 2500) + pointLightIndex * 0.33f));
            auto& pointLight = ptLights[pointLightIndex];
            pointLight->SetAttenuation().SetDistance(distance);
        }

        for (auto& obj : lightSourceObjects)
        {
            obj->Draw();
        }

        //Draw scene models
        lights->GetShader()->Use();

        glUniform3f(lights->GetShader()->GetUniformLocation("viewPosition"),
            g_camera.GetPosition().x, g_camera.GetPosition().y, g_camera.GetPosition().z);
        glUniformMatrix4fv(lights->GetShader()->GetUniformLocation("view"),
            1, GL_FALSE, glm::value_ptr(g_camera.GetViewMatrix()));
        glUniformMatrix4fv(lights->GetShader()->GetUniformLocation("projection"),
            1, GL_FALSE, glm::value_ptr(projection));

        for (size_t i = 0; i < models.size(); ++i)
        {
            auto& model = models[i];

            glm::mat4 matrix;
            matrix *= glm::rotate(matrix, glm::radians((float(i + 1)/12.f) * sin(val)), glm::vec3(1.f, 0.f, 0.f));
            matrix *= glm::rotate(matrix, glm::radians((float(models.size() + 1 - i)/12.f) * cos(val)), glm::vec3(0.f, 1.f, 0.f));
            matrix *= model->GetMatrix();
            matrix = glm::rotate(matrix, glm::radians(sin(val)), glm::vec3(1.f, 0.f, 0.f));
            matrix = glm::rotate(matrix, glm::radians(sin(val)), glm::vec3(.0f, 1.f, 0.f));
            matrix = glm::rotate(matrix, glm::radians(sin(val)), glm::vec3(.0f, 0.f, 1.f));
            model->SetMatrix(matrix);

            model->Draw();
        }

        stopwatch.Start();
        DoMovement(dt);

        {
            const glm::vec3& pos = g_camera.GetPosition();
            flashLight->SetPosition(Vector3f(pos.x, pos.y, pos.z));

            const glm::vec3& dir = g_camera.GetFront();
            flashLight->SetDirection(Vector3f(-dir.x, -dir.y, -dir.z));
        }

        ProcessPolygonModeChange(material);
        ProcessFlashlight(lights, flashLight);
        for (const auto& coloredMaterial : coloredMaterials)
        {
            ProcessPolygonModeChange(coloredMaterial);
        }
        glfwSwapBuffers(window);
    }

    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();

    return;
}