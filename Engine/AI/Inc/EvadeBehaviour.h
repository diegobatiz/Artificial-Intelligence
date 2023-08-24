#pragma once
#include "SteeringBehaviour.h"

namespace AI
{
	class EvadeBehaviour : public SteeringBehaviour
	{
	public:
		X::Math::Vector2 Calculate(Agent& agent) override;

	private:
		float panicDistance = 300.0f;
	};
}