#include <XEngine.h> // <> for external includes, "" for internal includes
#include <ImGui/Inc/imgui.h>

int startX = 0;
int startY = 0;
int endX = 0;
int endY = 0;

//--------------------------------------------------

void GameInit()
{
}

bool GameLoop(float deltaTime)
{
	ImGui::Begin("FSM");
	
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