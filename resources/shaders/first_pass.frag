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
    sampler2D albedo;
    sampler2D metalness;
    sampler2D normal;
    sampler2D roughness;
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


const float PI = 4.0 * atan(1.0);


void main()
{

    vec3 albedoSample = texture(u_Material.albedo, v_Texture.st).rgb;
    vec3 normalSample = texture(u_Material.normal, v_Texture.st).rgb;
    float metalnessSample = texture(u_Material.metalness, v_Texture.st).r;
    float roughnessSample = texture(u_Material.roughness, v_Texture.st).r;

    float alpha = roughnessSample;
    float alpha_sq = alpha * alpha;

    float k = pow(alpha + 1.0, 2.0) / 8.0;

    vec3 L0 = vec3(0.0);

    vec3 view = normalize(u_Camera.position - v_Position);
    vec3 normal = normalize(normalSample.rgb + v_Normal);
    float nv_dot = max(dot(normal, view), 0.0);
    vec3 f0 = mix(vec3(0.04), albedoSample, vec3(metalnessSample));

    // point lights
    for (int i = 0; i < min(u_PointLightsCount, u_PointLights.length()); ++i)
    {
        vec3 light = normalize(u_PointLights[i].position - v_Position);
        vec3 halfway = normalize(view + light);

        float attenuation = 1.0 / pow(length(u_PointLights[i].position - v_Position), 2.0);
        vec3 radiance = u_PointLights[i].color * u_PointLights[i].intensity * attenuation;

        float hv_dot = max(dot(halfway, view), 0.0);
        float nl_dot = max(dot(normal, light), 0.0);
        float nh_dot = max(dot(normal, halfway), 0.0);

        // NDF
        float ndf_denom = 1.0 + (nh_dot * nh_dot * (alpha_sq - 1.0));
        float ndf = alpha_sq / (PI * ndf_denom * ndf_denom);

        // Geometry
        float geometry_l = nl_dot / (k + (nl_dot * (1.0 - k)));
        float geometry_v = nv_dot / (k + (nv_dot * (1.0 - k)));
        float geometry = geometry_l * geometry_v;

        // Fresnel
        vec3 fresnel = f0 + ((vec3(1.0) - f0) * pow(1.0 - hv_dot, 5.0));

        // DFG
        vec3 dfg = fresnel * vec3(geometry * ndf);

        // Cook-Torrance function
        float cook_torrance_denom = max(4.0 * nv_dot * nl_dot, 0.001);
        vec3 cook_torrance = dfg / cook_torrance_denom;

        vec3 ks = fresnel;
        vec3 kd = (vec3(1.0) - ks) * (1.0 - metalnessSample);

        L0 += ((kd * albedoSample / PI) + cook_torrance) * radiance * nl_dot;
    }

    // directed lights
    for (int i = 0; i < min(u_DirectedLightsCount, u_DirectedLights.length()); ++i)
    {
        vec3 light = normalize(-u_DirectedLights[i].direction);
        vec3 halfway = normalize(view + light);

        vec3 radiance = u_DirectedLights[i].color * u_DirectedLights[i].intensity;

        float hv_dot = max(dot(halfway, view), 0.0);
        float nl_dot = max(dot(normal, light), 0.0);
        float nh_dot = max(dot(normal, halfway), 0.0);

        // NDF
        float ndf_denom = 1.0 + (nh_dot * nh_dot * (alpha_sq - 1.0));
        float ndf = alpha_sq / (PI * ndf_denom * ndf_denom);

        // Geometry
        float geometry_l = nl_dot / (k + (nl_dot * (1.0 - k)));
        float geometry_v = nv_dot / (k + (nv_dot * (1.0 - k)));
        float geometry = geometry_l * geometry_v;

        // Fresnel
        vec3 fresnel = f0 + ((vec3(1.0) - f0) * pow(1.0 - hv_dot, 5.0));

        // DFG
        vec3 dfg = fresnel * vec3(geometry * ndf);

        // Cook-Torrance function
        float cook_torrance_denom = max(4.0 * nv_dot * nl_dot, 0.001);
        vec3 cook_torrance = dfg / cook_torrance_denom;

        vec3 ks = fresnel;
        vec3 kd = (vec3(1.0) - ks) * (1.0 - metalnessSample);

        L0 += ((kd * albedoSample / PI) + cook_torrance) * radiance * nl_dot;
    }

    o_Color = vec4(L0 + vec3(albedoSample * 0.01), 1.0);
}
