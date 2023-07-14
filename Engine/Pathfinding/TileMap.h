#pragma once

#include <XEngine.h>

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

private:
	std::vector<int> mMap;
	std::vector<Tile> mTiles; 
	int mColumns = 0;
	int mRows = 0;
};