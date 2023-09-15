#pragma once
#include <AI.h>
#include "BadGuy.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

class WanderState : public AI::State<BadGuy>
{
	void Enter(BadGuy& agent) override
	{
		agent.SetWander(true);
	}

	void Update(BadGuy& agent, float deltaTime) override
	{
		agent.SetupWander(wanderRadius, wanderDistance, wanderJitter);

		if (!agent.HasTarget())
			return;

		Types targetType = agent.GetTargetType();
		if (targetType == Types::MinerID)
		{
			agent.ChangeState(BadGuyStates::ChaseMiner);
		}
	}

	void Exit(BadGuy& agent) override
	{
		agent.SetWander(false);
		agent.prevState = BadGuyStates::Wandering;
	}

	void DebugUI() override
	{
	}
};

class ChasingState : public AI::State<BadGuy>
{
	void Enter(BadGuy& agent) override
	{
		agent.SetSeek(true);
		agent.destination = agent.GetTargetPos();
	}

	void Update(BadGuy& agent, float deltaTime) override
	{
		if (agent.position.x >= agent.destination.x - 2 && agent.position.x <= agent.destination.x + 2 && agent.position.y >= agent.destination.y - 2 && agent.position.y <= agent.destination.y + 2)
		{
			agent.ChangeState(BadGuyStates::Returning);
			//kill miner using ai world
		}
		else if (!agent.HasTarget())
		{
			agent.ChangeState(BadGuyStates::Wandering);
		}
	}

	void Exit(BadGuy& agent) override
	{
		agent.SetSeek(false);
		agent.prevState = BadGuyStates::ChaseMiner;
	}

	void DebugUI() override
	{
	}
};

class ReturnState : public AI::State<BadGuy>
{
	void Enter(BadGuy& agent) override
	{
		agent.SetArrive(true);
		agent.SetDestinationBase();
	}

	void Update(BadGuy& agent, float deltaTime) override
	{
		if (agent.position.x >= agent.destination.x - 10 && agent.position.x <= agent.destination.x + 10 && agent.position.y >= agent.destination.y - 10 && agent.position.y <= agent.destination.y + 10)
		{
			//deposit crystal in base
			agent.ChangeState(BadGuyStates::Wandering);
		}
	}

	void Exit(BadGuy& agent) override
	{
		agent.SetArrive(false);
		agent.prevState = BadGuyStates::Returning;
	}

	void DebugUI() override
	{
	}
};