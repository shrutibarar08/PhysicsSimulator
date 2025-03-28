#include "pch.h"
#include "Collider/ICollider.h"


void ICollider::RegisterCollision(ICollider* collider, const DirectX::XMVECTOR& collisionNormal)
{
    float normalY = DirectX::XMVectorGetY(collisionNormal);

    if (normalY > 0.7f)  // Considers only vertical impacts
    {
        auto& collisionData = mCollisionMap[collider];
        collisionData.hitCount++;
        collisionData.lastHitTime = mTotalElapsedTime;

        if (collisionData.hitCount > mRestThreshold)
        {
            mAttachedObject->SetResting(true);
        }
    }
}

void ICollider::UpdateRestingStates(float deltaTime)
{
    mTotalElapsedTime += deltaTime;

    for (auto it = mCollisionMap.begin(); it != mCollisionMap.end();)
    {
        auto& data = it->second;
        ICollider* collider = it->first;

        if (mTotalElapsedTime - data.lastHitTime > mRestTimeThreshold)
        {
            mAttachedObject->SetResting(false);
            it = mCollisionMap.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void ICollider::AttachParticle(Particle* attachParticle)
{
	mAttachedObject = attachParticle;
}

std::string ICollider::GetColliderName()
{
	return mName;
}

void ICollider::SetColliderName(const std::string& name)
{
	mName = name;
}

bool ICollider::AreStatic(const ICollider* other) const
{
	return bStatic && other->bStatic;
}
