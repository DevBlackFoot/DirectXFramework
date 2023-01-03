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
		// TODO ��ȹ �и� ������ ó��.
	}

	void PhysicsManager::Update(float deltaTime, std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		// ������Ʈ ��. ���� �������� �ʱ�ȭ.
		ResetColliders(colliderList);

		// �浹 üũ
		CheckCollisionAll(colliderList);

		// ���� üũ�� ���� ��ε�ĳ��Ʈ
		BroadCastTriggerEvents(colliderList);
	}

	void PhysicsManager::Release()
	{
		// TODO ���� ��ȹ ������.
	}

	void PhysicsManager::CheckCollisionAll(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		// �ش� �κ��� ���߿� ���� ������ �� �� ���� ������..
	}

	void PhysicsManager::ResetColliders(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		
	}

	void PhysicsManager::BroadCastTriggerEvents(std::vector<std::shared_ptr<ColliderBase>>& colliderList)
	{
		
	}
}
