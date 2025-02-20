#pragma once

/*
 *	Define Common Interface for Subsystems that need to be updated per frame
 */
class IFrameInterface
{
public:
	IFrameInterface() = default;
	virtual ~IFrameInterface() = default;

	virtual void RecordingSetup() {}	// Called Once Before Recordings.
	virtual void RecordingBegin() {}	// Before Executing Update Necessary stuff
	virtual void RecordingEnd() {}		// After Executing Clear unnecessary stuff
	virtual void RecordingExecute() {}	// RecordExecute it (draw, update timer, physics etc...)
};
