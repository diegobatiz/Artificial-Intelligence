#pragma once
#include "State.h"

namespace AI
{
	template<class AgentType>
	class StateMachine final
	{
	public:
		StateMachine(AgentType& agent)
			: mAgent(agent)
		{
			
		}

		template <class NewState>
		void AddState()
		{
			static_assert(std::is_base_of_v<State, NewState>, "StateMachine: can onlt add state of same type");
			mStates.push_back(std::make_unique<NewState>());
		}
		void Update(float deltaTime)
		{
			if (mCurrentState != nullptr)
			{
				mCurrentState->Update(mAgent, deltaTime);
			}
			
		}

		void ChangeState(int index)
		{
			if (mCurrentState != nullptr)
			{
				mCurrentState->Exit(mAgent);
			}
			mCurrentState = mStates[index].get();
			mCurrentState->Enter(mAgent);
		}

	private:
		AgentType& mAgent;
		State<AgentType>* mCurrentState = nullptr;
		std::vector<std::unique_ptr<State<AgentType>> mStates;
	};
}