#pragma once
#include "Interface/IDXObject.h"
#include "Interface/IRenderer.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;
	class IRenderer;

	/**
	 * \brief 기본적으로 Basic32 쉐이더의 내용으로 만들어지는 Basic32 Obj 클래스
	 *
	 * 일단은 하드 코딩으로 만들어지는 Obj
	 * + 이후에 ASE 파서를 통해 만들어지는 오브젝트들 중 Node가 없는 Obj들을 대표하는 클래스. => 구 엔진의 MeshObj에 해당하는 내용을 추가해도 될 것 같다.
	 *
	 * 2022.07.11(월) Dev.BlackFoot
	 */
	class LegacyStaticObj : public IDXObject
	{
		// 멤버 변수 목록
		// 디바이스를 가지고 있자.
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		// 각종 정보가 포함된 리소스
		std::vector<std::shared_ptr<MeshResources>> meshResources;
		std::shared_ptr<ResourceManager> resourceManager;

		std::shared_ptr<IRenderer> renderer;

		// 라이팅 관련 변수들
			// 각종 조명은 렌더러에 넣어주고? 렌더러에서 가져온다.
		Material mat;

		// 클래스가 가지고 있는 구조체
			// 메인에서 받아온다.
		DirectionalLight dirLight;
		PointLight pointLight;
		SpotLight spotLight;
		XMFLOAT3 eyePosW;	// 쉐이더에서의 ViewPosition;

		// TM 행렬 => 간단한 transform 구조체로 묶을 수도 있을 것 같다.
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX proj;
		///------------- 이 정도까지.


		// 쉐이더에 던져줄 매트릭스
		struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
			SimpleMath::Matrix worldTransform;
			SimpleMath::Matrix worldInvTranspose;
		};



	public:
		// 외부에서 생성하기 때문에.. 생성자는 dll export를 시켜준다..!
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
		// Render에서 세팅
		//void SetFXVariable();

		// 텍스쳐 세팅
		//void InitTexture();
		//void CreateSRV(std::wstring path, ID3D11ShaderResourceView** srv);
	};

}