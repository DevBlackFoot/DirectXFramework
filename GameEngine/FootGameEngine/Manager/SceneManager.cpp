#include "GamePch.h"
#include "SceneManager.h"

#include "SceneBase.h"
#include "Object/GameObject.h"
#include "PhysicsManager.h"
#include "Timer.h"

#include "InputManager.h"
#include "GraphicsManager.h"

namespace GameEngineSpace
{
	std::shared_ptr<SceneManager> SceneManager::instance = nullptr;

	// 재우의 Unity Scene 불러오기 코드.
	// TODO 여기에 나오는 모든 컴포넌트를 제작한 뒤. 주석해제하면 됩니다.
	/*void SceneManager::LoadSceneData(std::string sceneName)
	{
		auto resourceManager = ResourceManager::GetInstance();

		auto sceneData = resourceManager->GetSceneData("SampleScene");

		auto scene = SceneManager::GetInstance()->GetCurrentScene();

		for (int i = 0; i < sceneData->gameObjects.size(); i++)
		{
			// 게임오브젝트 생성
			std::shared_ptr<GameObject> gameObject = scene->CreateEmpty();

			std::wstring gameObjectName;

			gameObjectName.assign(sceneData->gameObjects[i].name.begin(), sceneData->gameObjects[i].name.end());

			// 게임오브젝트 이름 세팅
			gameObject->SetName(gameObjectName);

			// 게임오브젝트 fileID 세팅
			gameObject->SetFileID(sceneData->gameObjects[i].gameObjectID);

			if (sceneData->gameObjects[i].transform != nullptr)
			{
				gameObject->AddComponent<Transform>();

				gameObject->GetComponent<Transform>()->gameObjectID = sceneData->gameObjects[i].transform->gameObjectID;
				gameObject->GetComponent<Transform>()->componentID = sceneData->gameObjects[i].transform->componentID;
				gameObject->GetComponent<Transform>()->childIDList = sceneData->gameObjects[i].transform->childIDList;

				gameObject->GetComponent<Transform>()->SetPosition(Vector3{
					sceneData->gameObjects[i].transform->localPosition.x,
					sceneData->gameObjects[i].transform->localPosition.y,
					sceneData->gameObjects[i].transform->localPosition.z });

				gameObject->GetComponent<Transform>()->SetRotation(Vector3{
					sceneData->gameObjects[i].transform->localRotation.x,
					sceneData->gameObjects[i].transform->localRotation.y,
					sceneData->gameObjects[i].transform->localRotation.z });

				gameObject->GetComponent<Transform>()->SetScale(Vector3{
					sceneData->gameObjects[i].transform->localScale.x,
					sceneData->gameObjects[i].transform->localScale.y,
					sceneData->gameObjects[i].transform->localScale.z });
			}

			if (sceneData->gameObjects[i].camera != nullptr)
			{
				scene->SetMainCamera(gameObject->AddComponent<Camera>());

				gameObject->GetComponent<Camera>()->gameObjectID = sceneData->gameObjects[i].camera->gameObjectID;
				gameObject->GetComponent<Camera>()->componentID = sceneData->gameObjects[i].camera->componentID;

				if (sceneData->gameObjects[i].camera->projectionMatrixMode == 0)
					gameObject->GetComponent<Camera>()->UpdateProjMatrix(ProjType::PERSPECTIVE);
				else
					gameObject->GetComponent<Camera>()->UpdateProjMatrix(ProjType::ORTHOGRAPHIC);

				gameObject->GetComponent<Camera>()->SetNearZ(sceneData->gameObjects[i].camera->nearPlane);

				gameObject->GetComponent<Camera>()->SetFarZ(sceneData->gameObjects[i].camera->farPlane);

				gameObject->AddComponent<BasicCameraController>();

				//gameObject->GetComponent<Transform>()->LookAt(Vector3{ 0.f, 0.f, 0.f });
			}

			if (sceneData->gameObjects[i].light != nullptr)
			{

			}

			if (sceneData->gameObjects[i].boxCollider != nullptr)
			{
				gameObject->AddComponent<BoxCollider>();

				gameObject->GetComponent<BoxCollider>()->gameObjectID = sceneData->gameObjects[i].boxCollider->gameObjectID;
				gameObject->GetComponent<BoxCollider>()->componentID = sceneData->gameObjects[i].boxCollider->componentID;

				gameObject->GetComponent<BoxCollider>()->SetSize(Vector3{
					sceneData->gameObjects[i].boxCollider->size.x,
					sceneData->gameObjects[i].boxCollider->size.y,
					sceneData->gameObjects[i].boxCollider->size.z });

				gameObject->GetComponent<BoxCollider>()->SetLocalCenter(Vector3{
				sceneData->gameObjects[i].boxCollider->center.x,
				sceneData->gameObjects[i].boxCollider->center.y,
				sceneData->gameObjects[i].boxCollider->center.z });

				gameObject->GetComponent<BoxCollider>()->InitRenderObj("WireCube");
			}

			if (sceneData->gameObjects[i].sphereCollider != nullptr)
			{
				/*gameObject->AddComponent<SphereCollider>();

				gameObject->GetComponent<SphereCollider>()->SetRadius(sceneData->gameObjects[i].sphereCollider->radius);

				gameObject->GetComponent<SphereCollider>()->SetLocalCenter(Vector3{
				sceneData->gameObjects[i].sphereCollider->center.x,
				sceneData->gameObjects[i].sphereCollider->center.y,
				sceneData->gameObjects[i].sphereCollider->center.z });#1#
			}

			if (sceneData->gameObjects[i].meshFilter != nullptr)
			{
				if (sceneData->gameObjects[i].meshFilter->meshName == "CubeMesh")
				{
					gameObject->AddComponent<MeshRenderer>()->Init("CubeMesh");
				}
				if (sceneData->gameObjects[i].meshFilter->meshName == "SphereMesh")
				{
					gameObject->AddComponent<MeshRenderer>()->Init("SphereMesh");
				}
				if (sceneData->gameObjects[i].meshFilter->meshName == "PlaneMesh")
				{
					gameObject->AddComponent<MeshRenderer>()->Init("PlaneMesh");
				}

				// meshrenderer 는 meshfilter로부터 가져오자..
				gameObject->GetComponent<MeshRenderer>()->gameObjectID = sceneData->gameObjects[i].meshFilter->gameObjectID;
				gameObject->GetComponent<MeshRenderer>()->componentID = sceneData->gameObjects[i].meshFilter->componentID;
			}
		}

		/// <summary>
		///	부모가 있다면 계층 구조로..
		/// </summary>
		auto gameObjectInScene = scene->GetGameObjectInScene();

		for (int i = 0; i < gameObjectInScene.size(); i++)
		{
			if (!gameObjectInScene[i]->GetComponent<Transform>()->childIDList.empty())
			{
				for (auto& childID : gameObjectInScene[i]->GetComponent<Transform>()->childIDList)
				{
					// 자식이 있는지 찾는다.
					auto it = find_if(gameObjectInScene.begin(), gameObjectInScene.end(), [&id = childID](const std::shared_ptr<GameObject>& obj)->bool {return (obj->GetComponent<Transform>()->componentID == id); });

					// 자식을 찾았다면
					if (it != gameObjectInScene.end())
					{
						gameObjectInScene[i]->SetChild(*it);
					}
				}
			}
		}

		for (int i = 0; i < sceneData->prefabs.size(); i++)
		{
			// 게임오브젝트 생성
			std::shared_ptr<GameObject> gameObject = scene->CreateEmpty();

			std::wstring gameObjectName;

			gameObjectName.assign(sceneData->prefabs[i].name.begin(), sceneData->prefabs[i].name.end());

			// 게임오브젝트 이름 세팅
			gameObject->SetName(gameObjectName);

			gameObject->AddComponent<Transform>();

			gameObject->GetComponent<Transform>()->SetPosition(Vector3{
				sceneData->prefabs[i].transform.localPosition.x,
				sceneData->prefabs[i].transform.localPosition.y,
				sceneData->prefabs[i].transform.localPosition.z });

			gameObject->GetComponent<Transform>()->SetRotation(Vector3{
				sceneData->prefabs[i].transform.localRotation.x,
				sceneData->prefabs[i].transform.localRotation.y,
				sceneData->prefabs[i].transform.localRotation.z });

			gameObject->GetComponent<Transform>()->SetScale(Vector3{
				sceneData->prefabs[i].transform.localScale.x,
				sceneData->prefabs[i].transform.localScale.y,
				sceneData->prefabs[i].transform.localScale.z });

			gameObject->AddComponent<MeshRenderer>()->Init(sceneData->prefabs[i].name);
		}
	}*/

