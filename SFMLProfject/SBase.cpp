#include "SBase.h"
SBase::SBase(SystemManager* mgr, System sysId)
	:systemId(sysId), manager(mgr)
{

}

void SBase::addEntity(const unsigned int& entityId)
{
	mEntities.push_back(entityId);
}

bool SBase::hasEntity(const unsigned int& entityId)
{
	auto entity = std::find(mEntities.begin(), mEntities.end(), entityId);
	return entity == mEntities.end() ? false : true;
}

void SBase::removeEntity(const unsigned int& entityId)
{
	auto entity = std::find(mEntities.begin(), mEntities.end(), entityId);
	if (entity != mEntities.end())
	{
		mEntities.erase(entity);
	}
}

bool SBase::matchesFlag(const unsigned int& req) const
{
	for (auto& a : requirementFlags)
	{
		bool matches = true;
		for (int i = 0; i < componentTypes; ++i)
		{
			int reqVal = req & (i << i);
			if (reqVal)
			{
				if (!((a & (1 << i)) & reqVal))
				{
					matches = false;
				}
			}
		}
		if (matches)
		{
			return true;
		}
	}
	return false;
}