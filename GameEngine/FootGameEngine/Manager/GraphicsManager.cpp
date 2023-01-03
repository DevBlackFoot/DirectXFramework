#include "GamePch.h"
#include "GraphicsManager.h"
#include "WindowManager.h"
#include "InputManager.h"
#include "../../FootGraphicsEngine/Manager/ResourceManager.h"

#include "SceneManager.h"
#include "Object/GameObject.h"
#include "SceneBase.h"

namespace GameEngineSpace
{
	// ����ƽ���� ����
	std::shared_ptr<GraphicsManager> GraphicsManager::instance = nullptr;

	bool GraphicsManager::Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		// ���⼭ Dll�� �ε����ݴϴ�.
			// TODO : �� ������ �°� �� ����.
#ifdef x64
#ifdef _DEBUG
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Debug_x64.dll"));
#else
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Release_x64.dll"));
#endif
#else
#ifdef _DEBUG
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Debug_x86.dll"));
#else
		graphicsDLL = LoadLibrary(_T("DLL/FootGraphicsEngine_Release_x86.dll"));
#endif
#endif

		if (graphicsDLL == nullptr)
		{
			// TO DO
			// DLL �ε� ����
			OutputDebugString(_T("Load graphics engine dll failed.\n"));
			return false;
		}

		OutputDebugString(_T("Load graphics engine dll success.\n"));

		// �ʱ� ������� ����� �ϱ����� Ŭ���̾�Ʈ ���� ������.
		RECT rect;
		GetClientRect(hWnd, &rect);
		uint32 _width = rect.right - rect.left;
		uint32 _height = rect.bottom - rect.top;

		// �������� �����غ��ô�.
		renderer.reset(CreateRenderer());

		if (renderer->Initialize(hWnd, _width, _height) != true)
			return false;

		static uint32 nowWidth = _width;
		static uint32 nowHeight = _height;

		// �������� ����� ���մϴ�..
		// ������ �Ŵ��� �ҷ�����.
		// TODO : ������ �Ŵ����� SetResize �Լ��� ���� Resize�� �����մϴ�.
		// ����� ���ٽ��� ����� �մϴ�.

		return true;
	}

	bool GraphicsManager::LateInit()
	{
		renderer->LateInitialize();

		return true;
	}

	void GraphicsManager::Update()
	{
		// ������ ��ũ�� ���� �ӽ� �ڵ��Դϴ�.
			// �� �κ��� ������ ��� �־���� �� ����غ�����
		renderer->BeginRender();
		renderer->Render();
		renderer->EndRender();

	}

	void GraphicsManager::Release()
	{
		if (renderer != nullptr)
			renderer->Finalize();
		renderer.reset();

		if (graphicsDLL != nullptr)
			FreeLibrary(graphicsDLL);
	}

	void GraphicsManager::PassDirectionalLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, SimpleMath::Matrix lightViewProj)
	{
		renderer->PassDirectionalLight(color, direction, power, lightViewProj);
	}

	void GraphicsManager::PassPointLight(SimpleMath::Vector3 color, SimpleMath::Vector3 position, float power, float range, SimpleMath::Matrix lightViewProj)
	{
		renderer->PassPointLight(color, position, power, range, lightViewProj);
	}

	void GraphicsManager::PassSpotLight(SimpleMath::Vector3 color, SimpleMath::Vector3 direction, float power, float halfAngle, float range, SimpleMath::Matrix lightViewProj)
	{
		renderer->PassSpotLight(color, direction, power, halfAngle, range, lightViewProj);
	}

	std::shared_ptr<GraphicsManager> GraphicsManager::GetInstance()
	{
		if (instance == nullptr)
			instance = std::make_shared<GraphicsManager>();

		return instance;
	}
}
