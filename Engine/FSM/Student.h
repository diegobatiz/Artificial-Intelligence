#pragma once
#include <AI.h>

enum class StudentState
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

	Student() = default;
	void Initialize();
	void Terminate();
	void Update(float deltaTime);
	void ChangeState(StudentState pNewState);
	void DebugUI();

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
	void IncreaseHunger(float deltaTime);
	void DecreaseHunger(float deltaTime);
	void IncreaseEffort(float deltaTime);
	void DecreaseEffort(float deltaTime);
	void IncreaseBoredom(float deltaTime);
	void DecreaseBoredom(float deltaTime);
	void IncreaseFatigue(float deltaTime);
	void DecreaseFatigue(float deltaTime);

private:
	AI::StateMachine<Student>* mStateMachine;
	Location mLocation;
	float mHunger;
	float mEffort;
	float mBoredom;
	float mTired;
};