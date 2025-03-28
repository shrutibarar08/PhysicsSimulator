#pragma once

#include "GuiAPI/PhysicsUI/IPhysicsUI.h"
#include "Collider/SphereCollider.h"

class IColliderUI: public IPhysicsUI
{
public:
	virtual void Update(float deltaTime);
	void OnUpdateGUI() override;

	virtual void ChildUpdateGUI() = 0;
    virtual ICollider* Collider() = 0;
	bool IsValid() { return Collider() != nullptr; }
};
