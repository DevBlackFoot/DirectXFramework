#include "GraphicsPch.h"
#include "ResourceManager.h"
#include "BuilderManager.h"
#include "Object/PBRObj.h"
#include "Resources/UnityScene.h"

namespace GraphicsEngineSpace
{
	std::shared_ptr<ResourceManager> ResourceManager::instance = nullptr;

	std::shared_ptr<ResourceManager> ResourceManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<ResourceManager>();

		return instance;
	}

	void ResourceManager::Initialize()
	{
		// 순서를 반드시 지켜 주어야 합니다.
		LoadCubeMesh();

		LoadSphereMesh();

		LoadAxisMesh();

		LoadGridMesh();

		LoadScreenMesh();

		LoadDebugScreenMesh();

		LoadWireCubeMesh();

		LoadPlaneMesh();

		LoadPrefab();

		LoadUnityScene();
	}

	void ResourceManager::LoadCubeMesh()
	{
		float w2 = 0.5f;
		float h2 = 0.5f;
		float d2 = 0.5f;

		std::vector<VertexStruct::PBRStaticVertex> vertices(24);

		// 앞면
		vertices[0] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, -d2), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[1] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, -d2), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[2] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, -d2), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[3] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, -d2), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		// 뒷면
		vertices[4] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, +d2), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[5] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, +d2), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[6] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, +d2), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[7] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, +d2), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		// 윗면
		vertices[8] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, -d2), SimpleMath::Vector3(0.0f, 1.0f, 0.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[9] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, +d2), SimpleMath::Vector3(0.0f, 1.0f, 0.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[10] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, +d2), SimpleMath::Vector3(0.0f, 1.0f, 0.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[11] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, -d2), SimpleMath::Vector3(0.0f, 1.0f, 0.0f), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		// 아랫면
		vertices[12] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, -d2), SimpleMath::Vector3(0.0f, -1.0f, 0.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[13] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, -d2), SimpleMath::Vector3(0.0f, -1.0f, 0.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[14] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, +d2), SimpleMath::Vector3(0.0f, -1.0f, 0.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[15] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, +d2), SimpleMath::Vector3(0.0f, -1.0f, 0.0f), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		// 왼쪽면
		vertices[16] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, +d2), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[17] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, +d2), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[18] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, +h2, -d2), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[19] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(-w2, -h2, -d2), SimpleMath::Vector3(-1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, -1.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		// 오른쪽면
		vertices[20] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, -d2), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector2(0.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[21] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, -d2), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector2(0.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[22] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, +h2, +d2), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector2(1.0f, 0.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));
		vertices[23] = VertexStruct::PBRStaticVertex(SimpleMath::Vector3(+w2, -h2, +d2), SimpleMath::Vector3(1.0f, 0.0f, 0.0f), SimpleMath::Vector3(0.0f, 0.0f, 1.0f), SimpleMath::Vector2(1.0f, 1.0f), SimpleMath::Vector4(1.0f, 1.0f, 1.0f, 1.0f));

		std::vector<std::vector<unsigned int>> idx(1, std::vector<unsigned int>(36));

		// 앞면
		idx[0][0] = 0; idx[0][1] = 1; idx[0][2] = 2;
		idx[0][3] = 0; idx[0][4] = 2; idx[0][5] = 3;
		// 뒷면
		idx[0][6] = 4; idx[0][7] = 5; idx[0][8] = 6;
		idx[0][9] = 4; idx[0][10] = 6; idx[0][11] = 7;
		// 윗면
		idx[0][12] = 8; idx[0][13] = 9; idx[0][14] = 10;
		idx[0][15] = 8; idx[0][16] = 10; idx[0][17] = 11;
		// 아랫면
		idx[0][18] = 12; idx[0][19] = 13; idx[0][20] = 14;
		idx[0][21] = 12; idx[0][22] = 14; idx[0][23] = 15;
		// 왼쪽면
		idx[0][24] = 16; idx[0][25] = 17; idx[0][26] = 18;
		idx[0][27] = 16; idx[0][28] = 18; idx[0][29] = 19;
		// 오른쪽면
		idx[0][30] = 20; idx[0][31] = 21; idx[0][32] = 22;
		idx[0][33] = 20; idx[0][34] = 22; idx[0][35] = 23;

		CreateMesh(
			vertices,
			idx,
			"CubeMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);
	}

	void ResourceManager::LoadSphereMesh()
	{
		float radius = 0.5f;	// 구의 반지름
		uint32 stackCount = 20; // 가로 분할
		uint32 sliceCount = 20; // 세로 분할

		std::vector<VertexStruct::PBRStaticVertex> vertices;

		VertexStruct::PBRStaticVertex v;

		// 북극
		v.Pos = SimpleMath::Vector3(0.0f, radius, 0.0f);
		v.Tex = SimpleMath::Vector2(0.5f, 0.0f);
		v.Normal = v.Pos;
		XMVector3Normalize(v.Normal);
		v.Normal.Normalize();
		v.TangentU = SimpleMath::Vector3(1.0f, 0.0f, 1.0f);
		vertices.push_back(v);

		float stackAngle = XM_PI / stackCount;
		float sliceAngle = XM_2PI / sliceCount;

		float deltaU = 1.f / static_cast<float>(sliceCount);
		float deltaV = 1.f / static_cast<float>(stackCount);

		// 고리마다 돌면서 정점을 계산한다 (북극/남극 단일점은 고리가 X)
		for (uint32 y = 1; y <= stackCount - 1; ++y)
		{
			float phi = y * stackAngle;

			// 고리에 위치한 정점
			for (uint32 x = 0; x <= sliceCount; ++x)
			{
				float theta = x * sliceAngle;

				v.Pos.x = radius * sinf(phi) * cosf(theta);
				v.Pos.y = radius * cosf(phi);
				v.Pos.z = radius * sinf(phi) * sinf(theta);

				v.Tex = SimpleMath::Vector2(deltaU * x, deltaV * y);

				v.Normal = v.Pos;
				v.Normal.Normalize();

				v.TangentU.x = -radius * sinf(phi) * sinf(theta);
				v.TangentU.y = 0.0f;
				v.TangentU.z = radius * sinf(phi) * cosf(theta);
				v.TangentU.Normalize();

				vertices.push_back(v);
			}
		}

		// 남극
		v.Pos = SimpleMath::Vector3(0.0f, -radius, 0.0f);
		v.Tex = SimpleMath::Vector2(0.5f, 1.0f);
		v.Normal = v.Pos;
		v.Normal.Normalize();
		v.TangentU = SimpleMath::Vector3(1.0f, 0.0f, 0.0f);
		vertices.push_back(v);

		std::vector<std::vector<unsigned int>> idx(1);

		// 북극 인덱스
		for (uint32 i = 0; i <= sliceCount; ++i)
		{
			//  [0]
			//   |  \
			//  [i+1]-[i+2]
			idx[0].push_back(0);
			idx[0].push_back(i + 2);
			idx[0].push_back(i + 1);
		}

		// 몸통 인덱스
		uint32 ringVertexCount = sliceCount + 1;
		for (uint32 y = 0; y < stackCount - 2; ++y)
		{
			for (uint32 x = 0; x < sliceCount; ++x)
			{
				//  [y, x]-[y, x+1]
				//  |		/
				//  [y+1, x]
				idx[0].push_back(1 + (y)*ringVertexCount + (x));
				idx[0].push_back(1 + (y)*ringVertexCount + (x + 1));
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x));
				//		 [y, x+1]
				//		 /	  |
				//  [y+1, x]-[y+1, x+1]
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x));
				idx[0].push_back(1 + (y)*ringVertexCount + (x + 1));
				idx[0].push_back(1 + (y + 1) * ringVertexCount + (x + 1));
			}
		}

		// 남극 인덱스
		uint32 bottomIndex = static_cast<uint32>(vertices.size()) - 1;
		uint32 lastRingStartIndex = bottomIndex - ringVertexCount;
		for (uint32 i = 0; i < sliceCount; ++i)
		{
			//  [last+i]-[last+i+1]
			//  |      /
			//  [bottom]
			idx[0].push_back(bottomIndex);
			idx[0].push_back(lastRingStartIndex + i);
			idx[0].push_back(lastRingStartIndex + i + 1);
		}

		CreateMesh(
			vertices,
			idx,
			"SphereMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);
	}

	void ResourceManager::LoadAxisMesh()
	{
		std::vector<VertexStruct::ColorVertex> vertices =
		{
			// x축 => 빨간색
			{SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector4((const float*)&Colors::Red)},
			{SimpleMath::Vector3(50.0f, 0.0f, 0.0f), SimpleMath::Vector4((const float*)&Colors::Red)},

			// y축 초록색
			{SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector4((const float*)&Colors::GreenYellow)},
			{SimpleMath::Vector3(0.0f, 50.0f, 0.0f), SimpleMath::Vector4((const float*)&Colors::GreenYellow)},

			// z축 파란색
			{SimpleMath::Vector3(0.0f, 0.0f, 0.0f), SimpleMath::Vector4((const float*)&Colors::Blue)},
			{SimpleMath::Vector3(0.0f, 0.0f, 50.0f), SimpleMath::Vector4((const float*)&Colors::Blue)},
		};

		// 인덱스 버퍼 생성
		std::vector<std::vector<unsigned int>> indices(1);
		indices[0].emplace_back(0);
		indices[0].emplace_back(1);
		indices[0].emplace_back(2);
		indices[0].emplace_back(3);
		indices[0].emplace_back(4);
		indices[0].emplace_back(5);

		CreateMesh(
			vertices,
			indices,
			"AxisMesh",
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			RasterizerState::GetWireFrameRS()
		);
	}

	void ResourceManager::LoadGridMesh()
	{
		std::vector<VertexStruct::ColorVertex> vertices(84);

		float z_val = -10.0f;
		// for문을 돌면서 버텍스 생성.
		// 세로 버텍스
		for (int i = 0; i < 42; i += 2)
		{
			vertices[i] = { SimpleMath::Vector3(-10.0f, 0.0f, z_val), SimpleMath::Vector4((const float*)&Colors::White) };
			vertices[i + 1] = { SimpleMath::Vector3(10.0f, 0.0f, z_val), SimpleMath::Vector4((const float*)&Colors::White) };
			z_val++;
		}
		float x_val = -10.0f;
		for (int i = 42; i < 84; i += 2)
		{
			vertices[i] = { SimpleMath::Vector3(x_val, 0.0f, -10.0f), SimpleMath::Vector4((const float*)&Colors::White) };
			vertices[i + 1] = { SimpleMath::Vector3(x_val, 0.0f, 10.0f), SimpleMath::Vector4((const float*)&Colors::White) };
			x_val++;
		}

		// 인덱스 버퍼 생성
		// 총 라인의 개수 40개 => 인덱스의 개수 40 * 2개
		std::vector<std::vector<unsigned int>> indices(1, std::vector<unsigned int>(84));

		for (int i = 0; i < 84; i++)
		{
			indices[0][i] = i;
		}

		CreateMesh(
			vertices,
			indices,
			"GridMesh",
			D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			RasterizerState::GetWireFrameRS()
		);
	}

	void ResourceManager::LoadScreenMesh()
	{
		// Screen Mesh 생성
		std::vector<VertexStruct::PBRStaticVertex> vertices(6);

		// 좌 하단
		vertices[0].Pos = SimpleMath::Vector3(-1.f, -1.f, 0.f);
		vertices[0].Tex = SimpleMath::Vector2(0.f, 1.f);

		// 좌 상단
		vertices[1].Pos = SimpleMath::Vector3(-1.f, 1.f, 0.f);
		vertices[1].Tex = SimpleMath::Vector2(0.f, 0.f);

		// 우 상단
		vertices[2].Pos = SimpleMath::Vector3(1.f, 1.f, 0.f);
		vertices[2].Tex = SimpleMath::Vector2(1.f, 0.f);

		// 좌 하단
		vertices[3].Pos = SimpleMath::Vector3(-1.f, -1.f, 0.f);
		vertices[3].Tex = SimpleMath::Vector2(0.f, 1.f);

		// 우 상단
		vertices[4].Pos = SimpleMath::Vector3(1.f, 1.f, 0.f);
		vertices[4].Tex = SimpleMath::Vector2(1.f, 0.f);

		// 우 하단
		vertices[5].Pos = SimpleMath::Vector3(1.f, -1.f, 0.f);
		vertices[5].Tex = SimpleMath::Vector2(1.f, 1.f);

		std::vector<std::vector<unsigned int>> indices(1, std::vector<unsigned int>(6));

		for (int i = 0; i < 6; i++)
			indices[0][i] = i;

		CreateMesh(
			vertices,
			indices,
			"ScreenMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);
	}

	void ResourceManager::LoadDebugScreenMesh()
	{
		// Debug Screen Mesh 생성
		std::vector<VertexStruct::PBRStaticVertex> vertices(6);

		// 좌 하단
		vertices[0].Pos = SimpleMath::Vector3(-1.f, -0.6f, 0.f);
		vertices[0].Tex = SimpleMath::Vector2(0.f, 1.f);

		// 좌 상단
		vertices[1].Pos = SimpleMath::Vector3(-1.f, 1.f, 0.f);
		vertices[1].Tex = SimpleMath::Vector2(0.f, 0.f);

		// 우 상단
		vertices[2].Pos = SimpleMath::Vector3(1.f, 1.f, 0.f);
		vertices[2].Tex = SimpleMath::Vector2(1.f, 0.f);

		// 좌 하단
		vertices[3].Pos = SimpleMath::Vector3(-1.f, -0.6f, 0.f);
		vertices[3].Tex = SimpleMath::Vector2(0.f, 1.f);

		// 우 상단
		vertices[4].Pos = SimpleMath::Vector3(1.f, 1.f, 0.f);
		vertices[4].Tex = SimpleMath::Vector2(1.f, 0.f);

		// 우 하단
		vertices[5].Pos = SimpleMath::Vector3(1.f, -0.6f, 0.f);
		vertices[5].Tex = SimpleMath::Vector2(1.f, 1.f);

		std::vector<std::vector<unsigned int>> indices(1, std::vector<unsigned int>(6));

		for (int i = 0; i < 6; i++)
			indices[0][i] = i;

		CreateMesh(
			vertices,
			indices,
			"DebugScreenMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);
	}

	void ResourceManager::LoadWireCubeMesh()
	{
		float w2 = 0.5f;
		float h2 = 0.5f;
		float d2 = 0.5f;

		std::vector<VertexStruct::PosVertex> vertices(24);

		// 앞면
		vertices[0]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, -d2));
		vertices[1]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, -d2));
		vertices[2]  = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, -d2));
		vertices[3]  = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, -d2));
		// 뒷면
		vertices[4]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, +d2));
		vertices[5]  = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, +d2));
		vertices[6]  = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, +d2));
		vertices[7]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, +d2));
		// 윗면
		vertices[8]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, -d2));
		vertices[9]  = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, +d2));
		vertices[10] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, +d2));
		vertices[11] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, -d2));
		// 아랫면
		vertices[12] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, -d2));
		vertices[13] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, -d2));
		vertices[14] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, +d2));
		vertices[15] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, +d2));
		// 왼쪽면
		vertices[16] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, +d2));
		vertices[17] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, +d2));
		vertices[18] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, +h2, -d2));
		vertices[19] = VertexStruct::PosVertex(SimpleMath::Vector3(-w2, -h2, -d2));
		// 오른쪽면						
		vertices[20] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, -d2));
		vertices[21] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, -d2));
		vertices[22] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, +h2, +d2));
		vertices[23] = VertexStruct::PosVertex(SimpleMath::Vector3(+w2, -h2, +d2));

		std::vector<std::vector<unsigned int>> idx(1, std::vector<unsigned int>(36));

		// 앞면
		idx[0][0] = 0; idx[0][1] = 1; idx[0][2] = 2;
		idx[0][3] = 0; idx[0][4] = 2; idx[0][5] = 3;
		// 뒷면
		idx[0][6] = 4; idx[0][7] = 5; idx[0][8] = 6;
		idx[0][9] = 4; idx[0][10] = 6; idx[0][11] = 7;
		// 윗면
		idx[0][12] = 8; idx[0][13] = 9; idx[0][14] = 10;
		idx[0][15] = 8; idx[0][16] = 10; idx[0][17] = 11;
		// 아랫면
		idx[0][18] = 12; idx[0][19] = 13; idx[0][20] = 14;
		idx[0][21] = 12; idx[0][22] = 14; idx[0][23] = 15;
		// 왼쪽면
		idx[0][24] = 16; idx[0][25] = 17; idx[0][26] = 18;
		idx[0][27] = 16; idx[0][28] = 18; idx[0][29] = 19;
		// 오른쪽면
		idx[0][30] = 20; idx[0][31] = 21; idx[0][32] = 22;
		idx[0][33] = 20; idx[0][34] = 22; idx[0][35] = 23;

		CreateMesh(
			vertices,
			idx,
			"WireCubeMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetWireFrameRS()
		);

		CreateMesh(
			vertices,
			idx,
			"SkyBoxMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);
	}

	void ResourceManager::LoadPlaneMesh()
	{
		// Plane Mesh 생성
		std::vector<VertexStruct::PBRStaticVertex> vertices(6);

		// 좌 하단
		vertices[0].Pos = SimpleMath::Vector3(-5.f, 0.f, -5.f);
		vertices[0].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[0].Tex = SimpleMath::Vector2(0.f, 1.f);

		// 좌 상단
		vertices[1].Pos = SimpleMath::Vector3(-5.f, 0.f, 5.f);
		vertices[1].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[1].Tex = SimpleMath::Vector2(0.f, 0.f);

		// 우 상단
		vertices[2].Pos = SimpleMath::Vector3(5.f, 0.f, 5.f);
		vertices[2].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[2].Tex = SimpleMath::Vector2(1.f, 0.f);

		// 좌 하단
		vertices[3].Pos = SimpleMath::Vector3(-5.f, 0.f, -5.f);
		vertices[3].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[3].Tex = SimpleMath::Vector2(0.f, 1.f);

		// 우 상단
		vertices[4].Pos = SimpleMath::Vector3(5.f, 0.f, 5.f);
		vertices[4].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[4].Tex = SimpleMath::Vector2(1.f, 0.f);

		// 우 하단
		vertices[5].Pos = SimpleMath::Vector3(5.f, 0.f, -5.f);
		vertices[5].Normal = SimpleMath::Vector3(0.f, 1.f, 0.f);
		vertices[5].Tex = SimpleMath::Vector2(1.f, 1.f);

		std::vector<std::vector<unsigned int>> indices(1, std::vector<unsigned int>(6));

		for (int i = 0; i < 6; i++)
			indices[0][i] = i;

		CreateMesh(
			vertices,
			indices,
			"PlaneMesh",
			D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			RasterizerState::GetSolidRS()
		);
	}

	void ResourceManager::LoadPrefab()
	{
		std::shared_ptr<IDXObject> loby = std::make_shared<PBRObj>();
		loby = BuilderManger::GetInstance()->GetBuilder("PBRBuilder")->BuildDXObject(loby, "BinaryFile/loby.noob");
		prefab.insert(std::make_pair("Loby", loby));

		std::shared_ptr<IDXObject> testMap = std::make_shared<PBRObj>();
		testMap = BuilderManger::GetInstance()->GetBuilder("PBRBuilder")->BuildDXObject(testMap, "BinaryFile/TestMap.noob");
		prefab.insert(std::make_pair("TestMap", testMap));

		std::shared_ptr<IDXObject> joy = std::make_shared<PBRObj>();
		joy = BuilderManger::GetInstance()->GetBuilder("PBRBuilder")->BuildDXObject(joy, "BinaryFile/Joy.noob");
		prefab.insert(std::make_pair("Joy", joy));

		std::shared_ptr<IDXObject> capo = std::make_shared<PBRObj>();
		capo = BuilderManger::GetInstance()->GetBuilder("PBRBuilder")->BuildDXObject(capo, "BinaryFile/TestPlayer.noob");
		prefab.insert(std::make_pair("Capo", capo));
	}

	void ResourceManager::LoadUnityScene()
	{
		UnityScene unityScene;
		
		sceneData.insert(std::make_pair("SampleScene", unityScene.LoadUnitySceneBinary("BinaryFile/SampleScene.noob")));
	}

	const std::shared_ptr<IDXObject> ResourceManager::GetPrefab(std::string name)
	{
		auto findIt = prefab.find(name);

		if (findIt != prefab.end())
			return findIt->second;

		return nullptr;
	}

	const std::shared_ptr<YAMLBinaryData::Scene> ResourceManager::GetSceneData(std::string name)
	{
		auto findIt = sceneData.find(name);

		if (findIt != sceneData.end())
			return findIt->second;

		return nullptr;
	}
}