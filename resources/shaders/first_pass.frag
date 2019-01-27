#version 330 core

#define MAX_POINT_LIGHTS 10
#define MAX_DIRECTED_LIGHTS 10

struct Camera
{
    vec3 position;
};

struct PointLight
{
    vec3 position;
    vec3 color; // TODO: need 3 different colors?
    vec3 attenuation; // constant, linear, quadratic terms
    vec3 intensity; // ambient, diffuse, specular intensities
};

struct DirectedLight
{
    vec3 direction;
    vec3 color;
    vec3 intensity;
};

struct Material
{
    float       shininess;
    sampler2D   diffuseTexture;
    sampler2D   specularTexture;
};


uniform Camera      u_Camera;

uniform int         u_PointLightsCount;
uniform PointLight  u_PointLights[MAX_POINT_LIGHTS];

uniform int             u_DirectedLightsCount;
uniform DirectedLight   u_DirectedLights[MAX_DIRECTED_LIGHTS];

uniform Material    u_Material;


in vec3 v_Position;
in vec3 v_Normal;
in vec3 v_Texture;


out vec4 o_Color;


void main()
{
    vec4 diffuseSample = texture(u_Material.diffuseTexture, v_Texture.st);
    vec4 specularSample = texture(u_Material.specularTexture, v_Texture.st);

    vec3 cameraDirection = normalize(u_Camera.position - v_Position);
    vec3 normal = normalize(v_Normal);

    int shininess = int(u_Material.shininess);

    vec3 finalColor = vec3(0.0);

    for (int i = 0; i < min(u_DirectedLightsCount, u_DirectedLights.length()); ++i)
    {
        vec3 lightDirection = normalize(-u_DirectedLights[i].direction);
        vec3 halfwayDir = normalize(cameraDirection + lightDirection);

        float ambientTerm = u_DirectedLights[i].intensity[0];
        float diffuseTerm = u_DirectedLights[i].intensity[1] * max(dot(normal, lightDirection), 0.0);
        float specularTerm = u_DirectedLights[i].intensity[2] * pow(max(dot(normal, halfwayDir), 0.0), shininess);

        finalColor += u_DirectedLights[i].color * (((ambientTerm + diffuseTerm) * diffuseSample.rgb) + (specularTerm * specularSample.rgb));
    }

    for (int i = 0; i < min(u_PointLightsCount, u_PointLights.length()); ++i)
    {
        vec3 lightVector = u_PointLights[i].position - v_Position;

        float lightDistance = length(lightVector);
        vec3 lightDirection = normalize(lightVector);
        vec3 halfwayDir = normalize(cameraDirection + lightDirection);

        float attenuation = 1.0 / (u_PointLights[i].attenuation[0] + u_PointLights[i].attenuation[1] * lightDistance + u_PointLights[i].attenuation[2] * pow(lightDistance, 2));

        float ambientTerm = u_PointLights[i].intensity[0];
        float diffuseTerm = u_PointLights[i].intensity[1] * max(dot(normal, lightDirection), 0.0);
        float specularTerm = u_PointLights[i].intensity[2] * pow(max(dot(normal, halfwayDir), 0.0), shininess);

        finalColor += attenuation * u_PointLights[i].color * (((ambientTerm + diffuseTerm) * diffuseSample.rgb) + (specularTerm * specularSample.rgb));
    }

    o_Color = vec4(finalColor, 1.0);
}
