#pragma once
#include "ICollider.h"


class SphereCollider;

class CubeCollider: public ICollider
{
public:
	bool CheckCollision(ICollider* other) override;
	void ResolveCollision(ICollider* other) override;
	~CubeCollider() override = default;

	void UpdateTransform();

	DirectX::XMFLOAT3 mScale;
	DirectX::XMFLOAT3 mHalfExtents;
	DirectX::XMFLOAT4X4 mTransform;

private:
	void HandleCubeCollision(CubeCollider* other);
	bool CheckOverlap(const CubeCollider* other, DirectX::XMVECTOR& pushDirection, float& minOverlap) const;
	void ApplyPositionCorrection(CubeCollider* other, DirectX::XMVECTOR pushDirection, float minOverlap) const;
	void ApplyVelocityResponse(const CubeCollider* other, DirectX::XMVECTOR pushDirection) const;

	//~ Sphere Collider
	void HandleSphereCollider(SphereCollider* other);
	bool CheckCubeSphereOverlap(const SphereCollider* other, DirectX::XMVECTOR& pushDirection, float& minOverlap) const;
	void ApplyPositionCorrectionFromSphere(const SphereCollider* other, DirectX::XMVECTOR pushDirection,
	                                       float minOverlap) const;
	void ApplyVelocityResponseFromSphere(const SphereCollider* other, DirectX::XMVECTOR pushDirection) const;
};
