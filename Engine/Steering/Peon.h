#pragma once

#include <AI.h>

class Peon : public AI::Agent
{
public:
	Peon(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();

	void ShowDebug(bool debug);

	void SetArrive(bool active) { mArriveBehaviour->SetActive(active); }
	void SetFlee(bool active) { mFleeBehaviour->SetActive(active); }
	void SetSeek(bool active) { mSeekBehaviour->SetActive(active); }
	void SetWander(bool active) { mWanderBehaviour->SetActive(active); }
	void SetPursuit(bool active) { mPursuitBehaviour->SetActive(active); }
	void SetEvade(bool active) { mEvadeBehaviour->SetActive(active); }

private:
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	AI::WanderBehaviour* mWanderBehaviour = nullptr;
	AI::SeekBehaviour* mSeekBehaviour = nullptr;
	AI::FleeBehaviour* mFleeBehaviour = nullptr;
	AI::ArriveBehaviour* mArriveBehaviour = nullptr;
	AI::PursuitBehaviour* mPursuitBehaviour = nullptr;
	AI::EvadeBehaviour* mEvadeBehaviour = nullptr;
	std::array<X::TextureId, 16> mTextureIds{};
};