#pragma once
#include "Interface/IDXObject.h"
#include "Interface/IRenderer.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;
	class IRenderer;

	/**
	 * \brief �⺻������ Basic32 ���̴��� �������� ��������� Basic32 Obj Ŭ����
	 *
	 * �ϴ��� �ϵ� �ڵ����� ��������� Obj
	 * + ���Ŀ� ASE �ļ��� ���� ��������� ������Ʈ�� �� Node�� ���� Obj���� ��ǥ�ϴ� Ŭ����. => �� ������ MeshObj�� �ش��ϴ� ������ �߰��ص� �� �� ����.
	 *
	 * 2022.07.11(��) Dev.BlackFoot
	 */
	class LegacyStaticObj : public IDXObject
	{
		// ��� ���� ���
		// ����̽��� ������ ����.
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		// ���� ������ ���Ե� ���ҽ�
		std::vector<std::shared_ptr<MeshResources>> meshResources;
		std::shared_ptr<ResourceManager> resourceManager;

		std::shared_ptr<IRenderer> renderer;

		// ������ ���� ������
			// ���� ������ �������� �־��ְ�? ���������� �����´�.
		Material mat;

		// Ŭ������ ������ �ִ� ����ü
			// ���ο��� �޾ƿ´�.
		DirectionalLight dirLight;
		PointLight pointLight;
		SpotLight spotLight;
		XMFLOAT3 eyePosW;	// ���̴������� ViewPosition;

		// TM ��� => ������ transform ����ü�� ���� ���� ���� �� ����.
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
		///------------- �� ��������.


		// ���̴��� ������ ��Ʈ����
		struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
			SimpleMath::Matrix worldTransform;
			SimpleMath::Matrix worldInvTranspose;
		};



	public:
		// �ܺο��� �����ϱ� ������.. �����ڴ� dll export�� �����ش�..!
		GRAPHICSENGINE_DECLSPEC LegacyStaticObj();
		~LegacyStaticObj() override;

		bool Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext) override;
		void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) override;
		void LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit, const XMFLOAT3& _eyePosW) override;
		void PreRender(float tick) override {}
		void Render() override;
		void SetMeshResources(std::shared_ptr<MeshResources> objResources) override;
		std::string GetObjName() override;
		std::vector<std::shared_ptr<MeshResources>> GetMeshResources() override;

		void SetRenderer(std::shared_ptr<IRenderer> renderer) override { this->renderer = renderer; }

	private:
		// Render���� ����
		//void SetFXVariable();

		// �ؽ��� ����
		//void InitTexture();
		//void CreateSRV(std::wstring path, ID3D11ShaderResourceView** srv);
	};

}