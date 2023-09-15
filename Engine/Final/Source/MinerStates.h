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

		Types targetType = agent.GetTargetType();
		if (targetType == Types::CrystalID)
		{
			agent.ChangeState(MinerStates::SeekingMine);
		}
		else if (targetType == Types::BadGuyID)
		{
			agent.ChangeState(MinerStates::Fleeing);
		}
	}

	void Exit(Miner& agent) override
	{
		agent.SetWander(false);
		agent.prevState = MinerStates::Wandering;
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
		if (agent.HasTarget())
		{
			agent.destination = agent.GetTargetPos();
		}
	}

	void Update(Miner& agent, float deltaTime) override
	{
		if (agent.position.x >= agent.destination.x - 2 && agent.position.x <= agent.destination.x + 2 && agent.position.y >= agent.destination.y - 2 && agent.position.y <= agent.destination.y + 2)
		{
			agent.ChangeState(MinerStates::Mining);
			return;
		}
		else if(!agent.HasTarget())
			return;
		else if (agent.GetTargetType() == Types::BadGuyID)
		{
			agent.ChangeState(MinerStates::Fleeing);
		}
		else if (agent.HasTarget())
		{
			agent.destination = agent.GetTargetPos();
		}
	}

	void Exit(Miner& agent) override
	{
		agent.SetArrive(false);
		agent.prevState = MinerStates::SeekingMine;
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

		if (mineTimer <= 0.0f)
		{
			agent.ChangeState(MinerStates::Returning);
			agent.hasCrystal = true;
			return;
		}

		if (!agent.HasTarget())
			return;
		else if (agent.GetTargetType() == Types::BadGuyID)
		{
			agent.ChangeState(MinerStates::Fleeing);
		}
	}

	void Exit(Miner& agent) override
	{
		agent.prevState = MinerStates::Mining;
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
		if (!agent.HasTarget())
			return;
		else if (agent.GetTargetType() == Types::BadGuyID)
		{
			agent.ChangeState(MinerStates::Fleeing);
			return;
		}
		else if (agent.position.x >= agent.destination.x - 10 && agent.position.x <= agent.destination.x + 10 && agent.position.y >= agent.destination.y - 10 && agent.position.y <= agent.destination.y + 10)
		{
			agent.ChangeState(MinerStates::Wandering);
			agent.hasCrystal = false;
		}
	}

	void Exit(Miner& agent) override
	{
		agent.SetArrive(false);
		agent.prevState = MinerStates::Returning;
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
		if (!agent.HasTarget())
		{
			agent.ChangeState(agent.prevState);
		}
		else
		{
			agent.destination = agent.GetTargetPos();

			if (agent.distToDest > 300.0f)
			{
				agent.RemoveTarget();
				agent.ChangeState(agent.prevState);
			}
		}
	}

	void Exit(Miner& agent) override
	{
		agent.SetFlee(false);
		agent.prevState = MinerStates::Fleeing;
	}

	void DebugUI() override
	{
	}
};