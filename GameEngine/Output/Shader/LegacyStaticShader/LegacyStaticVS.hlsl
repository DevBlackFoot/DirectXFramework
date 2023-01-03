// PBR�� �ƴ� n dot l�� h dot n �� �̿��� �� ������ �ϴ� �������� ����� ���̴� �Դϴ�.
	// �ش� ���̴������� Static Mesh���� �����մϴ�.
#include "LegacyStaticTypes.hlsli"

// ����� View�� ������ �ǹ̰�..
	// ����� ������Ʈ��ü�� ��ǥ
	// View�� proj�� ī�޶� ���� ����� �� ����..
// �ϴ� ������ �̷��� �صΰ�, ���߿� �ʿ信 ���� �����ϴ� ������ �Ѵ�.

// �⺻���� ��� ����
cbuffer cbPerObject : register( b0 )
{
    float4x4 WorldViewProj;
    float4x4 World;
    float4x4 WorldInvTranspose;
}


// ���ؽ� ���̴��� ������ ��
// ��ġ, �븻, Tangent ��
struct VS_Input
{
    float3 PosL : POSITION;
    float3 NormalL : NORMAL;
    float3 TangentL : TANGENT;
    float2 Tex : TEXCOORD;
};

// ���ؽ� ���̴�
VS_Output main(VS_Input vin)
{
    VS_Output vout = (VS_Output)0;

    // World �������� ��ȯ
    vout.PosW = mul(float4(vin.PosL, 1.0f), World).xyz;
    vout.NormalW = normalize(mul(vin.NormalL, (float3x3) WorldInvTranspose));
    vout.TangentW = mul(vin.TangentL, (float3x3) World);

    // NDC ��������.
    vout.PosH = mul(float4(vin.PosL, 1.0f), WorldViewProj);

    // �ؽ���.
    vout.Tex = vin.Tex.xy;

    return vout;
}