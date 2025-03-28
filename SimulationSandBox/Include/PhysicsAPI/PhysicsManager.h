#pragma once

#include "Core/Interface/ISubsystemInterface.h"
#include "RenderAPI/RenderQueue.h"


class PhysicsManager final: public ISubsystemInterface, public IFrameInterface
{
public:
	void Initialize() override;
	void RecordingExecute(float deltaTime) override;

private:
	void HandleCollisions() const;

private:
	RenderQueue* mRenderQ{ nullptr };

	//~ Cache for better logging
	std::string _collider{ "" };
	std::string _collided{ "" };
};
