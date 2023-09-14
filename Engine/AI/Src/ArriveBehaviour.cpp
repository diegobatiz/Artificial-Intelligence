#include "Precompiled.h"
#include "ArriveBehaviour.h"
#include "Agent.h"

using namespace AI;

extern ArriveBehaviour::Deacceleration deacceleration;

X::Math::Vector2 AI::ArriveBehaviour::Calculate(Agent& agent)
{
	const auto agentToDest = agent.destination - agent.position;
	agent.distToDest = X::Math::Magnitude(agentToDest);
	float speed = 0.0f;

	if (agent.distToDest > 1.0f)
	{
		const float deaccelTweaker = agent.mass * 1.5f;
		speed = agent.distToDest / ((float)deacceleration + 1.0f) * deaccelTweaker;
		speed = X::Math::Min(speed, agent.maxSpeed);
	}

	const auto desiredVelocity = (agentToDest / agent.distToDest) * speed;
	const auto arriveForce = desiredVelocity - agent.velocity;

	if (GetDebug())
	{
		X::DrawScreenLine(agent.position, agent.position + desiredVelocity, X::Colors::Yellow);
		X::DrawScreenLine(agent.position, agent.position + agent.velocity, X::Colors::Green);
		X::DrawScreenCircle(agent.destination, 20.0f, X::Colors::Red);
	}

	return arriveForce;
}
