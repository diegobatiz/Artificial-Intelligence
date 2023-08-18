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

private:
	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	AI::WanderBehaviour* mWanderBehaviour = nullptr;
	AI::SeekBehaviour* mSeekBehaviour = nullptr;
	AI::FleeBehaviour* mFleeBehaviour = nullptr;
	std::array<X::TextureId, 16> mTextureIds{};
};