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
		// 들어온 오브젝트 확인
		if(std::dynamic_pointer_cast<SkyBox>(pDXObj) == nullptr)
			return nullptr;

		pDXObj->SetMeshResources(BuildGeometry(pDXObj, objectName));

		return pDXObj;
	}

	std::shared_ptr<MeshResources> SkyBoxBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath)
	{
		std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();

		_tmpObjRes->ObjName = objectPath;

		// 뭐가 되었건.. 스카이 박스로 만들어줍니다.
		_tmpObjRes = BuildSkyBoxResources(_tmpObjRes);

		return _tmpObjRes;
	}

	void SkyBoxBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// 리소스 매니저 바인딩
		resourceManager = ResourceManager::GetInstance();
	}

	std::shared_ptr<IDXObject> SkyBoxBuilder::AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
		std::string textureName, std::wstring path, TextureMapType mapType)
	{
		std::vector<std::shared_ptr<MeshResources>> objRes = DXObj->GetMeshResources();

		if (objRes[0] == nullptr)
			return DXObj;

		// 텍스쳐 값을 추가할 머테리얼
		std::shared_ptr<RenderingMaterial> tempMaterial;

		// 머테리얼에 접근해본다.
			// 해당 맵의 키값으로 접근하는 것이기 때문에
			// 0(ID 초기화 값)이 머테리얼이 아니면 nullptr이 나올 것이다.
			// 0이 머테리얼이면 nullptr이 나오지 않을 것이다.
			// 기존의 머테리얼을 세팅해둔 상태면. 그것을 그대로 가져온다.
			// obj를 받아서 추가해주는 것이므로 이런식의 함수가 가능하다.

		if (!objRes[0]->materialID.empty() && objRes[0]->setMaterial == true)
		{
			tempMaterial = resourceManager->Get<RenderingMaterial>(objRes[0]->materialID[0]);
		}
		// 없다면 만들어서 넣어준다.
		else
		{
			objRes[0]->setMaterial = true;

			tempMaterial = std::make_shared<RenderingMaterial>();

			// 수정~
			tempMaterial->SetName("SkyBoxMaterial");

			objRes[0]->materialID.push_back(resourceManager->Add<RenderingMaterial>(tempMaterial->GetName()));
		}

		// 텍스쳐가 존재하는지 찾기
		if (resourceManager->Get<Texture>(textureID) != nullptr)
		{
			// 있다면 머테리얼에 넣어준다..
				// 현재는 수동으로 하지만 자체포맷의 Deserialize가 완성되면 자동으로 넣어줄 것
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

		// 만들어서 넣어준다.
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
		// 만들어진 구를 바탕으로 메시를 만들어줍니다.

		_objRes->meshID = SKYBOX_MESH;

		_objRes->setMesh = true;

		// 텍스쳐 어떻게 넣음..?
		_objRes->materialID.emplace_back(resourceManager->Add<RenderingMaterial>("SkyBoxMaterial"));
		std::shared_ptr<RenderingMaterial> resourceMaterial = resourceManager->Get<RenderingMaterial>("SkyBoxMaterial");

		// 일단 하드 코딩..
		uint64 diffuseMapID = ULLONG_MAX;

		diffuseMapID = resourceManager->Load<Texture>("CubeMapDiffuse", L"Resources/Texture/snowcube1024.dds");
		resourceMaterial->cubeMap = diffuseMapID;

		_objRes->setMaterial = true;

		return _objRes;
	}
}
