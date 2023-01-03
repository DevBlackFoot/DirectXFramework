#include "GraphicsPch.h"
#include "PBRBuilder.h"

#include "Object/PBRObj.h"
#include "GraphicsCore/RasterizerState.h"

#include "Manager/ResourceManager.h"
#include <typeinfo>

namespace GraphicsEngineSpace
{
	PBRBuilder::~PBRBuilder()
	{
	}

	std::shared_ptr<IDXObject> PBRBuilder::BuildDXObject(std::shared_ptr<IDXObject> pDXObj, std::string objectName)
	{
		// 들어온 오브젝트 확인
		if (std::dynamic_pointer_cast<PBRObj>(pDXObj) == nullptr)
		{
			OutputDebugString(L"Not PB OBJ");

			return nullptr;
		}

		if (ResourceManager::GetInstance()->GetPrefab(objectName) != nullptr)
			pDXObj = ResourceManager::GetInstance()->GetPrefab(objectName);
		else if (ResourceManager::GetInstance()->Get<Mesh>(objectName) != nullptr)
		{
			std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();

			auto mesh = ResourceManager::GetInstance()->Get<Mesh>(objectName);

			_tmpObjRes->ObjName = mesh->GetName();
			_tmpObjRes->meshID = mesh->GetID();

			pDXObj->SetMeshResources(_tmpObjRes);
		}
		else
			// DXObj에 지오메트리 정보들을 넣어줍니다.
			BuildGeometry(pDXObj, objectName);

		return pDXObj;
	}

