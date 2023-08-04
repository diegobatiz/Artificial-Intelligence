#pragma once
#include <AI.h>
#include "StudentStates.h"


enum StudentState
{
	Sleeping,
	Eating,
	Driving,
	Studying,
	Partying
};

class Student
{
public:
	enum Location
	{
		Bed, 
		Car,
		School,
		Party
	};

	Student();
	void Initialize();
	void Terminate();
	void Update(float deltaTime);
	void ChangeState(StudentState pNewState);

	//CheckState
	Location GetLocation();
	bool IsHungry() const;
	bool IsFull() const;
	bool HasEffort() const;
	bool IsBored() const;
	bool IsRested() const;
	bool IsTired() const;

	//Action Functions
	void SetLocation(Location location);
	void IncreaseHunger();
	void DecreaseHunger();
	void IncreaseEffort();
	void DecreaseEffort();
	void IncreaseBoredom();
	void DecreaseBoredom();
	void IncreaseFatigue();
	void DecreaseFatigue();

private:
	AI::StateMachine<StudentState> mStateMachine;
	Location mLocation;
	int mHunger;
	int mEffort;
	int mBoredom;
	int mTired;
};