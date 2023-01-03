#pragma once
#include <map>
#include <string>


namespace GraphicsEngineSpace
{
	/**
	 * \brief Font, Sprite Batch, DepthState, BlendState 등 UI를 그리기 위한 각종 정보들을 모아두고 있는
	 *  매니저 클래스입니다.
	 */
	class UIUtilsManager
	{
	private:
		static std::shared_ptr<UIUtilsManager> instance;

		// COM 변수는 함부로 스마트 포인터로 묶지말자..
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DepthStencilState> depthState;
		// Sprite에서 사용해줄 Blend State
		ComPtr<ID3D11BlendState> blendState;

		std::shared_ptr<SpriteBatch> spriteBatch;

		std::map<std::string, std::shared_ptr<SpriteFont>> fontMap;

		// Lim을 그릴때 필요한 Texture ID
		uint64 limTexID;

	public:
		UIUtilsManager()
			: device(nullptr)
			, depthState(nullptr)
			, blendState(nullptr)
			, spriteBatch(nullptr)
			, limTexID(ULLONG_MAX)
		{}
		~UIUtilsManager();

		// 인스턴스 얻기
		static std::shared_ptr<UIUtilsManager> GetInstance();

		// 내부 초기화 및 모든 폰트 생성
		void Init(ComPtr<ID3D11Device> _device, std::shared_ptr<SpriteBatch> spriteBatch, ComPtr<ID3D11DepthStencilState> depthState, ComPtr
		          <ID3D11BlendState> blendState);

		// 원하는 폰트 얻기
		std::shared_ptr<SpriteFont> GetFont(const std::string& fontName);
		std::shared_ptr<SpriteBatch> GetSpriteBatch() { return spriteBatch; }
		ComPtr<ID3D11DepthStencilState> GetDepthState() { return depthState; }
		ComPtr<ID3D11BlendState> GetBlendState() { return blendState; }
		uint64 GetLimTextureID() { return limTexID; }

		void Finalize();

	private:
		void CreateSpriteFont(const std::string& name, const std::string& path);
	};
}