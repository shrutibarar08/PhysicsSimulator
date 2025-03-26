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

void PhysicsManager::HandleCollisions() const
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
                    std::cout << "Collision Detected Between: "
                	<< object.second->GetObjectName()
                	<< " And " << otherObject.second->GetObjectName()
                	<< "\n";
                }
            }
        }
    }
}
