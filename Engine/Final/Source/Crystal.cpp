#include "Crystal.h"
#include "TypeIDs.h"

Crystal::Crystal(AI::AIWorld& world)
	: AI::Entity(world, Types::CrystalID)
{
}

void Crystal::Initialize()
{
	mTextureId = X::LoadTexture("minerals.png");

	const float screenWidth = X::GetScreenWidth();
	const float screenHeight = X::GetScreenHeight();
	position = X::RandomVector2({ 100.0f, 100.0f }, { screenWidth - 100.0f, screenHeight - 100.0f });
}

void Crystal::Render()
{
	X::DrawSprite(mTextureId, position);
}