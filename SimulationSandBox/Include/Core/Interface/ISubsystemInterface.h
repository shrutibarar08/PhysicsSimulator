#pragma once

/*
 *	Defines Common Subsystem Interface for all the subsystems
 */
class ISubsystemInterface
{
public:
	ISubsystemInterface()		   = default;
	virtual ~ISubsystemInterface() = default;

	virtual void Initialize(){}
	virtual void Shutdown(){}

	ISubsystemInterface(const ISubsystemInterface&)			   = delete;
	ISubsystemInterface& operator=(const ISubsystemInterface&) = delete;
	ISubsystemInterface(ISubsystemInterface&&)				   = delete;
	ISubsystemInterface& operator=(ISubsystemInterface&&)	   = delete;
};
