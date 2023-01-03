#pragma once

#ifdef FOOTGRAPHICSENGINE_EXPORTS
#define GRAPHICSENGINE_DECLSPEC __declspec(dllexport)
#else
#define GRAPHICSENGINE_DECLSPEC __declspec(dllimport)
#endif

namespace GraphicsEngineSpace
{
	/**
	 * \brief ImGUI�� �����ϱ� ���� �Ŵ��� Ŭ����.
	 */
	 // ���� �������� ��� �����ϵ��� DLL Export�� ���ش�.
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

		// Window ����
		void InitImplWin(HWND hwnd);
		LRESULT SetWndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
		void ShutDownImplWin();

		// DX11 ����
		void InitImplDX11(ID3D11Device* device, ID3D11DeviceContext* dc);
		void ShutDownImplDX11();

		// ���� ����..
		void Frame();
		void Render();
		void EndFrame();
		//void UpdateCamFrame();
		void OnOffImGui() { imGUIEnable = !imGUIEnable; }
		bool isImguiEnable() { return imGUIEnable; }

		// ���� imGui�� ������ �Լ�.
			// imGUI�� �Լ��� ������
		bool Begin(std::string guiTitle);
		bool SliderFloat(std::string label, float* value, float minValue, float maxValue);
		bool SliderFloat2(std::string label, float value[2], float minValue, float maxValue);
		bool SliderFloat3(std::string label, float value[3], float minValue, float maxValue);
		bool SliderFloat4(std::string label, float value[4], float minValue, float maxValue);

		// �������� �ᵵ ��������.. string�� �̿��ؼ� +�� ����ص� ������ �� ����.
		void Text(std::string text);
		// Printf()�� ����� ���� ����
		void Text(const char* text, ...);

		void End();
	};
}