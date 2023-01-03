#include "GamePch.h"
#include "PhysicsManager.h"

#include "Object/GameObject.h"

namespace GameEngineSpace
{
	std::shared_ptr<PhysicsManager> PhysicsManager::instance = nullptr;

	std::shared_ptr<PhysicsManager> PhysicsManager::GetInstance()
	{
		if(instance == nullptr)
			instance = std::make_shared<PhysicsManager>();

		return instance;
	}

	void PhysicsManager::Init()
	{
		// TODO 구획 분리 나눠서 처리.
	}

	void PhysicsManager::Update(float deltaTime, std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		// 업데이트 전. 지난 프레임을 초기화.
		ResetColliders(colliderList);

		// 충돌 체크
		CheckCollisionAll(colliderList);

		// 이후 체크된 것을 브로드캐스트
		BroadCastTriggerEvents(colliderList);
	}

	void PhysicsManager::Release()
	{
		// TODO 나눈 구획 릴리즈.
	}

	void PhysicsManager::CheckCollisionAll(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		// 해당 부분은 나중에 물리 엔진을 달 수 있지 않을까..
	}

	void PhysicsManager::ResetColliders(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		
	}

	void PhysicsManager::BroadCastTriggerEvents(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		
	}
}
