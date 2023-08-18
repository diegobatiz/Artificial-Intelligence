#pragma once
#include "SteeringBehaviour.h"

namespace AI
{
	class ArriveBehaviour : public SteeringBehaviour
	{
	public:
		enum class Deacceleration
		{
			Fast,
			Normal,
			Slow
		};

		X::Math::Vector2 Calculate(Agent& agent) override;
	};
}