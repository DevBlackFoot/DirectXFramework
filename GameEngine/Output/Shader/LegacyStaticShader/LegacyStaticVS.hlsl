// PBR이 아닌 n dot l과 h dot n 을 이용해 빛 연산을 하는 전통적인 방식의 쉐이더 입니다.
	// 해당 쉐이더에서는 Static Mesh만을 관리합니다.
#include "LegacyStaticTypes.hlsli"

// 월드와 View를 나누는 의미가..
	// 월드는 오브젝트자체의 좌표
	// View나 proj은 카메라에 따라 변경될 수 있음..
// 일단 지금은 이렇게 해두고, 나중에 필요에 따라 변경하는 것으로 한다.

// 기본적인 상수 버퍼
cbuffer cbPerObject : register( b0 )
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4x4 WorldInvTranspose;
}


// 버텍스 쉐이더에 들어오는 값
// 위치, 노말, Tangent 값
struct VS_Input
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float3 TangentL : TANGENT;
    float2 Tex : TEXCOORD;
};

// 버텍스 쉐이더
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output)0;

    // World 공간으로 변환
    vout.PosW = mul(float4(vin.PosL, 1.0f), World).xyz;
    vout.NormalW = normalize(mul(vin.NormalL, (float3x3) WorldInvTranspose));
    vout.TangentW = mul(vin.TangentL, (float3x3) World);

    // NDC 공간으로.
    vout.PosH = mul(float4(vin.PosL, 1.0f), WorldViewProj);

    // 텍스쳐.
    vout.Tex = vin.Tex.xy;

    return vout;
}