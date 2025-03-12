#include "Core/Interface/IEntityInterface.h"


void IEntityInterface::UnAssignID()
{
	mAssignedID = -1;
}

void IEntityInterface::SetAssignID(int id)
{
	mAssignedID = id;
}

bool IEntityInterface::IsAssignID() const
{
	return mAssignedID != -1;
}

int IEntityInterface::GetAssignID() const
{
	return mAssignedID;
}

std::string IEntityInterface::GetObjectType() const
{
	return mObjectType;
}

void IEntityInterface::SetObjectType(const std::string& objectType)
{
	mObjectType = objectType;
}

std::string IEntityInterface::GetObjectName() const
{
	return mObjectName;
}

void IEntityInterface::SetObjectName(const std::string& name)
{
	mObjectName = name.substr(0, name.find('\0'));
}

PhysicsObject* IEntityInterface::GetPhysicsObject()
{
	return &mPhysicsObject;
}
