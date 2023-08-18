#include "Precompiled.h"
#include "SeekBehaviour.h"
#include "Agent.h"

using namespace AI;

X::Math::Vector2 AI::SeekBehaviour::Calculate(Agent& agent)
{
	const auto agentToDest = agent.destination - agent.position;
	const float distToDest = X::Math::Magnitude(agentToDest);
	if (distToDest <= 0.1f)
	{
		return X::Math::Vector2::Zero();
	}

	const auto desiredVelocity = (agentToDest / distToDest) * agent.maxSpeed;
	const auto seekForce = desiredVelocity - agent.velocity;

	if (GetDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::Yellow);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Green);
		X::DrawScreenCircle(agent.destination, 20.0f, X::Colors::Red);
	}

	return seekForce;
}

X::Math::Vector2 AI::FleeBehaviour::Calculate(Agent& agent)
{
	X::Math::Vector2 fleeForce = X::Math::Vector2::Zero();
	X::Math::Vector2 desiredVelocity = X::Math::Vector2::Zero();

	const auto agentToDest = agent.destination - agent.position;
	const float distToDest = X::Math::Magnitude(agentToDest);

	if (distToDest < panicDistance)
	{
		desiredVelocity = -(agentToDest / distToDest) * agent.maxSpeed;
	}

	fleeForce = desiredVelocity - agent.velocity;

	if (GetDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::Yellow);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Green);
		X::DrawScreenCircle(agent.destination, 20.0f, X::Colors::Red);
	}

	return fleeForce;
}
