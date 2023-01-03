#pragma once

using namespace DirectX;

namespace GraphicsEngineSpace
{
	// 각종 버텍스 구조체 정보.
	namespace VertexStruct
	{
		// SkyBox를 위한 위치 정보만 있는 버텍스
		struct PosVertex
		{
			SimpleMath::Vector3 Pos;
		};

		// 가장 기본적인 색과 위치 정보를 가지고 있는 버텍스 구조체
		struct ColorVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector4 Color;
		};

		// 위치와 텍스쳐 좌표 정보를 가지고 있는 TexVertex
		struct TexVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector2 Tex;
		};

		// 빛 만 사용하는 LightVertex
		struct LightVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector3 Normal;
		};

		// 노말 맵 정보(탄젠트)를 포함하는 normalVertex
		struct NormalVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector3 Normal;
			SimpleMath::Vector3 TangentU;
			SimpleMath::Vector2 Tex;
		};

		// 스키닝 정보를 담는 버텍스 스트럭쳐.
		struct SkinnedVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector3 Normal;
			SimpleMath::Vector2 Tex;
			SimpleMath::Vector3 Weights;		// 본 하나 하나의 가중치 값을 담는다. 마지막은 쉐이더 코드에서 1 - 로 구한다.
			XMUINT4 BoneIndices;		// 각각의 가중치가 들어간 bone의 인덱스 정보.
		};

		// PBR static Mesh 정보를 담는 vertex
		struct PBRStaticVertex
		{
			SimpleMath::Vector3 Pos;
			SimpleMath::Vector3 Normal;
			SimpleMath::Vector3 TangentU;
			SimpleMath::Vector2 Tex;
			SimpleMath::Vector4 Color;
		};

		// PBR skinned Mesh 정보를 담는 vertex
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