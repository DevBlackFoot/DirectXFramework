#include "GraphicsPch.h"
#include "PBRObj.h"

#include "Manager/BufferManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/SamplerManager.h"
#include "Manager/ResourceManager.h"

#include "Shader/ShaderBase.h"
#include "Buffer/BufferBase.h"
#include "Resources/Texture.h"
#include "Util/Animator.h"

#include "GraphicsEngine/Renderer.h"

namespace GraphicsEngineSpace
{
	PBRObj::PBRObj()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, resourceManager(nullptr)
		, boneResources(nullptr)
		, animResources(nullptr)
		, renderer(nullptr)
		, objAnimator(nullptr)
		, world()
		, view()
		, proj()
	{
	}

	PBRObj::~PBRObj()
	{
		for (auto objRes : meshResources)
		{
			SafeReset(objRes);
		}

		SafeReset(boneResources);

		if (animResources != nullptr)
		{
			for (auto animClip : animResources->animationClipList)
			{
				SafeReset(animClip.second);
			}
		}

		SafeReset(animResources);
	}

	bool PBRObj::Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext)
	{
		// 예외 확인
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		resourceManager = ResourceManager::GetInstance();

		// mesh, material 캐싱
		for (auto& objRes : meshResources)
		{
			objRes->mesh = resourceManager->Get<Mesh>(objRes->meshID);

			if (objRes->materialID.empty())
			{
				objRes->materialID.emplace_back(resourceManager->Add<RenderingMaterial>("Default"));

				objRes->material.emplace_back(resourceManager->Get<RenderingMaterial>(objRes->materialID[0]));

				objRes->albedo.resize(objRes->material.size());
				objRes->normal.resize(objRes->material.size());
				objRes->metallic.resize(objRes->material.size());
				objRes->roughness.resize(objRes->material.size());
				objRes->ao.resize(objRes->material.size());
				objRes->emissive.resize(objRes->material.size());
			}
			else
			{
				for (auto& matID : objRes->materialID)
				{
					objRes->material.emplace_back(resourceManager->Get<RenderingMaterial>(matID));
				}

				objRes->albedo.resize(objRes->material.size());
				objRes->normal.resize(objRes->material.size());
				objRes->metallic.resize(objRes->material.size());
				objRes->roughness.resize(objRes->material.size());
				objRes->ao.resize(objRes->material.size());
				objRes->emissive.resize(objRes->material.size());

				for (int i = 0; i < objRes->material.size(); i++)
				{
					objRes->albedo[i] = resourceManager->Get<Texture>(objRes->material[i]->albedoMap);
					objRes->normal[i] = resourceManager->Get<Texture>(objRes->material[i]->normalMap);
					objRes->metallic[i] = resourceManager->Get<Texture>(objRes->material[i]->metallicMap);
					objRes->roughness[i] = resourceManager->Get<Texture>(objRes->material[i]->roughnessMap);
					objRes->ao[i] = resourceManager->Get<Texture>(objRes->material[i]->AOMap);
					objRes->emissive[i] = resourceManager->Get<Texture>(objRes->material[i]->emissiveMap);
				}
			}
		}

		// 본 개수 만큼 생성.
		if (boneResources != nullptr && animResources != nullptr)
		{
			// 기본적인 애니메이터와 본 세팅은 여기서 해줍니다.
			objAnimator = std::make_unique<Animator>(boneResources->bones.size());
			nowBoneTM.assign(boneResources->bones.size(), SimpleMath::Matrix::Identity);
		}

		return true;
	}

	void PBRObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// 행렬 넣기
		world = _world;
		view = _view;
		proj = _proj;
	}

	void PBRObj::PreRender(float tick)
	{
		// 본트랜스폼을 바꿔준다.
			// TODO : Animation을 업데이트 해줍니다.
			// Hint : BoneResource에서 본 정보를 얻어서 Animator에서 바꿔줘야 합니다.
	}

	void PBRObj::Render()
	{
		assert(D3DDeviceContext);

		for (auto& objRes : meshResources)
		{
			// Mesh의 종류에 따라 다른 Render 함수를 부른다.
			if (objRes->isSkinnedMesh != true)
				StaticRender(objRes);
			else
				SkinnedRender(objRes);
		}
	}

	void PBRObj::SetMeshResources(std::shared_ptr<MeshResources> objResources)
	{
		meshResources.push_back(objResources);
	}

	std::string PBRObj::GetObjName()
	{
		return meshResources[0]->ObjName;
	}

	std::vector<std::shared_ptr<MeshResources>> PBRObj::GetMeshResources()
	{
		return meshResources;
	}

	void PBRObj::PlayAnim(std::string animCilpName, bool isLoop)
	{
		if (animResources != nullptr)
		{
			auto findIt = animResources->animationClipList.find(animCilpName);

			if (findIt != animResources->animationClipList.end())
				objAnimator->SetAnimationClip(findIt->second, isLoop);
		}
	}

	void PBRObj::StaticRender(const std::shared_ptr<MeshResources>& objRes)
	{
		if (renderer != nullptr)
			renderer->GraphicsDebugBeginEvent(objRes->ObjName);

		auto mesh = objRes->mesh;
		auto material = objRes->material;

		for (int matCnt = 0; matCnt < material.size(); matCnt++)
		{
			auto& albedo = objRes->albedo;
			auto& normal = objRes->normal;
			auto& metallic = objRes->metallic;
			auto& roughness = objRes->roughness;
			auto& ao = objRes->ao;
			auto& emissive = objRes->emissive;

			// 쉐이더 세팅
			ShaderManager::GetInstance()->GetShader("PBRStaticVS")->SetUpShader();

			if (albedo[matCnt] == nullptr)
			{
				// 해당 조건을 만족하는 쉐이더는 하나 밖에 없다.
				ShaderManager::GetInstance()->GetShader("PBRPS")->SetUpShader();
			}
			else
			{
				// 나머지는 모두 알베도가 있음을 가정한다.
				if (
					normal[matCnt] != nullptr &&
					metallic[matCnt] != nullptr &&
					roughness[matCnt] != nullptr &&
					ao[matCnt] != nullptr &&
					emissive[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRFullPS")->SetUpShader();
				}
				else if (
					normal[matCnt] != nullptr &&
					metallic[matCnt] != nullptr &&
					roughness[matCnt] != nullptr &&
					ao[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalMRAOPS")->SetUpShader();
				}
				else if (
					normal[matCnt] != nullptr &&
					ao[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalAOPS")->SetUpShader();
				}
				else if (
					normal[matCnt] != nullptr &&
					metallic[matCnt] != nullptr &&
					roughness[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalMetallicRoughnessPS")->SetUpShader();
				}
				else if (
					metallic[matCnt] != nullptr &&
					roughness[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRMetallicRoughnessPS")->SetUpShader();
				}
				else if (
					normal[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalPS")->SetUpShader();
				}
				else
				{
					ShaderManager::GetInstance()->GetShader("PBRAlbedoPS")->SetUpShader();
				}
			}

			D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

			UINT stride = sizeof(VertexStruct::PBRStaticVertex);
			UINT offset = 0;
			D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
			D3DDeviceContext->IASetIndexBuffer(mesh->indexBuffers[matCnt].Get(), DXGI_FORMAT_R32_UINT, 0);

			// 상수 버퍼 세팅
			cbMatrix cbPerObj;
			Matrix newWorld = objRes->nodeTM * world;
			cbPerObj.worldViewProj = newWorld * view * proj;
			cbPerObj.worldTransform = newWorld;
			cbPerObj.worldInvTranspose = MathHelper::InverseTranspose(newWorld);
			BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);

			// PBR Material
			cbMaterial cbPBRMat;
			cbPBRMat.metallic = material[matCnt]->metallic;
			cbPBRMat.roughness = material[matCnt]->roughness;

			cbPBRMat.AddColor.x = material[matCnt]->material_Diffuse.x;
			cbPBRMat.AddColor.y = material[matCnt]->material_Diffuse.y;
			cbPBRMat.AddColor.z = material[matCnt]->material_Diffuse.z;

			cbPBRMat.emissiveColor.x = material[matCnt]->material_Emissive.x;
			cbPBRMat.emissiveColor.y = material[matCnt]->material_Emissive.y;
			cbPBRMat.emissiveColor.z = material[matCnt]->material_Emissive.z;
			BufferManager::GetInstance()->GetBuffer("PBRMaterialCB")->SetUpBuffer(1, &cbPBRMat, ShaderType::VERTEX);
			BufferManager::GetInstance()->GetBuffer("PBRMaterialCB")->SetUpBuffer(0, &cbPBRMat, ShaderType::PIXEL);

			// 텍스쳐 세팅
			if (albedo[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(0, 1, albedo[matCnt]->GetMapSRV().GetAddressOf());

			if (normal[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(1, 1, normal[matCnt]->GetMapSRV().GetAddressOf());

			if (metallic[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(2, 1, metallic[matCnt]->GetMapSRV().GetAddressOf());

			if (roughness[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(3, 1, roughness[matCnt]->GetMapSRV().GetAddressOf());

			if (ao[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(4, 1, ao[matCnt]->GetMapSRV().GetAddressOf());

			if (emissive[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(5, 1, emissive[matCnt]->GetMapSRV().GetAddressOf());


			D3DDeviceContext->PSSetSamplers(0, 1, SamplerManager::GetInstance()->GetLinearClamp().GetAddressOf());

			// 렌더 스테이트 설정.
			D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

			// 해당 내용 그리기
			D3DDeviceContext->DrawIndexed(mesh->indexBuffersSize[matCnt], 0, 0);

			// 셰이더 초기화
			ID3D11ShaderResourceView* nullSRV[6] = { nullptr };

			D3DDeviceContext->PSSetShaderResources(0, 6, nullSRV);

			if (renderer != nullptr)
				renderer->GraphicsDebugEndEvent();
		}
	}

	void PBRObj::SkinnedRender(const std::shared_ptr<MeshResources>& objRes)
	{
		if (renderer != nullptr)
			renderer->GraphicsDebugBeginEvent(objRes->ObjName);

		auto mesh = objRes->mesh;
		auto material = objRes->material;

		for (int matCnt = 0; matCnt < material.size(); matCnt++)
		{
			auto& albedo = objRes->albedo;
			auto& normal = objRes->normal;
			auto& metallic = objRes->metallic;
			auto& roughness = objRes->roughness;
			auto& ao = objRes->ao;
			auto& emissive = objRes->emissive;

			// 쉐이더 세팅
			ShaderManager::GetInstance()->GetShader("PBRSkinnedVS")->SetUpShader();

			if (albedo[matCnt] == nullptr)
			{
				// 해당 조건을 만족하는 쉐이더는 하나 밖에 없다.
				ShaderManager::GetInstance()->GetShader("PBRPS")->SetUpShader();
			}
			else
			{
				// 나머지는 모두 알베도가 있음을 가정한다.
				if (
					normal[matCnt] != nullptr &&
					metallic[matCnt] != nullptr &&
					roughness[matCnt] != nullptr &&
					ao[matCnt] != nullptr &&
					emissive[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRFullPS")->SetUpShader();
				}
				else if (
					normal[matCnt] != nullptr &&
					metallic[matCnt] != nullptr &&
					roughness[matCnt] != nullptr &&
					ao[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalMRAOPS")->SetUpShader();
				}
				else if (
					normal[matCnt] != nullptr &&
					ao[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalAOPS")->SetUpShader();
				}
				else if (
					normal[matCnt] != nullptr &&
					metallic[matCnt] != nullptr &&
					roughness[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalMetallicRoughnessPS")->SetUpShader();
				}
				else if (
					metallic[matCnt] != nullptr &&
					roughness[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRMetallicRoughnessPS")->SetUpShader();
				}
				else if (
					normal[matCnt] != nullptr
					)
				{
					ShaderManager::GetInstance()->GetShader("PBRNormalPS")->SetUpShader();
				}
				else
				{
					ShaderManager::GetInstance()->GetShader("PBRAlbedoPS")->SetUpShader();
				}
			}

			D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

			UINT stride = sizeof(VertexStruct::PBRSkinnedVertex);
			UINT offset = 0;
			D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
			D3DDeviceContext->IASetIndexBuffer(mesh->indexBuffers[matCnt].Get(), DXGI_FORMAT_R32_UINT, 0);

			// 상수 버퍼 세팅
			cbMatrix cbPerObj;
			cbPerObj.worldViewProj = world * view * proj;
			cbPerObj.worldTransform = world;
			cbPerObj.worldInvTranspose = MathHelper::InverseTranspose(world);
			BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);


			// PBR Material
			cbMaterial cbPBRMat;
			cbPBRMat.metallic = material[matCnt]->metallic;
			cbPBRMat.roughness = material[matCnt]->roughness;

			cbPBRMat.AddColor.x = material[matCnt]->material_Diffuse.x;
			cbPBRMat.AddColor.y = material[matCnt]->material_Diffuse.y;
			cbPBRMat.AddColor.z = material[matCnt]->material_Diffuse.z;

			cbPBRMat.emissiveColor.x = material[matCnt]->material_Emissive.x;
			cbPBRMat.emissiveColor.y = material[matCnt]->material_Emissive.y;
			cbPBRMat.emissiveColor.z = material[matCnt]->material_Emissive.z;
			BufferManager::GetInstance()->GetBuffer("PBRMaterialCB")->SetUpBuffer(1, &cbPBRMat, ShaderType::VERTEX);
			BufferManager::GetInstance()->GetBuffer("PBRMaterialCB")->SetUpBuffer(0, &cbPBRMat, ShaderType::PIXEL);

			// Bone Matrix
				// => boneResources의 boneTransform이 애니메이션에 의해 어디선가에서 변경된다고 가정.
				// PreRender부분이 될 수도 있고, 해당 object의 Update일수도 있음.
			cbBoneMatrix cbBone;
			auto bone = nowBoneTM;


			for (int i = 0; i < bone.size(); i++)
			{
				cbBone.boneMatrix[i] = bone[i];
			}

			BufferManager::GetInstance()->GetBuffer("BoneMatrixCB")->SetUpBuffer(2, &cbBone, ShaderType::VERTEX);

			// 텍스쳐 세팅
			if (albedo[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(0, 1, albedo[matCnt]->GetMapSRV().GetAddressOf());

			if (normal[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(1, 1, normal[matCnt]->GetMapSRV().GetAddressOf());

			if (metallic[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(2, 1, metallic[matCnt]->GetMapSRV().GetAddressOf());

			if (roughness[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(3, 1, roughness[matCnt]->GetMapSRV().GetAddressOf());

			if (ao[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(4, 1, ao[matCnt]->GetMapSRV().GetAddressOf());

			if (emissive[matCnt] != nullptr)
				D3DDeviceContext->PSSetShaderResources(5, 1, emissive[matCnt]->GetMapSRV().GetAddressOf());


			D3DDeviceContext->PSSetSamplers(0, 1, SamplerManager::GetInstance()->GetLinearClamp().GetAddressOf());

			// 렌더 스테이트 설정.
			D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

			// 해당 내용 그리기
			D3DDeviceContext->DrawIndexed(mesh->indexBuffersSize[matCnt], 0, 0);

			// BoneRender.



			// 셰이더 초기화
			ID3D11ShaderResourceView* nullSRV[6] = { nullptr };

			D3DDeviceContext->PSSetShaderResources(0, 6, nullSRV);

			if (renderer != nullptr)
				renderer->GraphicsDebugEndEvent();
		}
	}
}
