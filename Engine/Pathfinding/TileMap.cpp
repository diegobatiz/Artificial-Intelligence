#include "TileMap.h"

namespace
{
	inline int ToIndex(int x, int y, int columns)
	{
		return x + (y * columns);
	}
}

void TileMap::LoadTiles(const char* fileName)
{
	if (!std::filesystem::exists(fileName))
	{
		return;
	}

	std::fstream file(fileName);
	int count = 0; 
	std::string buffer;

	file >> buffer;
	file >> count;

	mTiles.clear();
	mTiles.reserve(count);

	for (int i = 0; i < count; i++)
	{
		int isBlocked = 0;

		file >> buffer;
		file >> isBlocked;
		auto& tile = mTiles.emplace_back();
		
		tile.textureId = X::LoadTexture(buffer.c_str());
		tile.isBlocked = isBlocked;
	}

	file.close();
}

void TileMap::LoadMap(const char* fileName)
{
	if (!std::filesystem::exists(fileName))
	{
		return;
	}

	std::fstream file(fileName);
	std::string buffer;

	file >> buffer;
	file >> mColumns;
	file >> buffer;
	file >> mRows;

	char tileType;
	while (file >> tileType)
	{
		int iTileType;
		iTileType = tileType - '0';
		mMap.push_back(iTileType);
	}

	file.close();
}

void TileMap::Render() const
{
	float spriteWidth = X::GetSpriteWidth(mTiles[0].textureId);
	float spriteHeight = X::GetSpriteHeight(mTiles[0].textureId);
	X::Math::Vector2 position;
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int index = (y * mColumns) + x;
			X::DrawSprite(mTiles[mMap[index]].textureId, position, X::Pivot::TopLeft);
			position.x += spriteWidth;
		}
		position.x = 0.0f;
		position.y += spriteHeight;
	}
}

// 2D map - 5 columns x 4 rows
// [0][0][0][0][0]
// [0][0][0][0][0]
// [0][0][0][X][0]   X is at (3, 2)
// [0][0][0][0][0]

// Stored as 1D - 5x4 = 20 slots
// [0][0][0][0][0] [0][0][0][0][0] [0][0][0][X][0] [0][0][0][0][0]
//
// index = column + (row * columnCount)
//       = 3 + (2 * 5)
//       = 13