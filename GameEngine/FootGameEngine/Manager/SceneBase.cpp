#include "GamePch.h"
#include "SceneBase.h"

#include "Object/GameObject.h"
#include "GraphicsManager.h"

#include "Timer.h"

#include "InputManager.h"
#include "GraphicsManager.h"


namespace GameEngineSpace
{
#ifdef _DEBUG
	bool SceneBase::debugRender = true;
#else
	bool SceneBase::debugRender = false;
#endif

	SceneBase::SceneBase(tstring sceneName) :
		sceneName(sceneName)
		, mainCamera(nullptr)
		, gameObjectInScene(std::vector<std::shared_ptr<GameObject>>())
		, renderObjInScene(std::vector<std::shared_ptr<RendererBase>>())
		, colliderObjInScene(std::vector<std::shared_ptr<ColliderBase>>())
	{

	}

	SceneBase::~SceneBase()
	{
	}

	void SceneBase::Awake()
	{
		// 최초 시작시..
		// TODO : 각 이벤트 함수가 호출 될 때 무엇을 해야할까요..
	}

	void SceneBase::Start()
	{

		// TODO : 각 이벤트 함수가 호출 될 때 무엇을 해야할까요..
	}

	void SceneBase::PreUpdate(float tick)
	{

		// TODO : 각 이벤트 함수가 호출 될 때 무엇을 해야할까요..
	}

	void SceneBase::Update(float tick)
	{

		// TODO : 각 이벤트 함수가 호출 될 때 무엇을 해야할까요..
	}

	void SceneBase::LateUpdate(float tick)
	{

		// TODO : 각 이벤트 함수가 호출 될 때 무엇을 해야할까요..
	}

	void SceneBase::Render(float tick)
	{

		// TODO : 각 이벤트 함수가 호출 될 때 무엇을 해야할까요..
		// 각 오브젝트들의 렌더전 작업

		// 렌더는 어떤 순서로 렌더할지에 따라서 코드가 조금 복잡해질수도 있습니다.
	}

	void SceneBase::OnEnable()
	{
		// TODO : 각 이벤트 함수가 호출 될 때 무엇을 해야할까요..

	}

	void SceneBase::ClearScene()
	{
		// 내부적으로 지워준다.
		// 순서대로 하나씩 리셋 후 마지막으로 릴리즈
		// TODO : 각 이벤트 함수가 호출 될 때 무엇을 해야할까요..

	}

	void SceneBase::AddGameObject(std::shared_ptr<GameObject> newGameObj)
	{
		// TODO : Scene에 게임 오브젝트를 넣으려면 어떻게 해야할까요..
		// 힌트 아래의 Render obj와 Collider obj를 넣는 코드를 참고합시다.

		// 자식들이 있으면 자식들도 넣어준다.
	}

	std::shared_ptr<GameObject> SceneBase::CreateEmpty()
	{
		// 빈게임 오브젝트를 만들고.. 트랜스폼을 붙인 뒤 넣어준다.

		// TODO : 이건 그냥 나뒀습니다... 코드의 의미를 이해하면 좋아요..
		std::shared_ptr<GameObject> newGameObj = std::make_shared<GameObject>();

		// 기본적으로 모든 게임오브젝트는 Transform을 가집니다.
		//newGameObj->AddComponent<Transform>();

		AddGameObject(newGameObj);

		// 반환.
		return newGameObj;
	}

	/*
	 void SceneBase::AddRenderer(std::shared_ptr<RendererBase> renderObj)
	{
		// 씬에서 그려지는 게임 오브젝트를 따로 관리하기 위한 함수..
		renderObjInScene.push_back(renderObj);
	}

	void SceneBase::AddCollider(std::shared_ptr<ColliderBase> colliderObj)
	{
		// 씬의 Collider를 게임 오브젝트를 따로 관리하기 위한 함수..
		colliderObjInScene.push_back(colliderObj);
	}
	*/

}
