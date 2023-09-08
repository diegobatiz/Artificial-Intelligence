#include <XEngine.h> 
#include <ImGui/Inc/imgui.h>
#include <AI.h>
#include "Source/Spaceship.h"

//--------------------------------------------------
std::vector<std::unique_ptr<Spaceship>> ships;

AI::AIWorld aiWorld;

bool showDebug = false;
float wanderJitter = 5.0f;
float wanderRadius = 20.0f;
float wanderDistance = 50.0f;

float viewRange = 300.0f;
float viewAngle = 45.0f;

int activeBehaviour = 0;

void SpawnShip()
{
	auto& ship = ships.emplace_back(std::make_unique<Spaceship>(aiWorld));
	ship->Load();
	ship->ShowDebug(showDebug);

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	ship->position = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f, screenHeight - 100.0f });
}

void DestroyShip()
{
	auto& ship = ships.back();
	ship->Unload();
	ships.pop_back();
}

void GameInit()
{
	aiWorld.Initialize();
	SpawnShip();

	aiWorld.AddObstacle({ 230.0f, 300.0f, 50.0f });

	X::Math::Vector2 topLeft(500.0f, 100.0f);
	X::Math::Vector2 topRight(600.0f, 100.0f);
	X::Math::Vector2 bottomLeft(500.0f, 600.0f);
	X::Math::Vector2 bottomRight(600.0f, 600.0f);
	aiWorld.AddWall({ topLeft, topRight });
	aiWorld.AddWall({ topLeft, bottomLeft });
	aiWorld.AddWall({ topRight, bottomRight });
	aiWorld.AddWall({ bottomRight, bottomLeft });
}

bool GameLoop(float deltaTime)
{
	ImGui::Begin("Steering Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Spawn"))
	{
		SpawnShip();
	}
	if (ImGui::Button("Kill"))
	{
		DestroyShip();
	}
	if (ImGui::Checkbox("ShowDebug", &showDebug))
	{
		for (auto& ship : ships)
		{
			ship->ShowDebug(showDebug);
		}
	}

	const char* behaviours[] =
	{
		"Wander",
		"Seek"
	};
	if (ImGui::Combo("ActiveBehviour##", &activeBehaviour, behaviours, std::size(behaviours)))
	{
		for (auto& ship : ships)
		{
			ship->SetSeek(activeBehaviour == 1);
			ship->SetWander(activeBehaviour == 0);
		}
	}
	if (ImGui::CollapsingHeader("Wander##Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Jitter##", &wanderJitter, 0.1f, 0.1f, 10.0f);
		ImGui::DragFloat("Radius##", &wanderRadius, 0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("Distance##", &wanderDistance, 0.1f, 0.1f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Visual##Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("View##Range", &viewRange, 1.0f, 100.0f, 1000.0f);
		ImGui::DragFloat("View##Angle", &viewAngle, 1.0f, 10.0f, 180.0f);
	}

	ImGui::End();

	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		const auto mouseX = static_cast<float>(X::GetMouseScreenX());
		const auto mouseY = static_cast<float>(X::GetMouseScreenY());
		const auto destination = X::Math::Vector2(mouseX, mouseY);
		for (auto& ship : ships)
		{
			ship->destination = destination;
		}
	}

	aiWorld.Update();

	for (auto& ship : ships)
	{
		ship->Update(deltaTime);
	}

	for (auto& ship : ships)
	{
		ship->Render();
	}

	auto& obstacles = aiWorld.GetObstacles();
	for (auto& obstacle : obstacles)
	{
		X::DrawScreenCircle(obstacle.center, obstacle.radius, X::Colors::Gray);
	}
	auto& walls = aiWorld.GetWalls();
	for (auto& wall : walls)
	{
		X::DrawScreenLine(wall.from, wall.to, X::Colors::Gray);
	}

	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{
	for (auto& ship : ships)
	{
		ship->Unload();
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