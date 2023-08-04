#include "Precompiled.h"
#include "Entity.h"

#include "AIWorld.h"

using namespace AI;

Entity::Entity(AIWorld& world, uint32_t typeId)
	: world(world)
	, mUniqueID(static_cast<uint64_t>(typeId) << 32 | world.GetNextId())
{
	world.Register(this);
}

Entity::~Entity()
{
	world.Unregister(this);
}
