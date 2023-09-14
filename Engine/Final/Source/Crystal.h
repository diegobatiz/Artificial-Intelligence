#pragma once

#include <XEngine.h>
#include <AI.h>

class Crystal : public AI::Entity
{
public:
	Crystal(AI::AIWorld& world);

	void Initialize();
	void Render();

private:
	X::TextureId mTextureId;
};