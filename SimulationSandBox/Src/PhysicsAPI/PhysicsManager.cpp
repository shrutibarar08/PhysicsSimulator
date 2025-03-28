#include "PhysicsAPI/PhysicsManager.h"

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
                auto* colliderAUI = object.second->GetPhysicsObject()->mCollider.get();
                auto* colliderBUI = otherObject.second->GetPhysicsObject()->mCollider.get();
               
                if (colliderAUI == nullptr || colliderBUI == nullptr) continue;

                auto* colliderA = colliderAUI->Collider();
                auto* colliderB = colliderBUI->Collider();

                if (colliderA == nullptr || colliderB == nullptr) continue;

                if (colliderA->CheckCollision(colliderB))
                {
                    colliderA->ResolveCollision(colliderB);
                    colliderB->ResolveCollision(colliderA);
                }
            }
        }
    }
}
