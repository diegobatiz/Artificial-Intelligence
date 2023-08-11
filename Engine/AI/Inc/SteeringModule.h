#pragma once

#include "SteeringBehaviour.h"

namespace AI
{
	class SteeringModule final
	{
	public:
		SteeringModule(Agent& agent);

		template<class NewBehaviourType>
		NewBehaviourType* AddNewBehaviour()
		{
			static_assert(std::is_base_of_v<SteeringBehaviour, NewBehaviourType>, "Steering Behaviour: can only derive from SteeringBehaviour");
			auto& newBehaviour = mBehaviours.emplace_back(std::make_unique<NewBehaviourType>());
			return static_cast<NewBehaviourType*>(newBehaviour.get());
		}

		X::Math::Vector2 Calculate();

	private:
		Agent& mAgent;

		using SteeringBehaviours = std::vector<std::unique_ptr<SteeringBehaviour>>;
		SteeringBehaviours mBehaviours;
	};
}