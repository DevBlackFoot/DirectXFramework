#include "pch.h"

#include "EngineMain/GameEngine.h"
#include "SceneManager.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::shared_ptr<GameEngine> gameEngine = GameEngine::GetEngineInstance();
	gameEngine->Init(TEXT("CockTailPrototype"), hInstance, 1000, 800);

	// 틱들어가기 전에
	// TODO : Scene 구조를 위해서는 이 부분에서 Scene을 생성해줄 필요성이 존재합니다.
	// 적당히 만들어보세요~

	gameEngine->Tick();

	gameEngine->Release();

	return 0;
}
