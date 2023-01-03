#pragma once

#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	/**
	 * \brief ImGUI를 관리하기 위한 매니저 클래스.
	 */
	 // 게임 엔진에서 사용 가능하도록 DLL Export를 해준다.
	class GRAPHICSENGINE_DECLSPEC ImGUIManager
	{
		static std::shared_ptr<ImGUIManager> instance;

		bool imGUIEnable;
		bool isFrame;

	public:
		ImGUIManager();
		~ImGUIManager();

		static std::shared_ptr<ImGUIManager> GetInstance();

		void Init();
		void Finalize();

		// Window 관련
		void InitImplWin(HWND hwnd);
		LRESULT SetWndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void ShutDownImplWin();

		// DX11 관련
		void InitImplDX11(ID3D11Device* device, ID3D11DeviceContext* dc);
		void ShutDownImplDX11();

		// 렌더 관련..
		void Frame();
		void Render();
		void EndFrame();
		//void UpdateCamFrame();
		void OnOffImGui() { imGUIEnable = !imGUIEnable; }
		bool isImguiEnable() { return imGUIEnable; }

		// 실제 imGui를 생성할 함수.
			// imGUI의 함수를 래핑함
		bool Begin(std::string guiTitle);
		bool SliderFloat(std::string label, float* value, float minValue, float maxValue);
		bool SliderFloat2(std::string label, float value[2], float minValue, float maxValue);
		bool SliderFloat3(std::string label, float value[3], float minValue, float maxValue);
		bool SliderFloat4(std::string label, float value[4], float minValue, float maxValue);

		// 포맷으로 써도 괜찮지만.. string을 이용해서 +로 사용해도 괜찮을 것 같다.
		void Text(std::string text);
		// Printf()와 비슷한 포맷 버전
		void Text(const char* text, ...);

		void End();
	};
}