#include "PhysicsAPI/PhysicsManager.h"

#include <iostream>

void PhysicsManager::Initialize()
{
	mRenderQ = SubsystemManager::Get<RenderQueue>();
}

void PhysicsManager::RecordingExecute(float deltaTime)
{
    HandleCollisions();
	for (auto& object : mRenderQ->mObjects)
	{
		object.second->GetPhysicsObject()->Update(deltaTime);
	}
}

void PhysicsManager::HandleCollisions()
{
    for (auto& object : mRenderQ->mObjects)
    {
        for (auto& otherObject : mRenderQ->mObjects)
        {
            if (object.first != otherObject.first)
            {
                auto* colliderA = object.second->GetPhysicsObject()->mCollider.get();
                auto* colliderB = otherObject.second->GetPhysicsObject()->mCollider.get();
               
                if (colliderA == nullptr || colliderB == nullptr) continue;

                if (colliderA->CheckCollision(colliderB))
                {
                    colliderA->ResolveCollision(colliderB);
                    colliderB->ResolveCollision(colliderA);

                    LogCollision(object.second->GetObjectName(),
                        otherObject.second->GetObjectName());
                }
            }
        }
    }
}

void PhysicsManager::LogCollision(const std::string& left, const std::string& right)
{
    if ((left == _collider || left == _collided) &&
        (right == _collider || right == _collided))
    {
        return;
    }
    _collided = left;
    _collider = right;

    std::cout << _collider << " Collided with: " << _collided << "!\n";
}
