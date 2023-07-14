#pragma once

#include <XEngine.h>
#include <AI.h>;

struct Tile
{
	X::TextureId textureId = 0;
	bool isBlocked = false;
};

class TileMap
{
public:
	void LoadTiles(const char* fileName);
	void LoadMap(const char* fileName);

	void Render() const;

	bool IsBlocked(int x, int y) const;

private:
	X::Math::Vector2 GetPixelPosition(int x, int y) const;

	AI::GridBasedGraph mGraph;
	std::vector<int> mMap;
	std::vector<Tile> mTiles; 
	int mColumns = 0;
	int mRows = 0;
	int mTileWidth = 0;
	int mTileHeight = 0;
};