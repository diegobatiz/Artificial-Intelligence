#include "Precompiled.h"
#include "AIWorld.h"

using namespace AI;

void AIWorld::Initialize()
{
}

void AIWorld::Update()
{
}

void AIWorld::Register(Entity* entity)
{
	XASSERT(std::find(mEntities.begin(), mEntities.end(), entity) == mEntities.end(), "AIWorld: entity already added!");
	mEntities.push_back(entity);
}

void AIWorld::Unregister(Entity* entity)
{
	auto itr = std::find(mEntities.begin(), mEntities.end(), entity);

	if (itr != mEntities.end())
	{
		mEntities.erase(itr);
	}
}
