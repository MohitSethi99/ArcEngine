// PBR

#type vertex
#version 450 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;
layout(location = 2) in vec3 a_Normal;
layout(location = 3) in int a_ObjectID;

layout (std140, binding = 0) uniform Camera
{
    mat4 u_View;
    mat4 u_Projection;
    mat4 u_ViewProjection;

    vec4 u_CameraPosition;
};

uniform mat4 u_Model;

layout (location = 0) out vec3 v_WorldPosition;
layout (location = 1) out vec2 v_TexCoord;
layout (location = 2) out vec3 v_Normal;
layout (location = 3) out vec3 v_CameraPosition;
layout (location = 4) out flat int v_ObjectID;

void main()
{
	v_WorldPosition = vec3(u_Model * vec4(a_Position, 1.0));
    v_TexCoord = a_TexCoord;
    v_CameraPosition = u_CameraPosition.xyz;

    v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

	v_ObjectID = a_ObjectID;

	gl_Position = u_ViewProjection * u_Model * vec4(a_Position, 1.0);
}


#type fragment
#version 450 core

const float PI = 3.141592653589793;

const int MAX_NUM_LIGHTS = 25;

struct Light
{
    vec4 u_Position;
    vec4 u_Color;
    
    /* packed into a vec4
    x: constant factor for attenuation
    y: linear factor
    z: quadratic factor
    w: light type */
    vec4 u_AttenFactors;
    
    // Used for directional and spot lights
    vec4 u_LightDir;

    float u_Intensity;
};

layout (std140, binding = 1) uniform LightBuffer
{
    Light u_Lights[MAX_NUM_LIGHTS];
    uint u_NumLights;
};

uniform vec4  u_Albedo;
uniform float u_Metallic;
uniform float u_Roughness;
uniform float u_AO;
uniform vec3  u_EmissionColor;
uniform float u_EmissiveIntensity;

uniform bool u_UseAlbedoMap;
uniform bool u_UseMetallicMap;
uniform bool u_UseNormalMap;
uniform bool u_UseRoughnessMap;
uniform bool u_UseOcclusionMap;
uniform bool u_UseEmissiveMap;

uniform samplerCube u_IrradianceMap;

uniform sampler2D u_AlbedoMap;
uniform sampler2D u_MetallicMap;
uniform sampler2D u_NormalMap;
uniform sampler2D u_RoughnessMap;
uniform sampler2D u_AmbientOcclusionMap;
uniform sampler2D u_EmissiveMap;

// =========================================
layout (location = 0) in vec3 v_WorldPosition;
layout (location = 1) in vec2 v_TexCoord;
layout (location = 2) in vec3 v_Normal;
layout (location = 3) in vec3 v_CameraPosition;
layout (location = 4) in flat int v_ObjectID;

// =========================================
layout (location = 0) out vec4 fragColor;
layout (location = 1) out int o_IDBuffer;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH * NdotH;

    float denom = PI * pow((NdotH2 * (a2 - 1.0) + 1.0), 2);

    return a2 / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float denom = NdotV * (1.0 - k) + k;

    return NdotV / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx1 = GeometrySchlickGGX(NdotV, roughness);
    float ggx2 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

vec3 GetNormalFromMap()
{
    vec3 tangentNormal = texture(u_NormalMap, v_TexCoord).xyz * 2.0 - 1.0;

    vec3 q1  = dFdx(v_WorldPosition);
    vec3 q2  = dFdy(v_WorldPosition);
    vec2 st1 = dFdx(v_TexCoord);
    vec2 st2 = dFdy(v_TexCoord);

    vec3 N = normalize(v_Normal);
    vec3 T = normalize(q1*st2.t - q2*st1.t);
    vec3 B = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

// =========================================
void main()
{
    vec3 albedo = u_UseAlbedoMap ? pow(texture(u_AlbedoMap, v_TexCoord).rgb * u_Albedo.rgb, vec3(2.2)) : u_Albedo.rgb;
    float metallic = u_UseMetallicMap ? texture(u_MetallicMap, v_TexCoord).r : u_Metallic;
    float roughness = u_UseRoughnessMap ? texture(u_RoughnessMap, v_TexCoord).r : u_Roughness;
    float ao = u_UseOcclusionMap ? texture(u_AmbientOcclusionMap, v_TexCoord).r : u_AO;

    vec3 emission = u_UseEmissiveMap ? texture(u_EmissiveMap, v_TexCoord).rgb : u_EmissionColor;
    emission *= u_EmissiveIntensity;

    vec3 N = u_UseNormalMap ? GetNormalFromMap() : normalize(v_Normal);

    vec3 V = normalize(v_CameraPosition - v_WorldPosition);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, albedo, metallic);

    // reflectance equation
    vec3 Lo = vec3(0.0);

    for (int i = 0; i < u_NumLights; ++i)
    {
        vec3 L = normalize(u_Lights[i].u_Position.rgb - v_WorldPosition);
        uint type = uint(round(u_Lights[i].u_AttenFactors.w));
        if (type == 0)
        {
            L = -1.0 * normalize(u_Lights[i].u_LightDir.xyz);
        }
        vec3 H = normalize(L + V);

        float attenuation = 1.0;
        if (type == 1)
        {
            vec4 attenFactor = u_Lights[i].u_AttenFactors;
            float distance = length(u_Lights[i].u_Position.xyz - v_WorldPosition);
            attenuation = 1.0 / (attenFactor[0] + attenFactor[1]*distance + attenFactor[2]*(distance*distance));
        }

        vec3 radiance = u_Lights[i].u_Color.rgb * u_Lights[i].u_Intensity * attenuation;

        // Cook-Torrance BRDF
        float NDF = DistributionGGX(N, H, roughness);
        float G = GeometrySmith(N, V, L, roughness);
        vec3 F = FresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denom = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular = numerator / denom;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;

        float NdotL = max(dot(N, L), 0.0);
        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }

    vec3 kS = FresnelSchlick(max(dot(N, V), 0.0), F0);
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;
    vec3 irradiance = texture(u_IrradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;
    vec3 ambient = (kD * diffuse) * ao;

    vec3 color = ambient + Lo + emission;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // apply gamma correction
    color = pow(color, vec3(1.0/2.2));

    fragColor = vec4(color, 1.0);

	o_IDBuffer = v_ObjectID;
}
