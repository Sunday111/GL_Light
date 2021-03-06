#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoords;

uniform mat4 model;
uniform mat3 normalMatrix;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragmentPosition;
out vec2 TextureCoords;

void main()
{
    gl_Position = projection * view * model
        * vec4(position.x, position.y, position.z, 1.0);

    FragmentPosition = vec3(model * vec4(position, 1.0f));

    Normal = normalMatrix * normal;
    TextureCoords = textureCoords;
}