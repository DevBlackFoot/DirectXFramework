#pragma once
#include <map>
#include <string>


namespace GraphicsEngineSpace
{
	/**
	 * \brief Font, Sprite Batch, DepthState, BlendState �� UI�� �׸��� ���� ���� �������� ��Ƶΰ� �ִ�
	 *  �Ŵ��� Ŭ�����Դϴ�.
	 */
	class UIUtilsManager
	{
	private:
		static std::shared_ptr<UIUtilsManager> instance;

		// COM ������ �Ժη� ����Ʈ �����ͷ� ��������..
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DepthStencilState> depthState;
		// Sprite���� ������� Blend State
		ComPtr<ID3D11BlendState> blendState;

		std::shared_ptr<SpriteBatch> spriteBatch;

		std::map<std::string, std::shared_ptr<SpriteFont>> fontMap;

		// Lim�� �׸��� �ʿ��� Texture ID
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

		// �ν��Ͻ� ���
		static std::shared_ptr<UIUtilsManager> GetInstance();

		// ���� �ʱ�ȭ �� ��� ��Ʈ ����
		void Init(ComPtr<ID3D11Device> _device, std::shared_ptr<SpriteBatch> spriteBatch, ComPtr<ID3D11DepthStencilState> depthState, ComPtr
		          <ID3D11BlendState> blendState);

		// ���ϴ� ��Ʈ ���
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