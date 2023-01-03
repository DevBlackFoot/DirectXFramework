#pragma once

using namespace DirectX;

namespace GraphicsEngineSpace
{
	// ���� ���ؽ� ����ü ����.
	namespace VertexStruct
	{
		// SkyBox�� ���� ��ġ ������ �ִ� ���ؽ�
		struct PosVertex
		{
			SimpleMath::Vector3 Pos;
		};

		// ���� �⺻���� ���� ��ġ ������ ������ �ִ� ���ؽ� ����ü
		struct ColorVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector4 Color;
		};

		// ��ġ�� �ؽ��� ��ǥ ������ ������ �ִ� TexVertex
		struct TexVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector2 Tex;
		};

		// �� �� ����ϴ� LightVertex
		struct LightVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector3 Normal;
		};

		// �븻 �� ����(ź��Ʈ)�� �����ϴ� normalVertex
		struct NormalVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector3 Normal;
			SimpleMath::Vector3 TangentU;
			SimpleMath::Vector2 Tex;
		};

		// ��Ű�� ������ ��� ���ؽ� ��Ʈ����.
		struct SkinnedVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector3 Normal;
			SimpleMath::Vector2 Tex;
			SimpleMath::Vector3 Weights;		// �� �ϳ� �ϳ��� ����ġ ���� ��´�. �������� ���̴� �ڵ忡�� 1 - �� ���Ѵ�.
			XMUINT4 BoneIndices;		// ������ ����ġ�� �� bone�� �ε��� ����.
		};

		// PBR static Mesh ������ ��� vertex
		struct PBRStaticVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector3 Normal;
			SimpleMath::Vector3 TangentU;
			SimpleMath::Vector2 Tex;
			SimpleMath::Vector4 Color;
		};

		// PBR skinned Mesh ������ ��� vertex
		struct PBRSkinnedVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector3 Normal;
			SimpleMath::Vector3 TangentU;
			SimpleMath::Vector2 Tex;
			SimpleMath::Vector4 Color;
			SimpleMath::Vector4 Weight;
			SimpleMath::Vector4 Weights;
			XMUINT4 BoneIndex;
			XMUINT4 BoneIndices;
		};
	}

}