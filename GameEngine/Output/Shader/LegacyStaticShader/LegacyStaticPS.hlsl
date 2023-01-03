// Legacy 빛 연산을 사용하는 Static Mesh의 Pixel Shader입니다.

// 이후에 필요하다면 추가하겠지만 이 쉐이더 코드는 기본적으로 모델이 있음을 가정합니다
#include "LegacyStaticTypes.hlsli"

// 여기서 픽셀 연산에 필요한 각종 값들을 받아옵니다
    // TexTransform..이나(생각해보니 이거 특수한 경우 아니면 기본적으로 Identity라서 안받은거 같은데..)

// light 값, material.. 등등

// 텍스쳐 맵
Texture2D DiffuseMap : register(t0);
Texture2D NormalMap : register(t1);

// 샘플러
    // 외부에서 만들어서 넣어주는 느낌.
SamplerState Sampler : register(s0);

// View..
// 빛 연산을 위한 cbView
cbuffer cbView : register(b0)
{
    float3 ViewPosition;
}

// 과거 lightHelper.fx 에 있던 구조체들
// directLight
cbuffer cbDirectionalLight : register(b1)
{
    float3 DLightDir;
    float4 DLightDiffuse;
    float4 DLightAmbient;
    float DLightSpecular;
}

// point Light
cbuffer cbPointLight : register(b2)
{
    float4 PLightPos;
    float4 PLightDiffuse;
    float PLightSpecular;
    float PLightIntensity;
}

// Material
cbuffer cbMaterial : register(b3)
{
    float4 MaterialAmbient;
    float4 MaterialDiffuse;
    float4 MaterialSpecular;
}

// outPut
    // 연결된 Render 타겟에 각각의 정보를 Output 해줍니다.
struct PS_Output
{
    // 스크린에 나오는 위치..?
    float4 Screen : SV_TARGET0;
    // depth
    float4 Depth : SV_TARGET1;
    // normal
    float4 Normal : SV_TARGET2;
    // albedo => Texture만
    float4 Albedo : SV_TARGET3;
    // 월드
    float4 WorldPos : SV_TARGET4;
};

// 픽셀 쉐이더
PS_Output main(VS_Output pin) : SV_Target
{
    float3 toEye = ViewPosition - pin.PosW;

    // 위의 값 노말라이즈..
    float distToEye = length(toEye);
    toEye /= distToEye;

    // 기본 텍스쳐 색상은 곱셈의 항등원
    float4 texColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

#ifdef USE_ALBEDO
    texColor = DiffuseMap.Sample(Sampler, pin.Tex);
#endif

#ifdef USE_NORMAL
    // 노멀맵
    float3 bumpedNormalW;
    float3 normalMapSample = NormalMap.Sample(Sampler, pin.Tex).rgb;

    // lighthelper에 있던 것 가져오기.
    // 각 성분을 -1, 1로 사상
    float3 normalT = 2.0f * normalMapSample - 1.0f;

    // 정규 직교 벡터
    pin.NormalW = normalize(pin.NormalW);
    /*float3 N = pin.NormalW;
    float3 T = normalize(pin.TangentW - dot(pin.TangentW, N) * N);
    float3 B = cross(N, T);

    float3x3 TBN = float3x3(T, B, N);

    bumpedNormalW = mul(normalT, TBN);*/
#endif

    // 빛 연산
    // 일단은 TexColor를 받아서 연산 시작
    float4 litColor = texColor;

    // 재질 속성
    float4 ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
    float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // Dirlight
    float3 lightVec = -DLightDir;

    // 주변광항을 더한다.
    ambient += MaterialAmbient * DLightAmbient;

    // 확산광 계산
        // 노말맵을 입힌 곳을 계산.
    float diffuseFactor = dot(lightVec, pin.NormalW);

    // 확산광이 있을때만..
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        // specular
        float3 v = reflect(-lightVec, pin.NormalW);
        float specFactor = pow(max(dot(v, toEye), 0.0f), MaterialSpecular.w);

        diffuse += diffuseFactor * MaterialDiffuse * DLightDiffuse;
        specular += specFactor * MaterialSpecular * DLightSpecular;
    }

#ifdef USE_POINTLIGHT
    // PointLight
    lightVec = PLightPos.xyz - pin.PosW;

    // 표면에서의 점과 광원 사이의 거리
    float d = length(lightVec);
    lightVec = normalize(lightVec);

    // 길이가 범위 내에 있을 때만.
    [flatten]
    if (d < PLightIntensity)
    {
        lightVec /= d;

        // 엠비언트 => 위의것 그대로 쓴다고 생각..
        // 빛이 막히지 않고 표면에 도달한다고 가정하고..
        diffuseFactor = dot(lightVec, pin.NormalW);


        [flatten]
        if (diffuseFactor > 0.0f)
        {
            float intensity = 1.0f - (d / PLightIntensity);
            float3 v = reflect(-lightVec, pin.NormalW);
            float specFactor = pow(max(dot(v, toEye), 0.0f), MaterialSpecular.w);

            diffuse += diffuseFactor * MaterialDiffuse * PLightDiffuse * intensity;
            specular += specFactor * MaterialSpecular * PLightSpecular * intensity;
        }

    }
#endif

    // 변조후 가산
    // 감마 Correction
    litColor = pow(litColor, 2.2f);
    litColor = texColor * (ambient + diffuse) /* + specular */;
    litColor = pow(litColor, 1 / 2.2f);

    litColor.a = MaterialDiffuse.a * texColor.a;

    PS_Output output = (PS_Output)0;

    output.Screen = litColor;
    output.Depth = pin.PosH.z;
    output.Normal = float4(pin.NormalW, 1.0f);
    output.Albedo = DiffuseMap.Sample(Sampler, pin.Tex);
    output.WorldPos = float4(pin.PosW, 1.0f);

    return output;
}