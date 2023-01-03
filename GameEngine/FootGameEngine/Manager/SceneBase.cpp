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
		// ���� ���۽�..
		// TODO : �� �̺�Ʈ �Լ��� ȣ�� �� �� ������ �ؾ��ұ��..
	}

	void SceneBase::Start()
	{

		// TODO : �� �̺�Ʈ �Լ��� ȣ�� �� �� ������ �ؾ��ұ��..
	}

	void SceneBase::PreUpdate(float tick)
	{

		// TODO : �� �̺�Ʈ �Լ��� ȣ�� �� �� ������ �ؾ��ұ��..
	}

	void SceneBase::Update(float tick)
	{

		// TODO : �� �̺�Ʈ �Լ��� ȣ�� �� �� ������ �ؾ��ұ��..
	}

	void SceneBase::LateUpdate(float tick)
	{

		// TODO : �� �̺�Ʈ �Լ��� ȣ�� �� �� ������ �ؾ��ұ��..
	}

	void SceneBase::Render(float tick)
	{

		// TODO : �� �̺�Ʈ �Լ��� ȣ�� �� �� ������ �ؾ��ұ��..
		// �� ������Ʈ���� ������ �۾�

		// ������ � ������ ���������� ���� �ڵ尡 ���� ������������ �ֽ��ϴ�.
	}

	void SceneBase::OnEnable()
	{
		// TODO : �� �̺�Ʈ �Լ��� ȣ�� �� �� ������ �ؾ��ұ��..

	}

	void SceneBase::ClearScene()
	{
		// ���������� �����ش�.
		// ������� �ϳ��� ���� �� ���������� ������
		// TODO : �� �̺�Ʈ �Լ��� ȣ�� �� �� ������ �ؾ��ұ��..

	}

	void SceneBase::AddGameObject(std::shared_ptr<GameObject> newGameObj)
	{
		// TODO : Scene�� ���� ������Ʈ�� �������� ��� �ؾ��ұ��..
		// ��Ʈ �Ʒ��� Render obj�� Collider obj�� �ִ� �ڵ带 �����սô�.

		// �ڽĵ��� ������ �ڽĵ鵵 �־��ش�.
	}

	std::shared_ptr<GameObject> SceneBase::CreateEmpty()
	{
		// ����� ������Ʈ�� �����.. Ʈ�������� ���� �� �־��ش�.

		// TODO : �̰� �׳� ���׽��ϴ�... �ڵ��� �ǹ̸� �����ϸ� ���ƿ�..
		std::shared_ptr<GameObject> newGameObj = std::make_shared<GameObject>();

		// �⺻������ ��� ���ӿ�����Ʈ�� Transform�� �����ϴ�.
		//newGameObj->AddComponent<Transform>();

		AddGameObject(newGameObj);

		// ��ȯ.
		return newGameObj;
	}

	/*
	 void SceneBase::AddRenderer(std::shared_ptr<RendererBase> renderObj)
	{
		// ������ �׷����� ���� ������Ʈ�� ���� �����ϱ� ���� �Լ�..
		renderObjInScene.push_back(renderObj);
	}

	void SceneBase::AddCollider(std::shared_ptr<ColliderBase> colliderObj)
	{
		// ���� Collider�� ���� ������Ʈ�� ���� �����ϱ� ���� �Լ�..
		colliderObjInScene.push_back(colliderObj);
	}
	*/

}
