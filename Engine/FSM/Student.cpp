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
	return mLocation;
}

bool Student::IsHungry() const
{
	return mHunger > 10.0f;
}

bool Student::IsFull() const
{
	return mHunger <= 0.0f;
}

bool Student::HasEffort() const
{
	return mEffort > 15.0f && mEffort > mBoredom;
}

bool Student::IsBored() const
{
	return mBoredom > 8.0f&& mBoredom > mEffort;
}

bool Student::IsRested() const
{
	return mTired <= 0.0f;
}

bool Student::IsTired() const
{
	return mTired > 12.0f;
}

void Student::SetLocation(Location location)
{
	mLocation = location;
}

void Student::IncreaseHunger(float deltaTime)
{
	mHunger += deltaTime;
}

void Student::DecreaseHunger(float deltaTime)
{
	mHunger -= deltaTime;
}

void Student::IncreaseEffort(float deltaTime)
{
	mEffort += deltaTime / 2.0f;
}

void Student::DecreaseEffort(float deltaTime)
{
	mEffort += deltaTime * 2.0f;
}

void Student::IncreaseBoredom(float deltaTime)
{
	mBoredom += deltaTime * 2.0f;
}

void Student::DecreaseBoredom(float deltaTime)
{
	mBoredom += deltaTime / 2.0f;
}

void Student::IncreaseFatigue(float deltaTime)
{
	mTired += deltaTime;
}

void Student::DecreaseFatigue(float deltaTime)
{
	mTired -= deltaTime;
}
