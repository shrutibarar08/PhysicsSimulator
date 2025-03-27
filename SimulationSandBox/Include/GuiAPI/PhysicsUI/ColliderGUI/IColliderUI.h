#pragma once

#include "GuiAPI/PhysicsUI/IPhysicsUI.h"
#include "Collider/SphereCollider.h"

class IColliderUI: public IPhysicsUI
{
public:
    virtual ICollider* Collider() = 0;
	bool IsValid() { return Collider() != nullptr; }
};
