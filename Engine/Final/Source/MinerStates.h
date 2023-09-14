#pragma once

#include <AI.h>
#include "Miner.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

class WanderState : public AI::State<Miner>
{
	void Enter(Miner& agent) override
	{
		agent.SetWander(true);
	}

	void Update(Miner& agent, float deltaTime) override
	{
		agent.SetupWander(wanderRadius, wanderDistance, wanderJitter);

		if (!agent.HasTarget())
			return;
		if (agent.GetTargetType() == Types::CrystalID)
		{
			agent.ChangeState(MinerStates::SeekingMine);
		}
		else if (agent.GetTargetType() == Types::BadGuyID)
		{
			agent.ChangeState(MinerStates::Fleeing);
		}
	}

	void Exit(Miner& agent) override
	{
		agent.SetWander(false);
	}

	void DebugUI() override
	{
	}
};



class SeekingMineState : public AI::State<Miner>
{
	void Enter(Miner& agent) override
	{
		agent.SetArrive(true);
		agent.destination = agent.GetTargetPos();
	}

	void Update(Miner& agent, float deltaTime) override
	{
		if (!agent.HasTarget())
		{
			agent.ChangeState(MinerStates::Wandering);
		}
		else if (agent.GetTargetType() == Types::BadGuyID)
		{
			agent.ChangeState(MinerStates::Fleeing);
		}
		else if (agent.position == agent.destination)
		{
			agent.ChangeState(MinerStates::Mining);
		}
	}

	void Exit(Miner& agent) override
	{
		agent.SetArrive(false);
	}

	void DebugUI() override
	{
	}
};



class MiningState : public AI::State<Miner>
{
	float mineTimer;

	void Enter(Miner& agent) override
	{
		mineTimer = 2.0f;
	}

	void Update(Miner& agent, float deltaTime) override
	{
		mineTimer -= deltaTime;

		if (agent.GetTargetType() == Types::BadGuyID)
		{
			agent.ChangeState(MinerStates::Fleeing);
		}
		else if (mineTimer <= 0.0f)
		{
			agent.ChangeState(MinerStates::Returning);
			agent.hasCrystal = true;
		}
	}

	void Exit(Miner& agent) override
	{
	}

	void DebugUI() override
	{
	}
};



class ReturningState : public AI::State<Miner>
{
	void Enter(Miner& agent) override
	{
		agent.SetDestinationBase();
		agent.SetArrive(true);
	}

	void Update(Miner& agent, float deltaTime) override
	{
		if (agent.GetTargetType() == Types::BadGuyID)
		{
			agent.ChangeState(MinerStates::Fleeing);
		}
		else if (agent.position == agent.destination)
		{
			agent.ChangeState(MinerStates::Wandering);
			agent.hasCrystal = false;
		}
	}

	void Exit(Miner& agent) override
	{
		agent.SetArrive(false);
	}

	void DebugUI() override
	{
	}
};



class FleeingState : public AI::State<Miner>
{
	void Enter(Miner& agent) override
	{
		agent.SetFlee(true);
	}

	void Update(Miner& agent, float deltaTime) override
	{
		agent.destination = agent.GetTargetPos();
		
		if (agent.distToDest > 300.0f)
		{
			//return to previous state
		}
	}

	void Exit(Miner& agent) override
	{
		agent.SetFlee(false);
	}

	void DebugUI() override
	{
	}
};