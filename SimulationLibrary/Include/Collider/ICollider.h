#pragma once
#include <string>
#include <unordered_map>

#include "Compute/Particle.h"


class ICollider
{
public:
    ICollider() = default;

	void RegisterCollision(ICollider* collider, const DirectX::XMVECTOR& collisionNormal);
    void UpdateRestingStates(float deltaTime);

    void AttachParticle(Particle* attachParticle);

    virtual bool CheckCollision(ICollider* other) = 0;
    virtual void ResolveCollision(ICollider* other) = 0;

    std::string GetColliderName();
    void SetColliderName(const std::string& name);
    virtual ~ICollider() = default;

    bool AreStatic(const ICollider* other) const;

    Particle* mAttachedObject; // The mAttachedObject this collider is attached to

	//~ Members
    float Elastic{ 1.0f };
    bool bStatic{ false };

    struct CollisionInfo
    {
        int hitCount = 0;
        float lastHitTime = 0.0f;
    };

    std::unordered_map<ICollider*, CollisionInfo> mCollisionMap;
    float mTotalElapsedTime = 0.0f;
    int mRestThreshold{ 10 };
    float mRestTimeThreshold{ 1.f };

private:
    std::string mName{ "UNKNOWN" };
};
