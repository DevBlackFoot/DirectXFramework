// PBR 빛 연산에서 필요한 각종 함수들을 모아뒀습니다.

// 레퍼런스 : https://github.com/Nadrin/PBR/blob/master/data/shaders/hlsl/pbr.hlsl

// Specular coefficiant - fixed reflectance value for non-metals
#define F_ZERO float3(0.04f, 0.04f, 0.04f)
#define PI          3.14159265359f
#define EPSILON 0.000001f

#define DIRECTION_LIGHT_COUNT	1
#define POINT_LIGHT_COUNT		30
#define SPOT_LIGHT_COUNT		30

// Light Informations For PBR
struct DirectionalLight
{
    float3 color; // 빛의 색
    float power; // 빛의 세기

    float3 direction; // 방향
    float pad;

    float4x4 lightViewProj;
};

struct PointLight
{
    float3 color;
    float power;

    float3 position; // 위치
    float range; // 범위

    float4x4 lightViewProj;
};

struct SpotLight
{
    float3 color;
    float power;

    float3 direction; // 범위
    float halfAngle; // Spot Half Angle (== FOV) => 스포트 라이트가 퍼지는 범위

    float3 position; // 위치
    float range; // 반지름 범위

    float4x4 lightViewProj;
};

// Diffuse BRDF
// Burley B. "Physically Based Shading at Disney"
// SIGGRAPH 2012 Course: Practical Physically Based Shading in Film and Game Production, 2012.
float3 Disney_Diffuse(in float roughnessPercent, in float3 diffuseColor, in float NdotL, in float NdotV, in float LdotH)
{
    float energyBias = lerp(0.0f, 0.5f, roughnessPercent);
    float energyFactor = lerp(1.0f, 1.0f / 1.51f, roughnessPercent);

    float fd90 = energyBias + 2.0f * roughnessPercent * LdotH * LdotH;

    float lightScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotL, 5.0f);
    float viewScatter = 1.0f + (fd90 - 1.0f) * pow(1.0f - NdotV, 5.0f);

    return diffuseColor * lightScatter * viewScatter * energyFactor;
}

//float D_GGX(float roughness, float NoH, const float3 NxH)
//{
//    float a = NoH * roughness;
//    float k = roughness / (dot(NxH, NxH) + a * a);
//    float d = k * k * (1.0 / PI);
//    return min(d, 65504.0);
//}

// GGX Specular D (normal distribution)
// https://www.cs.cornell.edu/~srm/publications/EGSR07-btdf.pdf
float D_GGX(in float roughness2, in float NdotH)
{
    const float alpha = roughness2 * roughness2;
    const float NdotH2 = NdotH * NdotH; // NdotH2 = NdotH^2

    //const float lower = (NdotH2 * (alpha - 1.0f)) + 1.0f;
    const float lower = NdotH2 * alpha + (1.0f - NdotH2);
    return alpha / (PI * lower * lower);
}

// Shlick's approximation of Fresnel By Unity Engine - Specular_F_Fresnel_Shlick
float3 F_Shlick(in float3 specularColor, in float HdotV)
{
    float FC = pow(1.0f - HdotV, 5.0f);
    return specularColor + (float3(1.0f, 1.0f, 1.0f) - specularColor) * FC;
}

// Schlick-Smith specular G (visibility) By Unity Version
float G_Smith(float roughness2, float NdotV, float NdotL)
{
    float SmithV = NdotL * sqrt(NdotV * (NdotV - NdotV * roughness2) + roughness2);
    float SmithL = NdotV * sqrt(NdotL * (NdotL - NdotL * roughness2) + roughness2);

    return 0.5f / max(SmithV + SmithL, 1e-5f);
}

float GGX_Geometry(float cosThetaN, float roughness4)
{
    float cosTheta_sqr = saturate(cosThetaN * cosThetaN);
    float tan2 = (1.0f - cosTheta_sqr) / cosTheta_sqr;

    return 2.0f / (1.0f + sqrt(1.0f + roughness4 * tan2));
}

float G_GGX(in float roughness2, in float NdotV, in float NdotL)
{
    float alpha = roughness2 * roughness2;

    return GGX_Geometry(NdotV, alpha) * GGX_Geometry(NdotL, alpha);
}

float G_SmithShlick(in float roughness2, in float NdotV, in float NdotL)
{
    float r = sqrt(roughness2) + 1.0f;
    float k = (r * r) / 8.0f;

    float SmithV = NdotV / (NdotV * (1.0 - k) + k);
    float SmithL = NdotL / (NdotL * (1.0 - k) + k);

    return SmithV * SmithL;
}

float3 BRDF(in float roughness2, in float metallic, in float3 diffuseColor, in float3 specularColor, in float NdotH, in float NdotV, in float NdotL, in float HdotV)
{
    // Distribution & Geometry & Fresnel
    float D = D_GGX(roughness2, NdotH); // 미세면 분포 함수
    float G = G_GGX(roughness2, NdotV, NdotL); // 미세면 감쇠 함수
    float3 F = F_Shlick(specularColor, HdotV); // 프레넬 함수

    /*float3 kS = F;
    float3 kD = float3(1.0, 1.0, 1.0) - kS;
    kD *= 1.0 - metallic;*/

    float3 kD = lerp(float3(1.f, 1.f, 1.f) - F, float3(0.f, 0.f, 0.f), metallic);

    // Diffuse & Specular factors
    float denom = max(4.0f * NdotV * NdotL, 0.001f); // 0.001f just in case product is 0 , 일반적으로 스페큘러는 4로 나누는 쪽이 많은듯
    float3 specular_factor = saturate((D * F * G) / denom);
    float3 diffuse_factor = kD * diffuseColor;

    return (diffuse_factor + specular_factor) * NdotL;
}

