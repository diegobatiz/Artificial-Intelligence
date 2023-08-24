#include "Peon.h"
#include "TypeIDs.h"

extern float wanderJitter;
extern float wanderRadius;
extern float wanderDistance;


Peon::Peon(AI::AIWorld& world)
	: Agent(world, Types::PeonId)
{
}

void Peon::Load()
{
	mSteeringModule = std::make_unique<AI::SteeringModule>(*this);
	mSeekBehaviour = mSteeringModule->AddNewBehaviour<AI::SeekBehaviour>();
	mWanderBehaviour = mSteeringModule->AddNewBehaviour<AI::WanderBehaviour>();
	mFleeBehaviour = mSteeringModule->AddNewBehaviour<AI::FleeBehaviour>();
	
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
	mFleeBehaviour->IsDebug(debug);
	mWanderBehaviour->IsDebug(debug);
	mSeekBehaviour->IsDebug(debug);
}
