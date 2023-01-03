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
		// 들어온 오브젝트가 빌더에 맞는 오브젝트인지 확인한다 아니면 리턴
		if (std::dynamic_pointer_cast<LegacyStaticObj>(pDXObj) == nullptr)
		{
			return nullptr;
		}

		// DXObj에 지오메트리 정보들을 넣어준다.
		BuildGeometry(pDXObj, objectName);

		// 그 뒤 리턴.
		return pDXObj;
	}

	// 들어온 오브젝트의 이름에 따라.. 세팅을 다르게 해줍니다.
		// 각 경우에 맞춰서 새 리소스들을 설정해줍니다.
	std::shared_ptr<MeshResources> LegacyStaticBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject, std::string objectPath)
	{
		std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();

		// 또한 이름을 추가해준다.
		_tmpObjRes->ObjName = objectPath;

		// 오브젝트 이름이 ASE로 끝날 경우. ASE리소스를 만들어줍니다.
		// 확장자를 판별합니다.
		std::string extension = objectPath;
		int _extensionIdx = objectPath.rfind(".");
		if (_extensionIdx != -1)
		{
			extension = objectPath.substr(_extensionIdx);
		}

		if (extension == ".noob")
		{
			// 재우가 준 디시리얼라이즈 소스를 이용해 줍니다.
			std::ifstream ifs(objectPath, std::ios_base::binary);
			boost::iostreams::filtering_streambuf<boost::iostreams::input> buffer;
			buffer.push(boost::iostreams::zlib_decompressor());
			buffer.push(ifs);
			boost::archive::binary_iarchive ia(buffer);

			FBXBinaryData::ModelData modelData;

			ia >> modelData;

			_tmpObjRes.reset();

			// 메시가 여러개일 가능성도 있기 때문에 내부에서 다 만들어준다.
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
			/*objRes[0]->setMaterial = true;

			tempMaterial = std::make_shared<RenderingMaterial>();

			objRes[0]->materialID = resourceManager->Add<RenderingMaterial>(tempMaterial);*/
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

	void LegacyStaticBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// 리소스 매니저 캐싱
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
		// 모든 메쉬를 돌면서 버텍스버퍼와 인덱스 버퍼를 만들어줍니다.
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			// 버텍스 정보 입력
			std::vector<VertexStruct::PBRStaticVertex> vertices;

			// 버텍스 정보 입력
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

			// 머터리얼 등록 및 ID값 반환
			for (auto& mat : mesh.materials)
			{
				objRes->materialID.emplace_back(resourceManager->Add<RenderingMaterial>(mat));
			}

			// node TM 정보를 여기서 넣어본다.
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

		// 모든 메쉬를 돌면서 버텍스버퍼와 인덱스 버퍼를 만들어줍니다.
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			// 버텍스 정보 입력
			std::vector<VertexStruct::NormalVertex> vertices;

			// 버텍스 정보 입력
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

			// 머터리얼 등록 및 ID값 반환
			for (auto& mat : mesh.materials)
			{
				objRes->materialID.emplace_back(resourceManager->Add<RenderingMaterial>(mat));
			}

			// node TM 정보를 여기서 넣어본다.
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

		// ModelData의 머터리얼 정보들을 가져와서 리소스 매니저에 등록한 머터리얼들에게 정보를 넣어준다.
 		for (auto& mat : modelData.materialList)
		{
			std::shared_ptr<RenderingMaterial> resourceMaterial = resourceManager->Get<RenderingMaterial>(mat.materialName);

			// 이미 있는 텍스쳐라면 리소스매니저에 있던 그 텍스쳐의 아이디를 가져옵니다. 아니면 만들어서 반환해줍니다.
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
