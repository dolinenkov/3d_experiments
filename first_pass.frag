#version 330 core

#define MAX_LIGHTS_COUNT 1

struct Material
{
    sampler2D   textureDiffuse;
    sampler2D   textureSpecular;
    float       shininess;
};

struct Light
{
    float intensityAmbient;
    float intensityDiffuse;
    float intensitySpecular;

    vec3 color;

    vec3 position;
};

struct Camera
{
    vec3 position;
};


uniform Material    u_Material;
uniform Light       u_Light;
uniform Camera      u_Camera;


in vec3             v_Position;
in vec3             v_Normal;
in vec3             v_Texture;


out vec4            o_FragmentColor;


void main()
{

    vec3 diffuseColor = texture(u_Material.textureDiffuse, v_Texture.st).rgb;
    vec3 specularColor = texture(u_Material.textureSpecular, v_Texture.st).rgb;

    vec3 ambientComponent = u_Light.intensityAmbient * u_Light.color;

    //

    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(v_Position - u_Light.position);
    vec3 diffuseComponent = max(dot(lightDirection, normal), 0.0) * u_Light.intensityDiffuse * u_Light.color;

    vec3 reflectionDirection = normalize(-lightDirection);
    vec3 viewDirection = u_Camera.position;

    int factor = int(round(2 * u_Material.shininess));
    vec3 specularComponent = pow(max(dot(reflectionDirection, viewDirection), factor), 0.0) * u_Light.intensitySpecular * u_Light.color;

    //

    vec3 finalColor = (diffuseColor * (ambientComponent + diffuseComponent)) + (specularColor * specularComponent);

    o_FragmentColor = vec4(finalColor, 1.0);
}
