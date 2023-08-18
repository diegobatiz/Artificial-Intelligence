#include "Precompiled.h"
#include "SteeringModule.h"

AI::SteeringModule::SteeringModule(Agent& agent)
	: mAgent(agent)
{
}

X::Math::Vector2 AI::SteeringModule::Calculate()
{
	X::Math::Vector2 totalForce;
	for (auto& behaviour : mBehaviours)
	{
		if (behaviour->IsActive())
		{
			totalForce += behaviour->Calculate(mAgent) * behaviour->GetWeight();
		}
	}

	return totalForce;
}
