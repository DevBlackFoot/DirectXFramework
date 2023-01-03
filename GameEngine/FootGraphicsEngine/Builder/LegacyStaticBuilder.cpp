#include "GraphicsPch.h"
#include "LegacyStaticBuilder.h"

#include "Interface/IDXObject.h"
#include "Object/LegacyStaticObj.h"
#include "GraphicsCore/RasterizerState.h"

#include "Manager/ResourceManager.h"
#include <typeinfo>

namespace GraphicsEngineSpace
{
	LegacyStaticBuilder::~LegacyStaticBuilder()
	{
	}

	std::shared_ptr<IDXObject> LegacyStaticBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		// ���� ������Ʈ�� ������ �´� ������Ʈ���� Ȯ���Ѵ� �ƴϸ� ����
		if (std::dynamic_pointer_cast<LegacyStaticObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		// DXObj�� ������Ʈ�� �������� �־��ش�.
		BuildGeometry(pDXObj, objectName);

		// �� �� ����.
		return pDXObj;
	}

	// ���� ������Ʈ�� �̸��� ����.. ������ �ٸ��� ���ݴϴ�.
		// �� ��쿡 ���缭 �� ���ҽ����� �������ݴϴ�.
	std::shared_ptr<MeshResources> LegacyStaticBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath)
	{
		std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();

		// ���� �̸��� �߰����ش�.
		_tmpObjRes->ObjName = objectPath;

		// ������Ʈ �̸��� ASE�� ���� ���. ASE���ҽ��� ������ݴϴ�.
		// Ȯ���ڸ� �Ǻ��մϴ�.
		std::string extension = objectPath;
		int _extensionIdx = objectPath.rfind(".");
		if (_extensionIdx != -1)
		{
			extension = objectPath.substr(_extensionIdx);
		}

		if (extension == ".noob")
		{
			// ��찡 �� ��ø�������� �ҽ��� �̿��� �ݴϴ�.
			std::ifstream ifs(objectPath, std::ios_base::binary);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
			buffer.push(boost::iostreams::zlib_decompressor());
			buffer.push(ifs);
			boost::archive::binary_iarchive ia(buffer);

			FBXBinaryData::ModelData modelData;

			ia >> modelData;

			_tmpObjRes.reset();

			// �޽ð� �������� ���ɼ��� �ֱ� ������ ���ο��� �� ������ش�.
			BuildMeshBinaryResources(DXObject, modelData);
			return nullptr;
		}
		else
		{
			_tmpObjRes.reset();
			return nullptr;
		}

		return _tmpObjRes;
	}

	std::shared_ptr<IDXObject> LegacyStaticBuilder::AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
		std::string textureName, std::wstring path, TextureMapType mapType)
	{
		std::vector<std::shared_ptr<MeshResources>> objRes = DXObj->GetMeshResources();

		if (objRes[0] == nullptr)
			return DXObj;

		// �ؽ��� ���� �߰��� ���׸���
		std::shared_ptr<RenderingMaterial> tempMaterial;

		// ���׸��� �����غ���.
			// �ش� ���� Ű������ �����ϴ� ���̱� ������
			// 0(ID �ʱ�ȭ ��)�� ���׸����� �ƴϸ� nullptr�� ���� ���̴�.
			// 0�� ���׸����̸� nullptr�� ������ ���� ���̴�.
			// ������ ���׸����� �����ص� ���¸�. �װ��� �״�� �����´�.
			// obj�� �޾Ƽ� �߰����ִ� ���̹Ƿ� �̷����� �Լ��� �����ϴ�.
		if (!objRes[0]->materialID.empty() && objRes[0]->setMaterial == true)
		{
			tempMaterial = resourceManager->Get<RenderingMaterial>(objRes[0]->materialID[0]);
		}
		// ���ٸ� ���� �־��ش�.
		else
		{
			/*objRes[0]->setMaterial = true;

			tempMaterial = std::make_shared<RenderingMaterial>();

			objRes[0]->materialID = resourceManager->Add<RenderingMaterial>(tempMaterial);*/
		}

		// �ؽ��İ� �����ϴ��� ã��
		if (resourceManager->Get<Texture>(textureID) != nullptr)
		{
			// �ִٸ� ���׸��� �־��ش�..
				// ����� �������� ������ ��ü������ Deserialize�� �ϼ��Ǹ� �ڵ����� �־��� ��
			switch (mapType)
			{
				case TextureMapType::ALBEDO:
				{
					tempMaterial->albedoMap = textureID;

					break;
				}

				case TextureMapType::NORMAL:
				{
					tempMaterial->normalMap = textureID;

					break;
				}

				case TextureMapType::METALLIC:
				{
					tempMaterial->metallicMap = textureID;

					break;
				}

				case TextureMapType::ROUGHNESS:
				{
					tempMaterial->roughnessMap = textureID;

					break;
				}

				case TextureMapType::AO:
				{
					tempMaterial->AOMap = textureID;

					break;
				}

				case TextureMapType::EMISSIVE:
				{
					tempMaterial->emissiveMap = textureID;

					break;
				}

				case TextureMapType::CUBE:
				{
					tempMaterial->cubeMap = textureID;

					break;
				}
			}

			return DXObj;
		}

		// ���� �־��ش�.
		uint64 tempID = resourceManager->Load<Texture>(textureName, path);

		switch (mapType)
		{
			case TextureMapType::ALBEDO:
			{
				tempMaterial->albedoMap = tempID;

				break;
			}

			case TextureMapType::NORMAL:
			{
				tempMaterial->normalMap = tempID;

				break;
			}

			case TextureMapType::METALLIC:
			{
				tempMaterial->metallicMap = tempID;

				break;
			}

			case TextureMapType::ROUGHNESS:
			{
				tempMaterial->roughnessMap = tempID;

				break;
			}

			case TextureMapType::AO:
			{
				tempMaterial->AOMap = tempID;

				break;
			}

			case TextureMapType::EMISSIVE:
			{
				tempMaterial->emissiveMap = tempID;

				break;
			}

			case TextureMapType::CUBE:
			{
				tempMaterial->cubeMap = tempID;

				break;
			}
		}

		return DXObj;
	}

	void LegacyStaticBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// ���ҽ� �Ŵ��� ĳ��
		resourceManager = ResourceManager::GetInstance();
	}

	void LegacyStaticBuilder::BuildMeshBinaryResources(
		std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData)
	{
		/*if (resourceManager->GetMesh(objectID) != nullptr)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			objRes->materialID = objectID;
			objRes->setMesh = true;

			DXObject->SetMeshResources(objRes);

			return;
		}*/
		// ��� �޽��� ���鼭 ���ؽ����ۿ� �ε��� ���۸� ������ݴϴ�.
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			// ���ؽ� ���� �Է�
			std::vector<VertexStruct::PBRStaticVertex> vertices;

			// ���ؽ� ���� �Է�
			for (auto& vertex : mesh.meshVertexList)
			{
				auto tempVertex = std::make_unique<VertexStruct::PBRStaticVertex>();

				tempVertex->Pos.x = vertex.position.x;
				tempVertex->Pos.y = vertex.position.y;
				tempVertex->Pos.z = vertex.position.z;

				tempVertex->Tex.x = vertex.uv.x;
				tempVertex->Tex.y = vertex.uv.y;

				tempVertex->Normal.x = vertex.normal.x;
				tempVertex->Normal.y = vertex.normal.y;
				tempVertex->Normal.z = vertex.normal.z;

				tempVertex->TangentU.x = vertex.tangent.x;
				tempVertex->TangentU.y = vertex.tangent.y;
				tempVertex->TangentU.z = vertex.tangent.z;

				tempVertex->Color.x = vertex.color.x;
				tempVertex->Color.y = vertex.color.y;
				tempVertex->Color.z = vertex.color.z;
				tempVertex->Color.w = vertex.color.w;

				vertices.push_back(*tempVertex);
			}

			objRes->meshID = resourceManager->CreateMesh(
				vertices,
				mesh.indices,
				mesh.meshName,
				D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				RasterizerState::GetSolidRS()
			);

			objRes->setMesh = true;

			objRes->setMaterial = true;

			// ���͸��� ��� �� ID�� ��ȯ
			for (auto& mat : mesh.materials)
			{
				objRes->materialID.emplace_back(resourceManager->Add<RenderingMaterial>(mat));
			}

			// node TM ������ ���⼭ �־��.
			auto nodeTM = mesh.nodeTM;

			objRes->nodeTM.m[0][0] = nodeTM.m[0][0];
			objRes->nodeTM.m[0][1] = nodeTM.m[0][1];
			objRes->nodeTM.m[0][2] = nodeTM.m[0][2];
			objRes->nodeTM.m[0][3] = nodeTM.m[0][3];

			objRes->nodeTM.m[1][0] = nodeTM.m[1][0];
			objRes->nodeTM.m[1][1] = nodeTM.m[1][1];
			objRes->nodeTM.m[1][2] = nodeTM.m[1][2];
			objRes->nodeTM.m[1][3] = nodeTM.m[1][3];

			objRes->nodeTM.m[2][0] = nodeTM.m[2][0];
			objRes->nodeTM.m[2][1] = nodeTM.m[2][1];
			objRes->nodeTM.m[2][2] = nodeTM.m[2][2];
			objRes->nodeTM.m[2][3] = nodeTM.m[2][3];

			objRes->nodeTM.m[3][0] = nodeTM.m[3][0];
			objRes->nodeTM.m[3][1] = nodeTM.m[3][1];
			objRes->nodeTM.m[3][2] = nodeTM.m[3][2];
			objRes->nodeTM.m[3][3] = nodeTM.m[3][3];

			objRes->ObjName = mesh.meshName;

			DXObject->SetMeshResources(objRes);
		}

		// ��� �޽��� ���鼭 ���ؽ����ۿ� �ε��� ���۸� ������ݴϴ�.
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			// ���ؽ� ���� �Է�
			std::vector<VertexStruct::NormalVertex> vertices;

			// ���ؽ� ���� �Է�
			for (auto& vertex : mesh.meshVertexList)
			{
				auto tempVertex = std::make_unique<VertexStruct::NormalVertex>();

				tempVertex->Pos.x = vertex.position.x;
				tempVertex->Pos.y = vertex.position.y;
				tempVertex->Pos.z = vertex.position.z;

				tempVertex->Tex.x = vertex.uv.x;
				tempVertex->Tex.y = vertex.uv.y;

				tempVertex->Normal.x = vertex.normal.x;
				tempVertex->Normal.y = vertex.normal.y;
				tempVertex->Normal.z = vertex.normal.z;

				tempVertex->TangentU.x = vertex.tangent.x;
				tempVertex->TangentU.y = vertex.tangent.y;
				tempVertex->TangentU.z = vertex.tangent.z;

				vertices.push_back(*tempVertex);
			}

			objRes->meshID = resourceManager->CreateMesh(
				vertices,
				mesh.indices,
				mesh.meshName,
				D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
				RasterizerState::GetSolidRS()
			);

			objRes->setMesh = true;

			objRes->setMaterial = true;

			// ���͸��� ��� �� ID�� ��ȯ
			for (auto& mat : mesh.materials)
			{
				objRes->materialID.emplace_back(resourceManager->Add<RenderingMaterial>(mat));
			}

			// node TM ������ ���⼭ �־��.
			auto nodeTM = mesh.nodeTM;

			objRes->nodeTM.m[0][0] = nodeTM.m[0][0];
			objRes->nodeTM.m[0][1] = nodeTM.m[0][1];
			objRes->nodeTM.m[0][2] = nodeTM.m[0][2];
			objRes->nodeTM.m[0][3] = nodeTM.m[0][3];

			objRes->nodeTM.m[1][0] = nodeTM.m[1][0];
			objRes->nodeTM.m[1][1] = nodeTM.m[1][1];
			objRes->nodeTM.m[1][2] = nodeTM.m[1][2];
			objRes->nodeTM.m[1][3] = nodeTM.m[1][3];

			objRes->nodeTM.m[2][0] = nodeTM.m[2][0];
			objRes->nodeTM.m[2][1] = nodeTM.m[2][1];
			objRes->nodeTM.m[2][2] = nodeTM.m[2][2];
			objRes->nodeTM.m[2][3] = nodeTM.m[2][3];

			objRes->nodeTM.m[3][0] = nodeTM.m[3][0];
			objRes->nodeTM.m[3][1] = nodeTM.m[3][1];
			objRes->nodeTM.m[3][2] = nodeTM.m[3][2];
			objRes->nodeTM.m[3][3] = nodeTM.m[3][3];

			objRes->ObjName = mesh.meshName;

			DXObject->SetMeshResources(objRes);
		}

		// ModelData�� ���͸��� �������� �����ͼ� ���ҽ� �Ŵ����� ����� ���͸���鿡�� ������ �־��ش�.
 		for (auto& mat : modelData.materialList)
		{
			std::shared_ptr<RenderingMaterial> resourceMaterial = resourceManager->Get<RenderingMaterial>(mat.materialName);

			// �̹� �ִ� �ؽ��Ķ�� ���ҽ��Ŵ����� �ִ� �� �ؽ����� ���̵� �����ɴϴ�. �ƴϸ� ���� ��ȯ���ݴϴ�.
			uint64 albedoMapID = ULLONG_MAX;
			uint64 normalMapID = ULLONG_MAX;
			uint64 metallicMapID = ULLONG_MAX;
			uint64 roughnessMapID = ULLONG_MAX;
			uint64 AOMapID = ULLONG_MAX;
			uint64 emissiveMapID = ULLONG_MAX;

			std::wstring name;
			name.assign(mat.albedoMap.begin(), mat.albedoMap.end());
			if (mat.albedoMap != "")
				albedoMapID = resourceManager->Load<Texture>(mat.albedoMap, L"Resources/Texture/" + name);

			name.assign(mat.normalMap.begin(), mat.normalMap.end());
			if (mat.normalMap != "")
				normalMapID = resourceManager->Load<Texture>(mat.normalMap, L"Resources/Texture/" + name);

			name.assign(mat.metallicMap.begin(), mat.metallicMap.end());
			if (mat.metallicMap != "")
				metallicMapID = resourceManager->Load<Texture>(mat.metallicMap, L"Resources/Texture/" + name);

			name.assign(mat.roughnessMap.begin(), mat.roughnessMap.end());
			if (mat.roughnessMap != "")
				roughnessMapID = resourceManager->Load<Texture>(mat.roughnessMap, L"Resources/Texture/" + name);

			name.assign(mat.AOMap.begin(), mat.AOMap.end());
			if (mat.AOMap != "")
				AOMapID = resourceManager->Load<Texture>(mat.AOMap, L"Resources/Texture/" + name);

			name.assign(mat.emissiveMap.begin(), mat.emissiveMap.end());
			if (mat.emissiveMap != "")
				emissiveMapID = resourceManager->Load<Texture>(mat.emissiveMap, L"Resources/Texture/" + name);

			resourceMaterial->albedoMap = albedoMapID;
			resourceMaterial->normalMap = normalMapID;
			resourceMaterial->metallicMap = metallicMapID;
			resourceMaterial->roughnessMap = roughnessMapID;
			resourceMaterial->AOMap = AOMapID;
			resourceMaterial->emissiveMap = emissiveMapID;

			resourceMaterial->material_Ambient.x = mat.material_Ambient.x;
			resourceMaterial->material_Ambient.y = mat.material_Ambient.y;
			resourceMaterial->material_Ambient.z = mat.material_Ambient.z;
			resourceMaterial->material_Ambient.w = mat.material_Ambient.w;

			resourceMaterial->material_Diffuse.x = mat.material_Diffuse.x;
			resourceMaterial->material_Diffuse.y = mat.material_Diffuse.y;
			resourceMaterial->material_Diffuse.z = mat.material_Diffuse.z;
			resourceMaterial->material_Diffuse.w = mat.material_Diffuse.w;

			resourceMaterial->material_Specular.x = mat.material_Specular.x;
			resourceMaterial->material_Specular.y = mat.material_Specular.y;
			resourceMaterial->material_Specular.z = mat.material_Specular.z;
			resourceMaterial->material_Specular.w = mat.material_Specular.w;

			resourceMaterial->material_Emissive.x = mat.material_Emissive.x;
			resourceMaterial->material_Emissive.y = mat.material_Emissive.y;
			resourceMaterial->material_Emissive.z = mat.material_Emissive.z;
			resourceMaterial->material_Emissive.w = mat.material_Emissive.w;

			resourceMaterial->emissionFactor = mat.emissionFactor;
			resourceMaterial->material_Reflectivity = mat.material_Reflectivity;
			resourceMaterial->material_Transparency = mat.material_Transparency;

			resourceMaterial->metallic = mat.metallic;
			resourceMaterial->roughness = mat.roughness;
		}
	}
}
