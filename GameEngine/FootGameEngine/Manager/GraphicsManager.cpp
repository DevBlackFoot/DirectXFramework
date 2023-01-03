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
	// 스태틱변수 선언
	std::shared_ptr<GraphicsManager> GraphicsManager::instance = nullptr;

	bool GraphicsManager::Init(HWND hWnd, uint32 screenWidth, uint32 screenHeight)
	{
		// 여기서 Dll을 로드해줍니다.
			// TODO : 내 구조에 맞게 재 설계.
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
			// DLL 로드 실패
			OutputDebugString(_T("Load graphics engine dll failed.\n"));
			return false;
		}

		OutputDebugString(_T("Load graphics engine dll success.\n"));

		// 초기 리사이즈를 제대로 하기위한 클라이언트 정보 얻어오기.
		RECT rect;
		GetClientRect(hWnd, &rect);
		uint32 _width = rect.right - rect.left;
		uint32 _height = rect.bottom - rect.top;

		// 렌더러를 생성해봅시다.
		renderer.reset(CreateRenderer());

		if (renderer->Initialize(hWnd, _width, _height) != true)
			return false;

		static uint32 nowWidth = _width;
		static uint32 nowHeight = _height;

		// 리사이즈 펑션을 정합니다..
		// 윈도우 매니저 불러오기.
		// TODO : 윈도우 매니저의 SetResize 함수를 통해 Resize를 구현합니다.
		// 참고로 람다식을 써봐야 합니다.

		return true;
	}

	bool GraphicsManager::LateInit()
	{
		renderer->LateInitialize();

		return true;
	}

	void GraphicsManager::Update()
	{
		// 프레임 워크를 위한 임시 코드입니다.
			// 이 부분이 원래는 어디에 있어야할 지 고민해보세요
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
