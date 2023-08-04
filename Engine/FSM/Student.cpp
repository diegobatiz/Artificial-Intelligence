#include "Student.h"


void Student::Initialize()
{
	mLocation = Location::Bed;
	mHunger = 0;
	mEffort = 0;
	mBoredom = 0;
	mTired = 0;
}

void Student::Terminate()
{
}

void Student::Update(float deltaTime)
{
	mStateMachine.Update(deltaTime);
}

void Student::ChangeState(StudentState pNewState)
{
	mStateMachine.ChangeState((int)pNewState);
}

Student::Location Student::GetLocation()
{
	return Location();
}

bool Student::IsHungry() const
{
	return false;
}

bool Student::IsFull() const
{
	return false;
}

bool Student::HasEffort() const
{
	return false;
}

bool Student::IsBored() const
{
	return false;
}

bool Student::IsRested() const
{
	return false;
}

bool Student::IsTired() const
{
	return false;
}

void Student::SetLocation(Location location)
{
}

void Student::IncreaseHunger()
{
}

void Student::DecreaseHunger()
{
}

void Student::IncreaseEffort()
{
}

void Student::DecreaseEffort()
{
}

void Student::IncreaseBoredom()
{
}

void Student::DecreaseBoredom()
{
}

void Student::IncreaseFatigue()
{
}

void Student::DecreaseFatigue()
{
}
