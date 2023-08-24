#include "Precompiled.h"
#include "EvadeBehaviour.h"
#include "Agent.h"

using namespace AI;

X::Math::Vector2 AI::EvadeBehaviour::Calculate(Agent& agent)
{
	X::Math::Vector2 fleeForce = X::Math::Vector2::Zero();
	X::Math::Vector2 desiredVelocity = X::Math::Vector2::Zero();

	const auto agentToDest = agent.target->destination - agent.position;
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
		X::DrawScreenCircle(agent.target->destination, 20.0f, X::Colors::Red);
	}

	return fleeForce;
}
