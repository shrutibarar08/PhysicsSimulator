#include "Core/IScenarioBase.h"
#include "RenderAPI/RenderQueue.h"

void IScenarioBase::OnLoad()
{
	for (int i = 0; i < mObjects.size(); i++)
	{
		RenderQueue::RegisterObject(mObjects[i].get());
	}
}

void IScenarioBase::OnUnload()
{
	for (int i = 0; i < mObjects.size(); i++)
	{
		RenderQueue::UnRegisterObject(mObjects[i].get());
	}
}
