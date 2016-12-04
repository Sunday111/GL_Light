#ifndef VERSION_DEFINED
#version 330 core
#endif

struct Attenuation
{
    float constant;
    float linear;
    float quadratic;
};

struct Color
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct TexturedMaterial
{
    sampler2D ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};

in vec3 FragmentPosition;
in vec3 Normal;
in vec2 TextureCoords;

#ifdef POINT_LIGHTS
    #ifndef POINT_LIGHTS_CAPACITY
        #error "POINT_LIGHTS_CAPACITY macro is not defined"
    #endif

    struct PointLight
    {
        vec3 position;

        Attenuation attenuation;
        Color color;
    };

    uniform int pointLightsCount;
    uniform PointLight pointLights[POINT_LIGHTS_CAPACITY];

    vec3 CalculatePointLight(in PointLight light, in vec3 normal, in vec3 fragmentPosition, in vec3 viewDirection);
#endif //POINT_LIGHTS

#ifdef DIRECTIONAL_LIGHTS
    #ifndef DIRECTIONAL_LIGHTS_CAPACITY
        #error "DIRECTIONAL_LIGHTS_CAPACITY macro is not defined"
    #endif

    struct DirectionalLight
    {
        vec3 direction;
        Color color;
    };

    uniform int directionalLightsCount;
    uniform DirectionalLight directionalLights[DIRECTIONAL_LIGHTS_CAPACITY];

    vec3 CalculateDirectionalLight(in DirectionalLight light, in vec3 normal, in vec3 viewDirection);
#endif //DIRECTIONAL_LIGHTS

#ifdef SPOT_LIGHTS
    #ifndef SPOT_LIGHTS_CAPACITY
        #error "SPOT_LIGHTS_CAPACITY macro is not defined"
    #endif

    struct SpotLight
    {
        vec3 direction;
        vec3 position;
        float cosInnerAngle;
        float cosOuterAngle;

        Color color;
    };

    uniform int spotLightsCount;
    uniform SpotLight spotLights[SPOT_LIGHTS_CAPACITY];

    vec3 CalculateSpotLight(in SpotLight light, in vec3 normal, in vec3 fragmentPosition, in vec3 viewDirection);
#endif //SPOT_LIGHTS

uniform TexturedMaterial material;
uniform vec3 viewPosition;

out vec4 color;

void main()
{
    // Properties
    vec3 norm = normalize(Normal);
    vec3 viewDirection = normalize(viewPosition - FragmentPosition);

    vec3 result = vec3(0.0, 0.0, 0.0);

    #ifdef POINT_LIGHTS
    for(int i = 0; i < pointLightsCount; ++i)
    {
        result += CalculatePointLight(pointLights[i], norm, FragmentPosition, viewDirection);
    }
    #endif //POINT_LIGHTS

    #ifdef DIRECTIONAL_LIGHTS
    for(int i = 0; i < directionalLightsCount; ++i)
    {
        result += CalculateDirectionalLight(directionalLights[i], norm, viewDirection);
    }
    #endif //DIRECTIONAL_LIGHTS

    #ifdef SPOT_LIGHTS
    for(int i = 0; i < spotLightsCount; ++i)
    {
        result += CalculateSpotLight(spotLights[i], norm, FragmentPosition, viewDirection);
    }
    #endif //SPOT_LIGHTS

    color = vec4(result, 1.0);
}

#ifdef POINT_LIGHTS
// Calculates the color when using a directional light.
vec3 CalculatePointLight(in PointLight light, in vec3 normal, in vec3 fragmentPosition, in vec3 viewDirection)
{
    vec3 lightDirection = normalize(light.position - fragmentPosition);

    // Diffuse shading
    float diff = max(dot(normal, lightDirection), 0.0);

    // Specular shading
    vec3 reflectedDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);

    // Attenuation
    float distance = length(light.position - fragmentPosition);
    float attenuation = 1.0f / (
        light.attenuation.constant +
        light.attenuation.linear * distance + 
        light.attenuation.quadratic * (distance * distance));

    // Apply texture color
    vec3 ambient = light.color.ambient * vec3(texture(material.ambient, TextureCoords));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuse, TextureCoords));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specular, TextureCoords));

    // Apply attenuation
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    return (ambient + diffuse + specular);
}
#endif //POINT_LIGHTS

#ifdef DIRECTIONAL_LIGHTS
// Calculates the color when using a point light.
vec3 CalculateDirectionalLight(in DirectionalLight light, in vec3 normal, in vec3 viewDirection)
{
    vec3 lightDirection = normalize(-light.direction);

    // Diffuse shading
    float diff = max(dot(normal, lightDirection), 0.0);

    // Specular shading
    vec3 reflectedDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);

    // Combine results
    vec3 ambient = light.color.ambient * vec3(texture(material.diffuse, TextureCoords));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuse, TextureCoords));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specular, TextureCoords));

    return (ambient + diffuse + specular);
}
#endif //DIRECTIONAL_LIGHTS

#ifdef SPOT_LIGHTS
// Calculates the color when using a spotlight.
vec3 CalculateSpotLight(in SpotLight light, in vec3 normal, in vec3 fragmentPosition, in vec3 viewDirection)
{
    vec3 fragmentDirection = normalize(fragmentPosition - light.position);
    float cosAngle = dot(fragmentDirection, -light.direction);

    if(cosAngle < light.cosOuterAngle)
    {
        return vec3(0.0, 0.0, 0.0);
    }
    
    vec3 lightDirection = normalize(-light.direction);
    
    // Diffuse shading
    float diff = max(dot(normal, lightDirection), 0.0);

    // Specular shading
    vec3 reflectedDirection = reflect(-lightDirection, normal);
    float spec = pow(max(dot(viewDirection, reflectedDirection), 0.0), material.shininess);

    // Combine results
    vec3 ambient = light.color.ambient * vec3(texture(material.diffuse, TextureCoords));
    vec3 diffuse = light.color.diffuse * diff * vec3(texture(material.diffuse, TextureCoords));
    vec3 specular = light.color.specular * spec * vec3(texture(material.specular, TextureCoords));

    float range = light.cosInnerAngle - light.cosOuterAngle;
    float delta = cosAngle - light.cosOuterAngle;
    float portion = abs(delta / range);

    return (ambient + diffuse + specular) * portion;
}
#endif //SPOT_LIGHTS