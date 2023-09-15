#include "Precompiled.h"
#include "PerceptionModule.h"

AI::PerceptionModule::PerceptionModule(Agent& agent, ImportanceCalculator calculator)
	: mAgent(agent)
	, ComputeImportance(calculator)
{
	SetMemorySpan(0.0f);
}

void AI::PerceptionModule::Update(float deltaTime)
{
	//update sensors
	for (auto& sensor : mSensors)
	{
		sensor->Update(mAgent, mMemoryRecords, deltaTime);
	}

	//remove old memories
	mMemoryRecords.erase(
		std::remove_if(
			mMemoryRecords.begin(),
			mMemoryRecords.end(),
			[span = mMemorySpan](const auto& record)
			{
				return record.lastRecordedTime + span < X::GetTime();
			}), mMemoryRecords.end());

	for (auto& record : mMemoryRecords)
	{
		record.importance = ComputeImportance(mAgent, record);
	}

	mMemoryRecords.sort(
		[](const auto& a, const auto& b)
		{
			return a.importance > b.importance;
		});
}

AI::MemoryRecord AI::PerceptionModule::GetMostImportant()
{
	return mMemoryRecords.front();
}

void AI::PerceptionModule::RemoveFront()
{
	mMemoryRecords.pop_front();
}
