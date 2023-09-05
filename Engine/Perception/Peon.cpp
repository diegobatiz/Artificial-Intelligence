#include "Peon.h"
#include "TypeIDs.h"
#include "VisualSensor.h"
#include "MemoryRecord.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;

extern float viewRange;
extern float viewAngle;

namespace
{
	float ComputeImportance(const AI::Agent& agent, const AI::MemoryRecord& record)
	{
		Types entityType = static_cast<Types>(record.GetProperty<int>("type"));
		switch (entityType)
		{
		case Types::Invalid: return 10000.0f;
		case Types::PeonId:
		{
			float distance = X::Math::Distance(agent.position, record.GetProperty<X::Math::Vector2>("lastSeenPosition"));
			float distanceScore = std::max(1000.0f - distance, 0.0f);
			return distanceScore;
		}
		break;
		case Types::MineralId:
		{
			float distance = X::Math::Distance(agent.position, record.GetProperty<X::Math::Vector2>("lastSeenPosition"));
			float distanceScore = std::max(10000.0f - distance, 0.0f);
			return distanceScore;
		}
		break;
		default: break;
		}

		return 10000.0f;
	}
}


Peon::Peon(AI::AIWorld& world)
	: Agent(world, Types::PeonId)
{
}

void Peon::Load()
{
	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
	mPerceptionModule->SetMemorySpan(3.0f);
	mVisualSensor = mPerceptionModule->AddSensor<VisualSensor>();
	mVisualSensor->targetType = Types::MineralId;

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSeekBehaviour = mSteeringModule->AddNewBehaviour<AI::SeekBehaviour>();
	mWanderBehaviour = mSteeringModule->AddNewBehaviour<AI::WanderBehaviour>();
	
	mWanderBehaviour->SetActive(true);

	for (int i = 0; i < mTextureIds.size(); i++)
	{
		char name[128];
		sprintf_s(name, "scv_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}
}

void Peon::Unload()
{
}

void Peon::Update(float deltaTime)
{
	mVisualSensor->viewRange = viewRange;
	mVisualSensor->viewHalfAngle = viewAngle * X::Math::kDegToRad;
	mPerceptionModule->Update(deltaTime);
	if (mWanderBehaviour->IsActive())
	{
		mWanderBehaviour->Setup(wanderRadius, wanderDistance, wanderJitter);
	}

	const auto force = mSteeringModule->Calculate();
	const auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	if (X::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = X::Math::Normalize(velocity);
	}

	position += velocity * deltaTime;

	const auto screenWidth = X::GetScreenWidth();
	const auto screenHeight = X::GetScreenHeight();

	if (position.x < 0.0f)
	{
		position.x += screenWidth;
	}
	else if (position.x >= screenWidth)
	{
		position.x -= screenWidth;
	}
	if (position.y < 0.0f)
	{
		position.y += screenHeight;
	}
	else if (position.y >= screenHeight)
	{
		position.y -= screenHeight;
	}

	const auto& memoryRecords = mPerceptionModule->GetMemoryRecords();
	for (auto& memory : memoryRecords)
	{
		auto pos = memory.GetProperty<X::Math::Vector2>("lastSeenPosition");
		X::DrawScreenLine(position, pos, X::Colors::Red);

		std::string score = std::to_string(memory.importance);
		X::DrawScreenText(score.c_str(), pos.x, pos.y, 12.0f, X::Colors::White);
	}
}

void Peon::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();
	X::DrawSprite(mTextureIds[frame], position);
}

void Peon::ShowDebug(bool debug)
{
	mWanderBehaviour->IsDebug(debug);
	mSeekBehaviour->IsDebug(debug);
}
