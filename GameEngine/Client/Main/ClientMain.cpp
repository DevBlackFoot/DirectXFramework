#include "pch.h"

#include "EngineMain/GameEngine.h"
#include "SceneManager.h"

int APIENTRY wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	std::shared_ptr<GameEngine> gameEngine = GameEngine::GetEngineInstance();
	gameEngine->Init(TEXT("CockTailPrototype"), hInstance, 1000, 800);

	// ƽ���� ����
	// TODO : Scene ������ ���ؼ��� �� �κп��� Scene�� �������� �ʿ伺�� �����մϴ�.
	// ������ ��������~

	gameEngine->Tick();

	gameEngine->Release();

	return 0;
}
