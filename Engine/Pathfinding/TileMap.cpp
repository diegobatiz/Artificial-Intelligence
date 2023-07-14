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
	const char* mFileName = fileName;

	std::fstream inputStream;
	inputStream.open(fileName);

	inputStream >> mTileNum;

	for (int i = 0; i < mTileNum; i++)
	{
		Tile newTile;
		std::string textureStr;
		
		inputStream >> textureStr;
		newTile.textureId = X::LoadTexture(textureStr.c_str());

		inputStream >> newTile.canWalk;

		mTiles.push_back(newTile);
	}
}

void TileMap::LoadMap(const char* fileName)
{
	const char* mFileName = fileName;

	std::fstream inputStream;
	inputStream.open(fileName);

	inputStream >> mColumns;
	inputStream >> mRows;
	int tileType;

	while (inputStream >> tileType)
	{
		mMap.push_back(tileType);
	}
}

void TileMap::Render() const
{
	float spriteWidth = X::GetSpriteWidth(mTiles[0].textureId);
	float spriteHeight = X::GetSpriteHeight(mTiles[0].textureId);
	X::Math::Vector2 position;
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