#include "Miner.h"
#include "VisualSensor.h"
#include "MemoryRecord.h"
#include "MinerStates.h"

extern float viewRange;
extern float viewAngle;

namespace
{
	float ComputeImportance(const AI::Agent& agent, const AI::MemoryRecord& record)
	{
		Types entityType = static_cast<Types>(record.GetProperty<int>("type"));

		switch (entityType)
		{
		case Types::Invalid: return 0.0f;
		case Types::MinerID: return 0.0f;
		case Types::CrystalID:
		{
			float distance = X::Math::Distance(agent.position, record.GetProperty<X::Math::Vector2>("lastSeenPosition"));
			float distanceScore = std::max(10000.0f - distance, 0.0f);
			return distanceScore;
		}
		break;
		case Types::BadGuyID:
		{
			return 10000.0f;
		}
		break;
		default: break;
		}

		return 0.0f;
	}
}


Miner::Miner(AI::AIWorld& world)
	: Agent(world, Types::MinerID)
{
}

void Miner::Load()
{
	mStateMachine = new AI::StateMachine<Miner>(*this);
	mStateMachine->AddState<WanderState>();
	mStateMachine->AddState<SeekingMineState>();
	mStateMachine->AddState<MiningState>();
	mStateMachine->AddState<ReturningState>();
	mStateMachine->AddState<FleeingState>();

	mPerceptionModule = std::make_unique<AI::PerceptionModule>(*this, ComputeImportance);
	mPerceptionModule->SetMemorySpan(5.0f);
	mBadGuySensor = mPerceptionModule->AddSensor<VisualSensor>();
	mCrystalSensor = mPerceptionModule->AddSensor<VisualSensor>();
	mBadGuySensor->targetType = Types::BadGuyID;
	mCrystalSensor->targetType = Types::CrystalID;

	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);

	mWanderBehaviour = mSteeringModule->AddNewBehaviour<AI::WanderBehaviour>();
	mArriveBehaviour = mSteeringModule->AddNewBehaviour<AI::ArriveBehaviour>();
	mFleeBehaviour = mSteeringModule->AddNewBehaviour<AI::FleeBehaviour>();

	mStateMachine->ChangeState(0);

	for (int i = 0; i < mTextureIds.size(); i++)
	{
		char name[128];
		sprintf_s(name, "carrier_%02i.png", i + 1);
		mTextureIds[i] = X::LoadTexture(name);
	}
}

void Miner::Unload()
{
	delete mStateMachine;
	mStateMachine = nullptr;
}

void Miner::Update(float deltaTime)
{
	//update perception module
	mBadGuySensor->viewRange = viewRange;
	mBadGuySensor->viewHalfAngle = viewAngle * X::Math::kDegToRad;
	mCrystalSensor->viewRange = viewRange;
	mCrystalSensor->viewHalfAngle = viewAngle * X::Math::kDegToRad;
	mPerceptionModule->Update(deltaTime);

	//update state machine
	mStateMachine->Update(deltaTime);


	//Update Position with SteeringModule
	const auto force = mSteeringModule->Calculate();
	const auto acceleration = force / mass;
	velocity += acceleration * deltaTime;
	if (X::Math::MagnitudeSqr(velocity) > 1.0f)
	{
		heading = X::Math::Normalize(velocity);
	}
	position += velocity * deltaTime;


	//Change position if crosses screen boundraries
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
}

void Miner::Render()
{
	const float angle = atan2(-heading.x, heading.y) + X::Math::kPi;
	const float percent = angle / X::Math::kTwoPi;
	const int frame = static_cast<int>(percent * mTextureIds.size()) % mTextureIds.size();
	X::DrawSprite(mTextureIds[frame], position);
}

void Miner::ChangeState(MinerStates newState)
{
	mStateMachine->ChangeState(int(newState));
}

void Miner::ShowDebug(bool debug)
{
	mWanderBehaviour->IsDebug(debug);
	mArriveBehaviour->IsDebug(debug);
	mFleeBehaviour->IsDebug(debug);
}

bool Miner::HasTarget()
{
	return !mPerceptionModule->GetMemoryRecords().empty();
}

Types Miner::GetTargetType()
{
	AI::MemoryRecord record = mPerceptionModule->GetMostImportant();

	Types type = static_cast<Types>(record.GetProperty<int>("type"));
	return type;
}

X::Math::Vector2 Miner::GetTargetPos()
{
	AI::MemoryRecord record = mPerceptionModule->GetMostImportant();
	auto pos = record.GetProperty<X::Math::Vector2>("lastSeenPosition");

	return pos;
}

void Miner::RemoveTarget()
{
	mPerceptionModule->RemoveFront();
}

void Miner::SetupWander(float radius, float distance, float jitter)
{
	mWanderBehaviour->Setup(radius, distance, jitter);
}

void Miner::SetBase(X::Math::Vector2 position)
{
	baseLocation = position;
}

void Miner::SetDestinationBase()
{
	destination = baseLocation;
}
