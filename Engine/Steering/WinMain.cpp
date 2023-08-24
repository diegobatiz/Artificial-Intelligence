#include <XEngine.h> 
#include <ImGui/Inc/imgui.h>
#include <AI.h>
#include "Peon.h"
//--------------------------------------------------

AI::AIWorld aiWorld;
std::vector<std::unique_ptr<Peon>> peons;
Peon targetPeon(aiWorld);

bool showDebug = false;
float wanderJitter = 5.0f;
float wanderRadius = 20.0f;
float wanderDistance = 50.0f;

int activeBehaviour = 0;

AI::ArriveBehaviour::Deacceleration deacceleration = AI::ArriveBehaviour::Deacceleration::Normal;

void SpawnPeon()
{
	auto& peon = peons.emplace_back(std::make_unique<Peon>(aiWorld));
	peon->Load();
	peon->ShowDebug(showDebug);
	peon->target = &targetPeon;

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	peon->position = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f, screenHeight - 100.0f });
	}

void KillPeon()
{
	auto& peon = peons.back();
	peon->Unload();
	peons.pop_back();
}

void GameInit()
{
	aiWorld.Initialize();

	targetPeon.Load();
	targetPeon.SetWander(true);
	targetPeon.SetFlee(false);
	targetPeon.SetSeek(false);
	SpawnPeon();
}

bool GameLoop(float deltaTime)
{
	ImGui::Begin("Steering Controls", nullptr, ImGuiWindowFlags_AlwaysAutoResize);
	if (ImGui::Button("Spawn"))
	{
		SpawnPeon();
	}
	if (ImGui::Button("Kill"))
	{
		KillPeon();
	}
	if (ImGui::Checkbox("ShowDebug", &showDebug))
	{
		for (auto& peon : peons)
		{
			peon->ShowDebug(showDebug);
		}
	}


	const char* behaviours[] =
	{
		"Arrive",
		"Flee",
		"Seek",
		"Wander",
		"Pursuit",
		"Evade"
	};
	if (ImGui::Combo("ActiveBehviour##", &activeBehaviour, behaviours, std::size(behaviours)))
	{
		for (auto& peon : peons)
		{
			peon->SetArrive(activeBehaviour == 0);
			peon->SetFlee(activeBehaviour == 1);
			peon->SetSeek(activeBehaviour == 2);
			peon->SetWander(activeBehaviour == 3);
			peon->SetPursuit(activeBehaviour == 4);
			peon->SetEvade(activeBehaviour == 5);
		}
	}

	if (ImGui::CollapsingHeader("Wander##Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		ImGui::DragFloat("Jitter##", &wanderJitter, 0.1f, 0.1f, 10.0f);
		ImGui::DragFloat("Radius##", &wanderRadius, 0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("Distance##", &wanderDistance, 0.1f, 0.1f, 100.0f);
	}
	if (ImGui::CollapsingHeader("Arrive##Settings", ImGuiTreeNodeFlags_DefaultOpen))
	{
		const char* deaccelerationSpeed[] =
		{
			"Fast",
			"Normal",
			"Slow"
		};
		int deaccel = static_cast<int>(deacceleration);
		if (ImGui::Combo("Deacceleration#", &deaccel, deaccelerationSpeed, std::size(deaccelerationSpeed)))
		{
			for (auto& peon : peons)
			{
				deacceleration = static_cast<AI::ArriveBehaviour::Deacceleration>(deaccel);
			}
		}
	}

	ImGui::End();

	if (X::IsMousePressed(X::Mouse::LBUTTON))
	{
		const auto mouseX = static_cast<float>(X::GetMouseScreenX());
		const auto mouseY = static_cast<float>(X::GetMouseScreenY());
		const auto destination = X::Math::Vector2(mouseX, mouseY);
		for (auto& peon : peons)
		{ 
			peon->destination = destination;
		}
	}

	aiWorld.Update();

	for (auto& peon : peons)
	{
		peon->Update(deltaTime);
	}
	targetPeon.Update(deltaTime);
	for (auto& peon : peons)
	{
		peon->Render();
	}
	targetPeon.Render();

	
	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{
	targetPeon.Unload();
	for (auto& peon : peons)
	{
		peon->Unload();
	}
}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("Steering");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}