	std::shared_ptr<MeshResources> PBRBuilder::BuildGeometry(std::shared_ptr<IDXObject> DXObject,
		std::string objectPath)
	{
		std::shared_ptr<MeshResources> _tmpObjRes = std::make_shared<MeshResources>();

		_tmpObjRes->ObjName = objectPath;

		// 오브젝트 이름을 판별합니다.
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

			// 애니메이션이 없으면, 정적 메시 생성
			if (modelData.isSkinnedAnimation != true)
			{
				// 메시가 여러개일 가능성도 있기 때문에 내부에서 다 만들어준다.
				BuildStaticMeshBinaryResources(DXObject, modelData);
				return nullptr;
			}
			// 있다면 skinned Mesh 생성
			else
			{
				BuildSkinnedMeshBinaryResources(DXObject, modelData);
				return nullptr;
			}
		}
		else
		{
			_tmpObjRes.reset();
			return nullptr;
		}
	}

	std::shared_ptr<IDXObject> PBRBuilder::AddTexture(std::shared_ptr<IDXObject> DXObj, uint64 textureID,
		std::string textureName, std::wstring path, TextureMapType mapType)
	{
		// 들어온 것 그대로 리턴.
		return DXObj;
	}

	void PBRBuilder::InitBuilder(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDC)
	{
		D3DDevice = pDevice;
		D3DDeviceContext = pDC;

		// 리소스 매니저 캐싱
		resourceManager = ResourceManager::GetInstance();
	}

	void PBRBuilder::BuildStaticMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData)
	{
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

			resourceMaterial->material_Reflectivity = mat.material_Reflectivity;
			resourceMaterial->material_Transparency = mat.material_Transparency;

			resourceMaterial->metallic = mat.metallic;
			resourceMaterial->roughness = mat.roughness;
		}
	}

	void PBRBuilder::BuildSkinnedMeshBinaryResources(std::shared_ptr<IDXObject> DXObject, FBXBinaryData::ModelData modelData)
	{
		// 모든 메쉬를 돌면서 버텍스버퍼와 인덱스 버퍼를 만들어줍니다.
		for (auto& mesh : modelData.meshInfoList)
		{
			std::shared_ptr<MeshResources> objRes = std::make_shared<MeshResources>();

			objRes->isSkinnedMesh = true;

			// 버텍스 정보 입력
			std::vector<VertexStruct::PBRSkinnedVertex> vertices;

			// 버텍스 정보 입력
			for (auto& vertex : mesh.meshVertexList)
			{
				auto tempVertex = std::make_unique<VertexStruct::PBRSkinnedVertex>();

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

				tempVertex->Weight.x = vertex.weights[0];
				tempVertex->Weight.y = vertex.weights[1];
				tempVertex->Weight.z = vertex.weights[2];
				tempVertex->Weight.w = vertex.weights[3];

				tempVertex->Weights.x = vertex.weights[4];
				tempVertex->Weights.y = vertex.weights[5];
				tempVertex->Weights.z = vertex.weights[6];
				tempVertex->Weights.w = vertex.weights[7];

				tempVertex->BoneIndex.x = vertex.boneIndices[0];
				tempVertex->BoneIndex.y = vertex.boneIndices[1];
				tempVertex->BoneIndex.z = vertex.boneIndices[2];
				tempVertex->BoneIndex.w = vertex.boneIndices[3];

				tempVertex->BoneIndices.x = vertex.boneIndices[4];
				tempVertex->BoneIndices.y = vertex.boneIndices[5];
				tempVertex->BoneIndices.z = vertex.boneIndices[6];
				tempVertex->BoneIndices.w = vertex.boneIndices[7];


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

			resourceMaterial->material_Reflectivity = mat.material_Reflectivity;
			resourceMaterial->material_Transparency = mat.material_Transparency;

			resourceMaterial->metallic = mat.metallic;
			resourceMaterial->roughness = mat.roughness;
		}

		// skinned 기 때문에 bone이 존재
		std::shared_ptr<BoneResources> boneRes = std::make_shared<BoneResources>();

		for (size_t i = 0; i < modelData.boneInfoList.size(); i++)
		{
			SimpleMath::Matrix boneOffset;
			SimpleMath::Matrix node;

			auto& bone = modelData.boneInfoList[i];

			boneOffset.m[0][0] = bone.offsetMatrix.m[0][0];
			boneOffset.m[0][1] = bone.offsetMatrix.m[0][1];
			boneOffset.m[0][2] = bone.offsetMatrix.m[0][2];
			boneOffset.m[0][3] = bone.offsetMatrix.m[0][3];

			boneOffset.m[1][0] = bone.offsetMatrix.m[1][0];
			boneOffset.m[1][1] = bone.offsetMatrix.m[1][1];
			boneOffset.m[1][2] = bone.offsetMatrix.m[1][2];
			boneOffset.m[1][3] = bone.offsetMatrix.m[1][3];

			boneOffset.m[2][0] = bone.offsetMatrix.m[2][0];
			boneOffset.m[2][1] = bone.offsetMatrix.m[2][1];
			boneOffset.m[2][2] = bone.offsetMatrix.m[2][2];
			boneOffset.m[2][3] = bone.offsetMatrix.m[2][3];

			boneOffset.m[3][0] = bone.offsetMatrix.m[3][0];
			boneOffset.m[3][1] = bone.offsetMatrix.m[3][1];
			boneOffset.m[3][2] = bone.offsetMatrix.m[3][2];
			boneOffset.m[3][3] = bone.offsetMatrix.m[3][3];

			node.m[0][0] = bone.nodeMatrix.m[0][0];
			node.m[0][1] = bone.nodeMatrix.m[0][1];
			node.m[0][2] = bone.nodeMatrix.m[0][2];
			node.m[0][3] = bone.nodeMatrix.m[0][3];

			node.m[1][0] = bone.nodeMatrix.m[1][0];
			node.m[1][1] = bone.nodeMatrix.m[1][1];
			node.m[1][2] = bone.nodeMatrix.m[1][2];
			node.m[1][3] = bone.nodeMatrix.m[1][3];

			node.m[2][0] = bone.nodeMatrix.m[2][0];
			node.m[2][1] = bone.nodeMatrix.m[2][1];
			node.m[2][2] = bone.nodeMatrix.m[2][2];
			node.m[2][3] = bone.nodeMatrix.m[2][3];

			node.m[3][0] = bone.nodeMatrix.m[3][0];
			node.m[3][1] = bone.nodeMatrix.m[3][1];
			node.m[3][2] = bone.nodeMatrix.m[3][2];
			node.m[3][3] = bone.nodeMatrix.m[3][3];
			

			// 초기 월드 값으로 바꿔준다.
			//boneOffset = boneOffset * node;

			// 해당 본의 offest, node, parentIdx를 넣어준다
			Bone tmpBone;
			tmpBone.offsetTransform = boneOffset;
			tmpBone.nodeTransform = node;
			if(i == 0)
				tmpBone.parentIdx = -1;
			else
				tmpBone.parentIdx = bone.parentIndex;

			boneRes->bones.emplace_back(tmpBone);
		}

		// 모든 본을 넣어준 뒤 해당 resource를 PBRObj에 넣어준다.
		DXObject->SetBoneResource(boneRes);

		// Animation 세팅
		std::shared_ptr<AnimationResources> animRes = std::make_shared<AnimationResources>();

		// 모델 데이터의 모든 Animation Clip을 넣는다..
		for (auto& animClip : modelData.animationClipList)
		{
			// 들어갈 AnimClip 생성
			std::shared_ptr<AnimationClipResource> animClipRes = std::make_shared<AnimationClipResource>();

			// 내부의 내용을 채워준다.
			animClipRes->animationName = animClip.animationName;
			animClipRes->frameRate = animClip.frameRate;
			animClipRes->startKeyFrame = animClip.startKeyFrame;
			animClipRes->totalKeyFrame = animClip.totalKeyFrame;
			animClipRes->endKeyFrame = animClip.endKeyFrame;
			animClipRes->tickPerFrame = animClip.tickPerFrame;

			// 각각의 KeyFrame을 추가해준다.
				// 각각의 Bone이 가지고 있는 모든 Keyframe 데이터를 넣어준다.
			for (size_t i = 0; i < modelData.boneInfoList.size(); i++)
			{
				std::vector<KeyFrame> boneKeyFrames;

				for (auto& keyFrame : animClip.keyFrameList[i])
				{
					KeyFrame keyFrameRes;
					
					keyFrameRes.framePos.x = keyFrame.localTransform.x;
					keyFrameRes.framePos.y = keyFrame.localTransform.y;
					keyFrameRes.framePos.z = keyFrame.localTransform.z;

					keyFrameRes.frameRot.x = keyFrame.localRotation.x;
					keyFrameRes.frameRot.y = keyFrame.localRotation.y;
					keyFrameRes.frameRot.z = keyFrame.localRotation.z;
					keyFrameRes.frameRot.w = keyFrame.localRotation.w;

					keyFrameRes.frameScale.x = keyFrame.localScale.x;
					keyFrameRes.frameScale.y = keyFrame.localScale.y;
					keyFrameRes.frameScale.z = keyFrame.localScale.z;

					keyFrameRes.time = keyFrame.time;

					// 각각의 keyFrame 목록을 벡터로 만들어 주고
					boneKeyFrames.emplace_back(keyFrameRes);
				}

				// 모든 keyFrame이 벡터로 들어갔을때. 해당 벡터를 넣어준다.
				animClipRes->keyFrames.emplace_back(boneKeyFrames);
			}

			animRes->animationClipList.insert(std::make_pair(animClipRes->animationName, animClipRes));
		}

		// 모든 애니메이션을 세팅한 뒤 해당 리소스를 PBRObj에 넣어준다.
		DXObject->SetAnimationResource(animRes);
	}
}

