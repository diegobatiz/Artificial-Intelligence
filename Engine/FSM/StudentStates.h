#pragma once
#include "AI.h"

#include "Student.h"

class SleepingState : public AI::State<Student>
{
	void Enter(Student& agent) override
	{
		agent.SetLocation(Student::Location::Bed);
	}

	void Update(Student& agent, float deltaTime) override
	{
		agent.IncreaseHunger(deltaTime);
		agent.IncreaseEffort(deltaTime);
		agent.DecreaseFatigue(deltaTime);
		if (agent.IsHungry())
		{
			agent.ChangeState(StudentState::Eating);
		}
		else if (agent.IsRested())
		{
			agent.ChangeState(StudentState::Driving);
		}
	}

	void Exit(Student& agent) override
	{

	}
};

class EatingState : public AI::State<Student>
{
	void Enter(Student& agent) override
	{

	}

	void Update(Student& agent, float deltaTime) override
	{
		agent.DecreaseHunger(deltaTime);

		if (agent.IsFull())
		{
			if (agent.GetLocation() == Student::Location::Bed)
			{
				agent.ChangeState(StudentState::Sleeping);
			}
			else if (agent.GetLocation() == Student::Location::Car)
			{
				agent.ChangeState(StudentState::Driving);
			}
			else if (agent.GetLocation() == Student::Location::School)
			{
				agent.ChangeState(StudentState::Studying);
			}
			else if (agent.GetLocation() == Student::Location::Party)
			{
				agent.ChangeState(StudentState::Partying);
			}
		}
	}

	void Exit(Student& agent) override
	{

	}
};

class DrivingState : public AI::State<Student>
{
	void Enter(Student& agent) override
	{
		agent.SetLocation(Student::Location::Car);
	}

	void Update(Student& agent, float deltaTime) override
	{
		agent.IncreaseHunger(deltaTime);
		agent.IncreaseFatigue(deltaTime);
		agent.IncreaseBoredom(deltaTime);
		if (agent.IsTired())
		{
			agent.ChangeState(StudentState::Sleeping);
		}
		else if (agent.IsHungry())
		{
			agent.ChangeState(StudentState::Eating);
		}
		else if (agent.HasEffort())
		{
			agent.ChangeState(StudentState::Studying);
		}
		else if (agent.IsBored())
		{
			agent.ChangeState(StudentState::Partying);
		}
	}

	void Exit(Student& agent) override
	{

	}
};

class StudyingState : public AI::State<Student>
{
	void Enter(Student& agent) override
	{
		agent.SetLocation(Student::Location::School);
	}

	void Update(Student& agent, float deltaTime) override
	{
		agent.IncreaseHunger(deltaTime);
		agent.IncreaseFatigue(deltaTime);
		agent.IncreaseBoredom(deltaTime);
		agent.DecreaseEffort(deltaTime);
		if (agent.IsTired())
		{
			agent.ChangeState(StudentState::Driving);
		}
		else if (agent.IsHungry())
		{
			agent.ChangeState(StudentState::Eating);
		}
	}

	void Exit(Student& agent) override
	{

	}
};

class PartyingState : public AI::State<Student>
{
	void Enter(Student& agent) override
	{
		agent.SetLocation(Student::Location::Party);
	}

	void Update(Student& agent, float deltaTime) override
	{
		agent.IncreaseHunger(deltaTime);
		agent.IncreaseFatigue(deltaTime);
		agent.IncreaseEffort(deltaTime);
		agent.DecreaseBoredom(deltaTime);
		if (agent.IsTired())
		{
			agent.ChangeState(StudentState::Driving);
		}
		else if (agent.IsHungry())
		{
			agent.ChangeState(StudentState::Eating);
		}
	}

	void Exit(Student& agent) override
	{

	}
};