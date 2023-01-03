#include "GraphicsPch.h"
#include "SkyBoxBuilder.h"

#include "Interface/IDXObject.h"
#include "Object/SkyBox.h"
#include "GraphicsCore/RasterizerState.h"
#include "Manager/ResourceManager.h"

using namespace DirectX::SimpleMath;

namespace GraphicsEngineSpace
{
	std::shared_ptr<IDXObject> SkyBoxBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		// ���� ������Ʈ Ȯ��
		if(std::dynamic_pointer_cast<SkyBox>(pDXObj) == nullptr)
			return nullptr;

		pDXObj->SetMeshResources(BuildGeometry(pDXObj, objectName));

		return pDXObj;
	}

	std::shared_ptr<MeshResources> SkyBoxBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath)
	{
		std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();

		_tmpObjRes->ObjName = objectPath;

		// ���� �Ǿ���.. ��ī�� �ڽ��� ������ݴϴ�.
		_tmpObjRes = BuildSkyBoxResources(_tmpObjRes);

		return _tmpObjRes;
	}

	void SkyBoxBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// ���ҽ� �Ŵ��� ���ε�
		resourceManager = ResourceManager::GetInstance();
	}

	std::shared_ptr<IDXObject> SkyBoxBuilder::AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
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
			objRes[0]->setMaterial = true;

			tempMaterial = std::make_shared<RenderingMaterial>();

			// ����~
			tempMaterial->SetName("SkyBoxMaterial");

			objRes[0]->materialID.push_back(resourceManager->Add<RenderingMaterial>(tempMaterial->GetName()));
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

	std::shared_ptr<MeshResources> SkyBoxBuilder::BuildSkyBoxResources(std::shared_ptr<MeshResources> _objRes)
	{
		// ������� ���� �������� �޽ø� ������ݴϴ�.

		_objRes->meshID = SKYBOX_MESH;

		_objRes->setMesh = true;

		// �ؽ��� ��� ����..?
		_objRes->materialID.emplace_back(resourceManager->Add<RenderingMaterial>("SkyBoxMaterial"));
		std::shared_ptr<RenderingMaterial> resourceMaterial = resourceManager->Get<RenderingMaterial>("SkyBoxMaterial");

		// �ϴ� �ϵ� �ڵ�..
		uint64 diffuseMapID = ULLONG_MAX;

		diffuseMapID = resourceManager->Load<Texture>("CubeMapDiffuse", L"Resources/Texture/snowcube1024.dds");
		resourceMaterial->cubeMap = diffuseMapID;

		_objRes->setMaterial = true;

		return _objRes;
	}
}
