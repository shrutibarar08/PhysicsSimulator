#pragma once
#include <string>

#include "RenderAPI/DefineRender.h"

class IEntityInterface
{
public:
	virtual ~IEntityInterface() = default;
	virtual void SetupObject() = 0;

	virtual void RenderObject(ID3D11DeviceContext* deviceContext) = 0;
	virtual void UpdateBuffer(const Simulation::WorldSpace& space) = 0;

	void UnAssignID();
	void SetAssignID(int id);
	bool IsAssignID() const;
	int GetAssignID() const;

	std::string GetObjectType() const;
	void SetObjectType(const std::string& objectType);

	std::string GetObjectName() const;
	void SetObjectName(const std::string& name);

protected:
	//~ Identity
	int mAssignedID{ -1 };
	std::string mObjectType{ "UNKNOWN" };
	std::string mObjectName{ "UNKNOWN" };
};
