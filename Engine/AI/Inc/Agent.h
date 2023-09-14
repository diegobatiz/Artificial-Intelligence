#pragma once

#include "Entity.h"

namespace AI
{
	class Agent;

	using AgentPtrs = std::vector<Agent*>;

	class Agent : public Entity
	{
	public:
		Agent(AIWorld& world, uint32_t typeId);
		~Agent() override = default;

		const Agent* target = nullptr;

		float distToDest = 0.f;
		X::Math::Vector2 destination = X::Math::Vector2::Zero();
		X::Math::Vector2 velocity = X::Math::Vector2::Zero();
		float maxSpeed = 100.0f;
		float mass = 1.0f;
	};
}