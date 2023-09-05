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

void AI::AIWorld::AddObstacle(const X::Math::Circle& obstacle)
{
	mObstacles.push_back(obstacle);
}

void AI::AIWorld::AddWall(const X::Math::LineSegment& wall)
{
	mWalls.push_back(wall);
}

bool AI::AIWorld::HasLineOfSight(const X::Math::LineSegment& lineSegment) const
{
	for (auto& wall : mWalls)
	{
		if (X::Math::Intersect(lineSegment, wall))
		{
			return false;
		}
	}
	for (auto& obstacle : mObstacles)
	{
		if (X::Math::Intersect(lineSegment, obstacle))
		{
			return false;
		}
	}

	return true;
}