float3 PBR_DirectionalLight(
    in float3 V, in float3 N, in DirectionalLight light,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic, in float shadow)
{
    // Output color
    float3 acc_color = float3(0.0f, 0.0f, 0.0f);

    // Burley roughness bias
    const float roughness2 = max(roughness * roughness, 0.01f);

    // Blend base colors
    const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
    const float3 c_spec = lerp(F_ZERO, albedo, metallic) * ambientOcclusion;

    // Calculate Directional Light
    const float3 L = normalize(-light.direction);
    const float3 H = normalize(V + L);

    // products
    const float NdotL = max(dot(N, L), EPSILON);
    const float NdotV = abs(dot(N, V)) + EPSILON;
    const float NdotH = max(dot(N, H), EPSILON);
    const float HdotV = max(dot(H, V), EPSILON);

    // BRDF
    float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);

    // Directional light
    acc_color += light.color.rgb * light.power * shadow * brdf_factor;

    return acc_color;
}

float3 PBR_PointLight(
    in float3 V, in float3 N, in PointLight lights[POINT_LIGHT_COUNT], in uint lightCount, in float3 position,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic)
{
    // Output color
    float3 acc_color = float3(0.0f, 0.0f, 0.0f);

    PointLight light;

    [unroll]
    for (uint i = 0; i < lightCount; i++)
    {
        light = lights[i];

        // Light vector (to light)
        float3 lightVec = light.position - position;
        float distance = length(lightVec);

        if (distance > light.range)
            continue;

        const float3 L = normalize(lightVec);
        const float3 H = normalize(V + L);

        // products
        const float NdotL = max(dot(N, L), EPSILON);
        const float NdotV = max(dot(N, V), EPSILON);
        const float NdotH = max(dot(N, H), EPSILON);
        const float HdotV = max(dot(H, V), EPSILON);

        // Attenuation
        float DistToLightNorm = 1.0 - saturate(distance * (1.0f / light.range));
        float Attn = DistToLightNorm * DistToLightNorm;

        float3 radiance = Attn * light.power;

        // Burley roughness bias
        const float roughness2 = max(roughness * roughness, 0.01f);

        // Blend base colors
        const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
        const float3 c_spec = lerp(F_ZERO, albedo, metallic) * ambientOcclusion;

        // BRDF
        float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);

        // Point light
        acc_color += light.color * radiance * brdf_factor;
    }

    return acc_color;
}

float3 PBR_SpotLight(
    in float3 V, in float3 N, in SpotLight lights[SPOT_LIGHT_COUNT], in uint lightCount, in float3 position,
    in float3 albedo, in float ambientOcclusion, in float roughness, in float metallic)
{
    // Output color
    float3 acc_color = float3(0.0f, 0.0f, 0.0f);

    SpotLight light;

    [unroll]
    for (uint i = 0; i < lightCount; i++)
    {
        light = lights[i];

        float3 lightVec = light.position - position;
        float distance = length(lightVec);

        if (distance > light.range)
            continue;

        const float3 L = normalize(lightVec);
        const float3 H = normalize(L + V);

        // products
        const float NdotL = max(dot(N, L), EPSILON);
        const float NdotV = max(dot(N, V), EPSILON);
        const float NdotH = max(dot(N, H), EPSILON);
        const float HdotV = max(dot(H, V), EPSILON);

        // Cone attenuation
        float attStart = light.range;
        float attRange = cos(light.halfAngle * attStart) - cos(light.halfAngle);
        attStart = cos(light.halfAngle);
        float cosAng = dot(-light.direction, L);
        float conAtt = saturate((cosAng - attStart) / attRange);
        conAtt *= conAtt;

        // Attenuation
        float DistToLightNorm = 1.0 - saturate(distance * (1.0f / light.range));
        float Attn = DistToLightNorm * DistToLightNorm;

        float3 radiance = Attn * conAtt * light.power;

        // Burley roughness bias
        const float roughness2 = max(roughness * roughness, 0.01f);

        // Blend base colors
        const float3 c_diff = lerp(albedo, float3(0, 0, 0), metallic) * ambientOcclusion;
        const float3 c_spec = lerp(F_ZERO, albedo, metallic) * ambientOcclusion;

        // BRDF
        float3 brdf_factor = BRDF(roughness2, metallic, c_diff, c_spec, NdotH, NdotV, NdotL, HdotV);

        // Spot light
        acc_color += light.color * radiance * brdf_factor;
    }

    return acc_color;
}

//---------------------------------------------------------------------------------------
// Transforms a normal map sample to world space.
//---------------------------------------------------------------------------------------
float3 NormalSampleToWorldSpace(float3 normalMapSample, float3 unitNormalW, float3 tangentW)
{
    // Uncompress each component from [0,1] to [-1,1].
    float3 normalT = 2.0f * normalMapSample - 1.0f;

    // Build orthonormal basis.
    float3 N = unitNormalW;
    float3 T = normalize(tangentW - dot(tangentW, N) * N);
    //float3 T = tangentW;
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    // Transform from tangent space to world space.
    float3 bumpedNormalW = mul(normalT, TBN);

    return bumpedNormalW;
}