	void SceneManager::ChangeScene()
	{
		// TODO Scene을 바꿀 때 일어나는 일 작성해보기
			// 렌더러 부분의 초기화
			// Scene 부분의 특정함수 호출
	}

	std::shared_ptr<SceneManager> SceneManager::GetInstance()
	{
		// 인스턴스가 없으면 만들어주고 있으면 인스턴스 리턴
		if (instance == nullptr)
		{
			instance = std::make_shared<SceneManager>();
		}

		return instance;
	}

	void SceneManager::Init()
	{
		// TODO 물리를 넣으면 여기서 물리를 Init해준다.
	}

	void SceneManager::Release()
	{
		currentScene.reset();

		physicsManager->Release();

		// 씬 전체 릴리즈
		for (auto& scene : scenes)
		{
			scene.second->ClearScene();
		}
	}

	void SceneManager::Update()
	{
		// 현재 씬이 있다면..
		if (currentScene != nullptr)
		{
			// TODO 현재 씬을 받아와서 특정 코드들을 업데이트 해줍니다.
				// 어떤 순서로 이벤트 함수를 호출해야하는가.. 외부에 있는 Render요소를 안에 넣을 수 있지 않을까 등을 고민합시다.
		}

		// 씬 변경이 예정되어 있으면.. 바꿔준다.
		if (reservedLoadScene)
		{
			ChangeScene();
		}
	}

	void SceneManager::AddGameScene(std::shared_ptr<SceneBase> _scene)
	{
		if (_scene != nullptr)
		{
			scenes.insert({ _scene->GetSceneName(), _scene });
		}
	}

	void SceneManager::LoadScene(tstring _sceneName)
	{
		// 씬이 Map에 존재하는가.
		assert(scenes.find(_sceneName) != scenes.end());

		// 씬을 예약한다.
		reservedLoadScene = true;
		reservedLoadSceneName = _sceneName;
	}

	void SceneManager::UpdateMainCameraAspectRatio(uint32 _screenWidth, uint32 _screenHeight)
	{
		// TODO 카메라의 비율을 바꿔줘야합니다.
	}

	void SceneManager::ChangeTick(float multiple)
	{
		isChangedTick = true;

		deltaMultiple = multiple;
	}
}
