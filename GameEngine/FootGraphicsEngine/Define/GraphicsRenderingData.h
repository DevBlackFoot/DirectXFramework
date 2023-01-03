#pragma once
//#include "Utils/d3d11Utils.h"
#include <string>
#include "SimpleMath.h"
#include "GraphicsTypedef.h"
#include "Resources/Resource.h"

// Mesh �ҷ������ �ش� �޽õ��� ���ҽ��Ŵ������� ���� ���� �ʱ�ȭ ������Ѵ�.
constexpr size_t CUBE_MESH = 0;
constexpr size_t SPHERE_MESH = 1;
constexpr size_t AXIS_MESH = 2;
constexpr size_t GRID_MESH = 3;
constexpr size_t SCREEN_MESH = 4;
constexpr size_t DEBUG_SCREEN_MESH = 5;
constexpr size_t WIRECUBE_MESH = 6;
constexpr size_t SKYBOX_MESH = 7;
constexpr size_t PLANE_MESH = 8;

// �Լ� �߰��� enum class
enum class TextureMapType
{
	ALBEDO,
	NORMAL,
	METALLIC,
	ROUGHNESS,
	AO,
	EMISSIVE,
	CUBE
};

class RenderingMaterial : public GraphicsEngineSpace::Resource
{
public:
	RenderingMaterial()
		: Resource(GraphicsEngineSpace::RESOURCE_TYPE::MATERIAL)
	{}
	~RenderingMaterial() {};

public:
	uint64 albedoMap = ULLONG_MAX;
	uint64 normalMap = ULLONG_MAX;
	uint64 metallicMap = ULLONG_MAX;
	uint64 roughnessMap = ULLONG_MAX;
	uint64 AOMap = ULLONG_MAX;
	uint64 emissiveMap = ULLONG_MAX;
	uint64 cubeMap = ULLONG_MAX;

	DirectX::SimpleMath::Vector4 material_Ambient = { 0.f, 0.f, 0.f, 0.f };
	DirectX::SimpleMath::Vector4 material_Diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };
	DirectX::SimpleMath::Vector4 material_Specular = { 0.f, 0.f, 0.f, 0.f };
	DirectX::SimpleMath::Vector4 material_Emissive = { 0.f, 0.f, 0.f, 0.f };

	float emissionFactor = 0.f;			// ���� ũ��

	float material_Transparency = 0.f;	// ����

	float material_Reflectivity = 0.f;	// �ݻ���

	float metallic = 0.1f;

	float roughness = 0.f;
};

struct ObjectInfo
{
	uint64 objectID;

	DirectX::SimpleMath::Matrix worldTM;

	DirectX::SimpleMath::Vector3 worldPos;

	DirectX::SimpleMath::Matrix finalBoneListMatrix[96];

	uint64 materialName;
};
