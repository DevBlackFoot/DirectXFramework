#pragma once
#include "Interface/IDXObject.h"
#include "Interface/IRenderer.h"
#include "Resources/ObjectResources.h"

namespace GraphicsEngineSpace
{
	class ResourceManager;
	class Animator;
	//class IRenderer;

	// 모든 객체가 가지고 있지 않아도 되는 bone struct는 여기로 빼준다
		// Skinned mesh만 가지고 있으면 되기 때문
	__declspec(align(16)) struct cbBoneMatrix
	{
		SimpleMath::Matrix boneMatrix[96];
	};

	/**
	 * \brief PBR모델을 화면에 그리는 PBR Obj
	 */
	class PBRObj : public IDXObject
	{
		// 멤버 변수 목록
		ComPtr<ID3D11Device> D3DDevice;
		ComPtr<ID3D11DeviceContext> D3DDeviceContext;

		// 각종 정보가 포함된 리소스
			// Mesh가 여러개 존재할 수 있다.
		std::vector<std::shared_ptr<MeshResources>> meshResources;
		std::shared_ptr<BoneResources> boneResources;
		std::shared_ptr<AnimationResources> animResources;

		// ResourceManager 캐싱 용도
		std::shared_ptr<ResourceManager> resourceManager;
		// Render Debug 용도.
		std::shared_ptr<IRenderer> renderer;
		std::unique_ptr<Animator> objAnimator;

		std::vector<SimpleMath::Matrix> nowBoneTM;

		// 라이팅 관련 각종 변수들 => Deferred를 위해 라이팅 Pass로 옮긴다.

		// TM 행렬
		SimpleMath::Matrix world;
		SimpleMath::Matrix view;
		SimpleMath::Matrix proj;

		// 쉐이더에 전달할 매트릭스
		__declspec(align(16)) struct cbMatrix
		{
			SimpleMath::Matrix worldViewProj;
			SimpleMath::Matrix worldTransform;
			SimpleMath::Matrix worldInvTranspose;
		};

		// PBR Material;
		__declspec(align(16)) struct cbMaterial
		{
			float metallic;
			float roughness;
			SimpleMath::Vector2 pad1;

			SimpleMath::Vector3 AddColor;
			float pad2;

			SimpleMath::Vector3 emissiveColor;
			float pad3;

			BOOL isLight = true;
		};


	public:
		GRAPHICSENGINE_DECLSPEC PBRObj();
		~PBRObj() override;

		virtual bool Init(ComPtr<ID3D11Device> pDevice, ComPtr<ID3D11DeviceContext> pDeviceContext) override;
		virtual void Update(const XMMATRIX& _world, const XMMATRIX& _view, const XMMATRIX& _proj) override;

		// deferred pass로 옮길 것이기 때문에 내부적으로 구현을 하지 않음
		virtual void LightUpdate(const DirectionalLight& _dirLit, const PointLight& _pointLit, const SpotLight& _spotLit, const XMFLOAT3& _eyePosW) override {}
		virtual void PreRender(float tick) override;
		virtual void Render() override;
		virtual void SetMeshResources(std::shared_ptr<MeshResources> objResources) override;
		virtual void SetBoneResource(std::shared_ptr<BoneResources> boneResources) override { this->boneResources = boneResources; }
		virtual void SetAnimationResource(std::shared_ptr<AnimationResources> animResources) override { this->animResources = animResources; }

		virtual std::string GetObjName() override;
		virtual std::vector<std::shared_ptr<MeshResources>> GetMeshResources() override;
		virtual std::shared_ptr<AnimationResources> GetAnimationResources() override { return animResources; }

		virtual void SetRenderer(std::shared_ptr<IRenderer> renderer) override { this->renderer = renderer; }

		virtual void PlayAnim(std::string animCilpName, bool isLoop) override;

	private:
		void StaticRender(const std::shared_ptr<MeshResources>& objRes);
		void SkinnedRender(const std::shared_ptr<MeshResources>& objRes);
	};
}