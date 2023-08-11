#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>
#include "Student.h"

//--------------------------------------------------
Student student;


void GameInit()
{
	student.Initialize();
}

bool GameLoop(float deltaTime)
{

	student.Update(deltaTime);

	ImGui::Begin("FSM");
	student.DebugUI();
	ImGui::End();


	const bool quit = X::IsKeyPressed(X::Keys::ESCAPE);
	return quit;
}

void GameCleanup()
{

}

//--------------------------------------------------

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	X::Start("FSM");
	GameInit();

	X::Run(GameLoop);

	GameCleanup();
	X::Stop();
	return 0;
}