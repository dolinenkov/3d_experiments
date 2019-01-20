#version 330 core


struct Camera
{
    vec3 position;
};

struct Light
{
    vec3 position;
    vec3 color; // TODO: need 3 different colors?
    vec3 attenuation; // constant, linear, quadratic terms
    vec3 intensity; // ambient, diffuse, specular intensities
};


struct Material
{
    float       shininess;
    sampler2D   diffuseTexture;
    sampler2D   specularTexture;
};


uniform Camera      u_Camera;
uniform Light       u_Light;
uniform Material    u_Material;


in vec3 v_Position;
in vec3 v_Normal;
in vec3 v_Texture;


out vec4 o_FragmentColor;


void main()
{
    vec3 lightVector = u_Light.position - v_Position;

    float lightDistance = length(lightVector);

    // ослабление света с расстоянием
    float attenuation = 1.0 - clamp(u_Light.attenuation[0] + u_Light.attenuation[1] * lightDistance + u_Light.attenuation[2] * pow(lightDistance, 2), 0.0, 1.0);


    vec4 diffuseTextureSample = texture(u_Material.diffuseTexture, v_Texture.st);
    vec4 specularTextureSample = texture(u_Material.specularTexture, v_Texture.st);


    vec3 normal = normalize(v_Normal);
    vec3 lightDirection = normalize(lightVector);


    vec3 cameraDirection = normalize(u_Camera.position - v_Position);
    vec3 reflectedLightDirection = reflect(-lightDirection, normal);


    int convertedShininess = int(2.0 + u_Material.shininess * (256.0 - 2.0));


    float ambientTerm = u_Light.intensity[0];
    float diffuseTerm = u_Light.intensity[1] * max(dot(normal, lightDirection), 0.0);
    float specularTerm = u_Light.intensity[2] * pow(max(dot(cameraDirection, reflectedLightDirection), 0.0), convertedShininess);


    vec3 finalColor = attenuation * u_Light.color * (((ambientTerm + diffuseTerm) * diffuseTextureSample.rgb) + (specularTerm * specularTextureSample.rgb));

    o_FragmentColor = vec4(finalColor, 1.0);
}
