#include <AI.h>
#include "TypeIDs.h"

enum class BadGuyStates
{
	Wandering,
	ChaseMiner,
	Returning
};

class VisualSensor;

class BadGuy : public AI::Agent
{
public:
	BadGuy(AI::AIWorld& world);

	void Load();
	void Unload();

	void Update(float deltaTime);
	void Render();
	void ChangeState(BadGuyStates newState);

	void ShowDebug(bool debug);

	void SetArrive(bool active) { mArriveBehaviour->SetActive(active); }
	void SetWander(bool active) { mWanderBehaviour->SetActive(active); }
	void SetSeek(bool active) { mSeekBehaviour->SetActive(active); }

	bool HasTarget();
	Types GetTargetType();
	X::Math::Vector2 GetTargetPos();

	void SetupWander(float radius, float distance, float jitter);
	void SetBase(X::Math::Vector2 position); // should be called when making the bad guy
	void SetDestinationBase();

	BadGuyStates prevState = BadGuyStates::Wandering;

private:
	std::unique_ptr<AI::PerceptionModule> mPerceptionModule;
	VisualSensor* mVisualSensor = nullptr;

	std::unique_ptr<AI::SteeringModule> mSteeringModule;
	AI::WanderBehaviour* mWanderBehaviour = nullptr;
	AI::ArriveBehaviour* mArriveBehaviour = nullptr;
	AI::SeekBehaviour* mSeekBehaviour = nullptr;

	AI::StateMachine<BadGuy>* mStateMachine;

	X::Math::Vector2 baseLocation;

	std::array<X::TextureId, 32> mTextureIds{};
};