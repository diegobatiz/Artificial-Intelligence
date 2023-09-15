#include <XEngine.h> 
#include <ImGui/Inc/imgui.h>
#include <AI.h>
#include "Source/Miner.h"
#include "Source/Crystal.h"
#include "Source/BadGuy.h"

//--------------------------------------------------
std::vector<std::unique_ptr<Miner>> miners;
std::vector<std::unique_ptr<Crystal>> crystals;
std::vector<std::unique_ptr<BadGuy>> badGuys;

AI::AIWorld aiWorld;

bool showDebug = false;
float wanderJitter = 5.0f;
float wanderRadius = 20.0f;
float wanderDistance = 50.0f;

float viewRange = 300.0f;
float viewAngle = 45.0f;

X::Math::Vector2 basePosition{};
X::Math::Vector2 badBasePosition{};

AI::ArriveBehaviour::Deacceleration deacceleration = AI::ArriveBehaviour::Deacceleration::Normal;

int activeBehaviour = 0;

void SpawnMiner()
{
	auto& miner = miners.emplace_back(std::make_unique<Miner>(aiWorld));
	miner->Load();
	miner->ShowDebug(showDebug);

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	miner->position = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f, screenHeight - 100.0f });
	miner->SetBase(basePosition);
}

void SpawnBadGuy()
{
	auto& badGuy = badGuys.emplace_back(std::make_unique<BadGuy>(aiWorld));
	badGuy->Load();
	badGuy->ShowDebug(showDebug);

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	badGuy->position = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f, screenHeight - 100.0f });
	badGuy->SetBase(badBasePosition);
}

void SpawnCrystal()
{
	auto& crystal = crystals.emplace_back(std::make_unique<Crystal>(aiWorld));
	crystal->Initialize();
}

void KillMiner()
{
	auto& miner = miners.back();
	miner->Unload();
	miners.pop_back();
}

void GameInit()
{
	aiWorld.Initialize();

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();

	basePosition = { 0, 0 };
	badBasePosition = { screenWidth, 0};

	SpawnMiner();
	SpawnCrystal();
	SpawnBadGuy();

	//aiWorld.AddObstacle({ 230.0f, 300.0f, 50.0f });

	/*X::Math::Vector2 topLeft(500.0f, 100.0f);
	X::Math::Vector2 topRight(600.0f, 100.0f);
	X::Math::Vector2 bottomLeft(500.0f, 600.0f);
	X::Math::Vector2 bottomRight(600.0f, 600.0f);
	aiWorld.AddWall({ topLeft, topRight });
	aiWorld.AddWall({ topLeft, bottomLeft });
	aiWorld.AddWall({ topRight, bottomRight });
	aiWorld.AddWall({ bottomRight, bottomLeft });*/
}

bool GameLoop(float deltaTime)
{
	ImGui::Begin("Steering Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("SpawnMiner"))
	{
		SpawnMiner();
	}
	if (ImGui::Button("SpawnBadGuy"))
	{
		SpawnBadGuy();
	}
	if (ImGui::Button("SpawnCrystal"))
	{
		SpawnCrystal();
	}

	if (ImGui::Checkbox("ShowDebug", &showDebug))
	{
		for (auto& miner : miners)
		{
			miner->ShowDebug(showDebug);
		}
	}

	if (ImGui::CollapsingHeader("Wander##Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Jitter##", &wanderJitter, 0.1f, 0.1f, 10.0f);
		ImGui::DragFloat("Radius##", &wanderRadius, 0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("Distance##", &wanderDistance, 0.1f, 0.1f, 100.0f);
	}

	ImGui::End();

	aiWorld.Update();

	for (auto& miner : miners)
	{
		miner->Update(deltaTime);
	}
	for (auto& badGuy : badGuys)
	{
		badGuy->Update(deltaTime);
	}

	for (auto& miner : miners)
	{
		miner->Render();
	}
	for (auto& badGuy : badGuys)
	{
		badGuy->Render();
	}

	for (auto& crystal : crystals)
	{
		crystal->Render();
	}

	/*auto& obstacles = aiWorld.GetObstacles();
	for (auto& obstacle : obstacles)
	{
		X::DrawScreenCircle(obstacle.center, obstacle.radius, X::Colors::Gray);
	}
	auto& walls = aiWorld.GetWalls();
	for (auto& wall : walls)
	{
		X::DrawScreenLine(wall.from, wall.to, X::Colors::Gray);
	}*/

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{
	for (auto& miner : miners)
	{
		miner->Unload();
	}
}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Final");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}