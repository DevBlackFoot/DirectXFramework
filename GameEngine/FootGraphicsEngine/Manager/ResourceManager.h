#pragma once
#include "GraphicsCore/RasterizerState.h"
#include "Resources/Mesh.h"
#include "Resources/Texture.h"
#include "Define/YAMLBinaryLayout.h"

#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	class IDXObject;

	class UnityScene;

	// ���� �������� ��� �����ϵ��� DLL Export�� ���ش�.
	class GRAPHICSENGINE_DECLSPEC ResourceManager
	{
	private:
		static std::shared_ptr<ResourceManager> instance;

	public:
		ResourceManager()
		{}
		~ResourceManager() {};

		static std::shared_ptr<ResourceManager> GetInstance();

	private:
		using KeyResourceMap = std::map<uint64/*key*/, std::shared_ptr<Resource>>;

		std::array<KeyResourceMap, RESOURCE_TYPE_COUNT> resources;

		std::unordered_map<std::string, std::shared_ptr<IDXObject>> prefab;

		std::unordered_map<std::string, std::shared_ptr<YAMLBinaryData::Scene>> sceneData;

	public:
		// ��ΰ� �ִ� ���ҽ��� �߰��� �� ����Ѵ�.
		template<typename T>
		uint64 Load(const std::string& name, const std::wstring& path);

		// ���ҽ��� �߰��� �� ����Ѵ�.
		template<typename T>
		uint64 Add(const std::string& name);

		// ���ҽ��� name���� ���� �� ����Ѵ�.
		template<typename T>
		std::shared_ptr<T> Get(const std::string& name);

		// ���ҽ��� id�� ���� �� ����Ѵ�.
		template<typename T>
		std::shared_ptr<T> Get(const uint64 id);

		// ���ҽ� Ÿ���� ��´�.
		template<typename T>
		RESOURCE_TYPE GetResourceType();

		// �޽ø� meshes�� �����ϰ� resourceID�� return �մϴ�.
		template <typename T>
		const uint64& CreateMesh(std::vector<T> vertices, std::vector<std::vector<unsigned int>> indices, std::string name, D3D_PRIMITIVE_TOPOLOGY topology, ComPtr<ID3D11RasterizerState> rasterizerState);
		
		// Mesh�� Prefab�� �ҷ��ͼ� �����Ѵ�.
		void Initialize();

		void LoadCubeMesh();			// CUBE_MESH = 0;

		void LoadSphereMesh();			// SPHERE_MESH = 1;

		void LoadAxisMesh();			// AXIS_MESH = 2;

		void LoadGridMesh();			// GRID_MESH = 3;

		void LoadScreenMesh();			// SCREEN_MESH = 4;

		void LoadDebugScreenMesh();		// DEBUG_SCREEN_MESH = 5;

		void LoadWireCubeMesh();		// WIRECUBE_MESH = 6;

		void LoadPlaneMesh();			// PLANE_MESH = 7;

		void LoadPrefab();

		void LoadUnityScene();

		const std::shared_ptr<IDXObject> GetPrefab(std::string name);

		const std::shared_ptr<YAMLBinaryData::Scene> GetSceneData(std::string name);
	};

	template<typename T>
	inline uint64 ResourceManager::Load(const std::string& name, const std::wstring& path)
	{
		// ���ҽ�Ÿ���� ���´�.
		RESOURCE_TYPE resourceType = GetResourceType<T>();
		
		// reources[type] �� ���� resources�� �ִ� keyResourceMap�� ���´�.
		KeyResourceMap& keyResourceMap = resources[static_cast<uint8>(resourceType)];

		for (auto& res : keyResourceMap)
		{
			// �̹� �ִ��Ŷ�� �����´�.
			if (res.second->GetName().compare(name) == 0)
			{
				return res.second->GetID();
			}
		}
		
		std::shared_ptr<T> resource = std::make_shared<T>();

		uint64 id = resource->GetID();

		resource->SetName(name);

		resource->Load(path);

		keyResourceMap[id] = resource;

		return id;
	}

	template<typename T>
	inline uint64 ResourceManager::Add(const std::string& name)
	{
		// ���ҽ�Ÿ���� ���´�.
		RESOURCE_TYPE resourceType = GetResourceType<T>();

		// reources[type] �� ���� resources�� �ִ� keyResourceMap�� ���´�.
		KeyResourceMap& keyResourceMap = resources[static_cast<uint8>(resourceType)];

		for (auto& res : keyResourceMap)
		{
			// �̹� �ִ��Ŷ�� �����´�.
			if (res.second->GetName().compare(name) == 0)
			{
				return res.second->GetID();
			}
		}

		std::shared_ptr<T> resource = std::make_shared<T>();

		uint64 id = resource->GetID();

		resource->SetName(name);

		keyResourceMap[id] = resource;

		return id;
	}

	template<typename T>
	inline std::shared_ptr<T> ResourceManager::Get(const std::string& name)
	{
		// ���ҽ�Ÿ���� ���´�.
		RESOURCE_TYPE resourceType = GetResourceType<T>();

		// reources[type] �� ���� resources�� �ִ� keyResourceMap�� ���´�.
		KeyResourceMap& keyResourceMap = resources[static_cast<uint8>(resourceType)];

		for (auto& res : keyResourceMap)
		{
			// �̹� �ִ��Ŷ�� �����´�.
			if (res.second->GetName().compare(name) == 0)
			{
				return static_pointer_cast<T>(res.second);
			}
		}

		return nullptr;
	}

	template<typename T>
	inline std::shared_ptr<T> ResourceManager::Get(const uint64 id)
	{
		// ���ҽ�Ÿ���� ���´�.
		RESOURCE_TYPE resourceType = GetResourceType<T>();

		// reources[type] �� ���� resources�� �ִ� keyResourceMap�� ���´�.
		KeyResourceMap& keyResourceMap = resources[static_cast<uint8>(resourceType)];

		// ���� ã�� ������ �����ٸ� �׳� keyResourceMap[id] �� return �ϴ°ɷ� �ٲ���..
		auto findIt = keyResourceMap.find(id);

		if (findIt != keyResourceMap.end())
			return static_pointer_cast<T>(findIt->second);

		return nullptr;
	}

	template<typename T>
	inline RESOURCE_TYPE ResourceManager::GetResourceType()
	{
		if (std::is_same_v<T, RenderingMaterial>)
			return RESOURCE_TYPE::MATERIAL;
		else if (std::is_same_v<T, Mesh>)
			return RESOURCE_TYPE::MESH;
		else if (std::is_same_v<T, Texture>)
			return RESOURCE_TYPE::TEXTURE;
		else
			return RESOURCE_TYPE::NONE;
	}

	template<typename T>
	inline const uint64& ResourceManager::CreateMesh(std::vector<T> vertices, std::vector<std::vector<unsigned int>> indices, std::string name, D3D_PRIMITIVE_TOPOLOGY topology, ComPtr<ID3D11RasterizerState> rasterizerState)
	{
		std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

		mesh->SetName(name);

		uint64 id = mesh->GetID();

		mesh->SetPrimitiveTopology(topology);
		
		mesh->SetRenderState(rasterizerState);

		mesh->CreateVertexBuffer(vertices);
		for (int i = 0; i < indices.size(); i++)
		{
			mesh->CreateIndexBuffer(indices[i]);
		}
		mesh->SetStride(sizeof(T));

		KeyResourceMap& keyResourceMap = resources[static_cast<uint8>(RESOURCE_TYPE::MESH)];

		keyResourceMap[id] = mesh;

		return id;
	}
}