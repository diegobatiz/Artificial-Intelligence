#pragma once

#include <XEngine.h>
#include <AI.h>;

using Path = std::vector<X::Math::Vector2>;

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

	Path FindPathBFS(int startX, int startY, int endX, int endY);
	Path FindPathDijkstra(int startX, int startY, int endX, int endY);

	int GetColumns() const { return mColumns; }
	int GetRows() const { return mRows; }

	X::Math::Vector2 GetPixelPosition(int x, int y) const;

private:
	AI::GridBasedGraph mGraph;
	std::vector<int> mMap;
	std::vector<Tile> mTiles; 
	int mColumns = 0;
	int mRows = 0;
	int mTileWidth = 0;
	int mTileHeight = 0;
};