#include "Precompiled.h"
#include "WanderBehaviour.h"
#include "Agent.h"

using namespace AI;

X::Math::Vector2 AI::WanderBehaviour::Calculate(Agent& agent)
{
	//apply random jitter
	auto newWanderTarget = mLocalWanderTarget + (X::RandomUnitCircle() * mWanderJitter);

	//snap the new position back to the wander circle
	newWanderTarget = X::Math::Normalize(newWanderTarget) * mWanderRadius;
	mLocalWanderTarget = newWanderTarget;

	//transform target into world space
	const auto worldTransform = agent.GetWorldTransform();
	const auto worldWanderTarget = X::Math::TransformCoord(newWanderTarget, worldTransform);

	//check if at target
	const auto agentDist = worldWanderTarget - agent.position;
	const float distToDest = X::Math::Magnitude(agentDist);
	if (distToDest <= 0.01f)
	{
		return X::Math::Vector2::Zero();
	}

	//seek to wandertarget
	const auto desiredVelocity = (agentDist / distToDest) * agent.maxSpeed;
	auto seekForce = desiredVelocity - agent.velocity;

	if (GetDebug())
	{
		const auto wanderCenter = X::Math::TransformCoord({ 0.0f, mWanderDistance }, worldTransform);
		X::DrawScreenCircle(wanderCenter, mWanderRadius, X::Colors::Yellow);
		X::DrawScreenDiamond(worldWanderTarget, 3.0f, X::Colors::Red);
		X::DrawScreenLine(agent.position, worldWanderTarget, X::Colors::Green);
	}

	return seekForce;
}

void AI::WanderBehaviour::Setup(float radius, float distance, float jitter)
{
	mWanderRadius = radius;
	mWanderDistance = distance;
	mWanderJitter = jitter;
}
