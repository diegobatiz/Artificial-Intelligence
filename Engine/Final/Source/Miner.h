#pragma once

#include <AI.h>
#include "TypeIDs.h"

enum class MinerStates
{
	Wandering,
	SeekingMine,
	Mining,
	Returning,
	Fleeing
};

class VisualSensor;

class Miner : public AI::Agent
{
public:
	Miner(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();
	void ChangeState(MinerStates newState);

	void ShowDebug(bool debug);

	void SetArrive(bool active) { mArriveBehaviour->SetActive(active); }
	void SetWander(bool active) { mWanderBehaviour->SetActive(active); }
	void SetFlee(bool active) { mFleeBehaviour->SetActive(active); }

	bool HasTarget();
	Types GetTargetType();
	X::Math::Vector2 GetTargetPos();
	void RemoveTarget();

	void SetupWander(float radius, float distance, float jitter); 
	void SetBase(X::Math::Vector2 position); // should be called when making the miner
	void SetDestinationBase();

	bool hasCrystal;
	MinerStates prevState = MinerStates::Wandering;

private:
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	VisualSensor* mBadGuySensor = nullptr;
	VisualSensor* mCrystalSensor = nullptr;

	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	AI::WanderBehaviour* mWanderBehaviour = nullptr;
	AI::ArriveBehaviour* mArriveBehaviour = nullptr;
	AI::FleeBehaviour* mFleeBehaviour = nullptr;

	AI::StateMachine<Miner>* mStateMachine;

	X::Math::Vector2 baseLocation;

	std::array<X::TextureId, 32> mTextureIds{};
};