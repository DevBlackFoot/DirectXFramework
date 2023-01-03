#include "GraphicsPch.h"
#include "LegacyStaticObj.h"
#include "DirectXTex.h"

#include "Manager/BufferManager.h"
#include "Manager/ShaderManager.h"
#include "Manager/SamplerManager.h"
#include "Manager/ResourceManager.h"

#include "Shader/ShaderBase.h"
#include "Buffer/BufferBase.h"
#include "Resources/Texture.h"

#include "GraphicsEngine/Renderer.h"

namespace GraphicsEngineSpace
{
	LegacyStaticObj::LegacyStaticObj()
		: D3DDevice(nullptr)
		, D3DDeviceContext(nullptr)
		, resourceManager(nullptr)
		, renderer(nullptr)
		, mat({})
		, dirLight({})
		, pointLight({})
		, spotLight({})
		, eyePosW()
		, world()
		, view()
		, proj()
	{
	}

	LegacyStaticObj::~LegacyStaticObj()
	{
		for (auto objRes : meshResources)
		{
			SafeReset(objRes);
		}
	}

	bool LegacyStaticObj::Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext)
	{
		// ���� Ȯ��
		assert(pDevice);
		assert(pDeviceContext);

		D3DDevice = pDevice;
		D3DDeviceContext = pDeviceContext;

		resourceManager = ResourceManager::GetInstance();

		// ���׸��� �ʱ�ȭ.
		mat.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		mat.Specular = XMFLOAT4(0.8f, 0.8f, 0.8f, 96.0f);

		// mesh, material ĳ��
		for (auto& objRes : meshResources)
		{
			objRes->mesh = resourceManager->Get<Mesh>(objRes->meshID);

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

		// �ʱ�ȭ �� ��. 
		return true;
	}

	void LegacyStaticObj::Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj)
	{
		// ��� �ֱ�
		world = _world;
		view = _view;
		proj = _proj;
	}

	void LegacyStaticObj::LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit,
		const XMFLOAT3& _eyePosW)
	{
		dirLight = _dirLit;
		pointLight = _pointLit;
		spotLight = _spotLit;
		eyePosW = _eyePosW;
	}

	void LegacyStaticObj::Render()
	{
		assert(D3DDeviceContext);

		// objectResources���� ��������..
		for (auto& objRes : meshResources)
		{
			renderer->GraphicsDebugBeginEvent(objRes->ObjName);

			auto mesh = objRes->mesh;
			auto material = objRes->material;

			for (int matCnt = 0; matCnt < material.size(); matCnt++)
			{
				auto& albedo = objRes->albedo;
				auto& normal = objRes->normal;

				ShaderManager::GetInstance()->GetShader("LegacyStaticVS")->SetUpShader();
				// ���̴��� �����մϴ�.
				if (albedo[matCnt] == nullptr && normal[matCnt] == nullptr)
				{
					// ���� �� �� ���� ��
					ShaderManager::GetInstance()->GetShader("LegacyStaticPS")->SetUpShader();
				}
				else if (normal[matCnt] == nullptr)
				{
					// �븻 �ʸ� ���� ��
					ShaderManager::GetInstance()->GetShader("LegacyStaticDiffusePS")->SetUpShader();
				}
				else
				{
					ShaderManager::GetInstance()->GetShader("LegacyStaticNormalPS")->SetUpShader();
				}

				D3DDeviceContext->IASetPrimitiveTopology(mesh->GetPrimitiveTopology());

				UINT stride = sizeof(VertexStruct::NormalVertex);
				UINT offset = 0;
				D3DDeviceContext->IASetVertexBuffers(0, 1, mesh->vertexBuffer.GetAddressOf(), &stride, &offset);
				D3DDeviceContext->IASetIndexBuffer(mesh->indexBuffers[matCnt].Get(), DXGI_FORMAT_R32_UINT, 0);

				// ��� ���� ����
				cbMatrix cbPerObj;
				cbPerObj.worldViewProj = objRes->nodeTM * world * view * proj;
				cbPerObj.worldTransform = world;
				cbPerObj.worldInvTranspose = MathHelper::InverseTranspose(world);
				BufferManager::GetInstance()->GetBuffer("MatrixCB")->SetUpBuffer(0, &cbPerObj, ShaderType::VERTEX);

				// �ȼ� ���̴� ��� ���� ����
				SimpleMath::Vector4 viewpos = SimpleMath::Vector4{ eyePosW.x, eyePosW.y, eyePosW.z, 0.f };
				BufferManager::GetInstance()->GetBuffer("ViewPosCB")->SetUpBuffer(0, &viewpos, ShaderType::PIXEL);

				std::shared_ptr<RenderingMaterial> resourceMaterial = resourceManager->GetInstance()->Get<RenderingMaterial>(objRes->materialID[matCnt]);

				mat.Ambient = resourceMaterial->material_Ambient;
				mat.Diffuse = resourceMaterial->material_Diffuse;
				mat.Specular = resourceMaterial->material_Specular;

				BufferManager::GetInstance()->GetBuffer("DLightCB")->SetUpBuffer(1, &dirLight, ShaderType::PIXEL);
				BufferManager::GetInstance()->GetBuffer("PLightCB")->SetUpBuffer(2, &pointLight, ShaderType::PIXEL);
				BufferManager::GetInstance()->GetBuffer("LegacyMaterialCB")->SetUpBuffer(3, &mat, ShaderType::PIXEL);

				// �ؽ��� ����
				if (albedo[matCnt] != nullptr)
					D3DDeviceContext->PSSetShaderResources(0, 1, albedo[matCnt]->GetMapSRV().GetAddressOf());

				if (normal[matCnt] != nullptr)
					D3DDeviceContext->PSSetShaderResources(1, 1, normal[matCnt]->GetMapSRV().GetAddressOf());

				// ���÷� ����. ���� �־�����..
					// �ݵ�� Ŭ������ ����
				D3DDeviceContext->PSSetSamplers(0, 1, SamplerManager::GetInstance()->GetAnisotropicWrap().GetAddressOf());

				// ���� ������Ʈ ����.
				D3DDeviceContext->RSSetState(mesh->GetRasterState().Get());

				// �ش� ���� �׸���
				D3DDeviceContext->DrawIndexed(mesh->indexBuffersSize[matCnt], 0, 0);

				// ���̴� �ʱ�ȭ
				ID3D11ShaderResourceView* nullSRV[2] = { nullptr };

				D3DDeviceContext->PSSetShaderResources(0, 2, nullSRV);

				renderer->GraphicsDebugEndEvent();
			}
		}
	}

	void LegacyStaticObj::SetMeshResources(std::shared_ptr<MeshResources> objResources)
	{
		meshResources.push_back(objResources);
	}

	std::string LegacyStaticObj::GetObjName()
	{
		return meshResources[0]->ObjName;
	}

	std::vector<std::shared_ptr<MeshResources>> LegacyStaticObj::GetMeshResources()
	{
		return meshResources;
	}


	// ���� ������ �ؽ��� ���� �� SRV ���� �ڵ�..
		// ���� �ߺ��� �ؽ��� ������ ���� ���� Resource�Ŵ��� ������ ���� �ʿ�.
	/*void LegacyStaticObj::CreateSRV(std::wstring path, ID3D11ShaderResourceView** srv)
	{
		ScratchImage image;

		// ���⼭ ����� �����. ���Ŀ� ���� Ŭ������ �Ĵ°� ���� �� �ϴ�.
		std::wstring ext = path;
		int _extensionIdx = ext.rfind(L".");
		if (_extensionIdx != -1)
		{
			ext = path.substr(_extensionIdx);
		}

		if (ext == L".dds" || ext == L".DDS")
		{
			::LoadFromDDSFile(path.c_str(), DDS_FLAGS_NONE, nullptr, image);
		}
		else // ���� Wiic ����
		{
			::LoadFromWICFile(path.c_str(), WIC_FLAGS_NONE, nullptr, image);
		}

		ScratchImage mipChain;

		HRESULT hr;

		ID3D11Texture2D* texture2D;

		hr = ::CreateTexture(D3DDevice, image.GetImages(), image.GetImageCount(), image.GetMetadata(),
			reinterpret_cast<ID3D11Resource**>(&texture2D));

		if (FAILED(hr))
			::MessageBoxA(nullptr, "Texture Create Failed!", nullptr, MB_OK);

		// desc�� ���� �־��ֱ�.
		D3D11_TEXTURE2D_DESC textureDesc;
		texture2D->GetDesc(&textureDesc);

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;

		ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

		srvDesc.Format = textureDesc.Format;
		srvDesc.Texture2D.MipLevels = textureDesc.MipLevels;
		srvDesc.Texture2D.MostDetailedMip = 0;

		srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;

		hr = D3DDevice->CreateShaderResourceView(texture2D, &srvDesc, srv);

		ReleaseCOM(texture2D);
	}*/
}
