#include "TileMap.h"

using namespace AI;

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

	mTileWidth = X::GetSpriteWidth(mTiles[0].textureId);
	mTileHeight = X::GetSpriteHeight(mTiles[0].textureId);
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

	auto GetNeighbour = [&](int c, int r)->GridBasedGraph::Node*
	{
		if (IsBlocked(c, r))
		{
			return nullptr;
		}

		return mGraph.GetNode(c, r);
	};

	mGraph.Initialize(mColumns, mRows);
	for (int r = 0; r < mRows; ++r)
	{
		for (int c = 0; c < mColumns; c++)
		{
			if (IsBlocked(c, r))
			{
				continue;
			}

			GridBasedGraph::Node* node = mGraph.GetNode(c, r);
			node->neighbours[GridBasedGraph::Direction::North] = GetNeighbour(c, r - 1);
			node->neighbours[GridBasedGraph::Direction::South] = GetNeighbour(c, r + 1);
			node->neighbours[GridBasedGraph::Direction::East] = GetNeighbour(c + 1, r);
			node->neighbours[GridBasedGraph::Direction::West] = GetNeighbour(c - 1, r);
			node->neighbours[GridBasedGraph::Direction::NorthEast] = GetNeighbour(c + 1, r - 1);
			node->neighbours[GridBasedGraph::Direction::NorthWest] = GetNeighbour(c - 1, r - 1);
			node->neighbours[GridBasedGraph::Direction::SouthEast] = GetNeighbour(c + 1, r + 1);
			node->neighbours[GridBasedGraph::Direction::SouthWest] = GetNeighbour(c - 1, r + 1);
		}
	}
}

void TileMap::Render() const
{
	X::Math::Vector2 position;
	for (int y = 0; y < mRows; y++)
	{
		for (int x = 0; x < mColumns; x++)
		{
			int index = (y * mColumns) + x;
			X::DrawSprite(mTiles[mMap[index]].textureId, position, X::Pivot::TopLeft);
			position.x += mTileWidth;
		}
		position.x = 0.0f;
		position.y += mTileHeight;
	}

	for (int r = 0; r < mRows; r++)
	{
		for (int c = 0; c < mColumns; c++)
		{
			const auto node = mGraph.GetNode(c, r);
			for (const auto neighbour : node->neighbours)
			{
				if (neighbour == nullptr)
				{
					continue;
				}
				const auto a = GetPixelPosition(node->column, node->row);
				const auto b = GetPixelPosition(neighbour->column, neighbour->row);
				X::DrawScreenLine(a, b, X::Colors::Blue);
			}
		}
	}
}

bool TileMap::IsBlocked(int x, int y) const
{
	if (x >= 0 && x < mColumns && y >= 0 && y < mRows)
	{
		const int index = ToIndex(x, y, mColumns);
		if (index < mMap.size())
		{
			int tile = mMap[index];
			return mTiles[tile].isBlocked;
		}
	}

	return true;
}

Path TileMap::FindPathBFS(int startX, int startY, int endX, int endY)
{
	Path path;
	BFS bfs;
	if (bfs.Run(mGraph, startX, startY, endX, endY))
	{
		const auto& closedList = bfs.GetClosedList();
		auto node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathDFS(int startX, int startY, int endX, int endY)
{
	Path path;
	DFS dfs;
	if (dfs.Run(mGraph, startX, startY, endX, endY))
	{
		const auto& closedList = dfs.GetClosedList();
		auto node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathDijkstra(int startX, int startY, int endX, int endY)
{
	Path path;
	Dijkstra dijkstra;
	auto getCost = [](const GridBasedGraph::Node* node, const GridBasedGraph::Node* neighbour)
	{
		if (node->column != neighbour->column && node->row != neighbour->row)
		{
			return 1.5f;
		}

		return 1.0f;
	};

	if (dijkstra.Run(mGraph, startX, startY, endX, endY, getCost))
	{
		const auto& closedList = dijkstra.GetClosedList();
		auto node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathAStarManhattan(int startX, int startY, int endX, int endY)
{
	Path path;
	AStar AStar;

	auto getCost = [](const GridBasedGraph::Node* node, const GridBasedGraph::Node* neighbour)
	{
		if (node->column != neighbour->column && node->row != neighbour->row)
		{
			return 1.5f;
		}

		return 1.0f;
	};

	auto getHeuristic = [](const GridBasedGraph::Node* neighbour, const GridBasedGraph::Node* endNode)
	{
		float D = 1.0f;
		float dx = abs(neighbour->column - endNode->column);
		float dy = abs(neighbour->row - endNode->row);

		return D * (dx + dy);
	};

	if (AStar.Run(mGraph, startX, startY, endX, endY, getCost, getHeuristic))
	{
		const auto& closedList = AStar.GetClosedList();
		auto node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathAStarDiagonal(int startX, int startY, int endX, int endY)
{
	Path path;
	AStar AStar;

	auto getCost = [](const GridBasedGraph::Node* node, const GridBasedGraph::Node* neighbour)
	{
		if (node->column != neighbour->column && node->row != neighbour->row)
		{
			return 1.5f;
		}

		return 1.0f;
	};

	auto getHeuristic = [](const GridBasedGraph::Node* neighbour, const GridBasedGraph::Node* endNode)
	{
		float D = 1.0f;
		float D2 = 1.5f;
		float dx = abs(neighbour->column - endNode->column);
		float dy = abs(neighbour->row - endNode->row);

		return D * std::max(dx, dy) + (D2 - D) * std::min(dx, dy);
	};

	if (AStar.Run(mGraph, startX, startY, endX, endY, getCost, getHeuristic))
	{
		const auto& closedList = AStar.GetClosedList();
		auto node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

Path TileMap::FindPathAStarEuclidean(int startX, int startY, int endX, int endY)
{
	Path path;
	AStar AStar;

	auto getCost = [](const GridBasedGraph::Node* node, const GridBasedGraph::Node* neighbour)
	{
		if (node->column != neighbour->column && node->row != neighbour->row)
		{
			return 1.5f;
		}

		return 1.0f;
	};

	auto getHeuristic = [](const GridBasedGraph::Node* neighbour, const GridBasedGraph::Node* endNode)
	{
		float D = 1.0f;
		float dx = abs(neighbour->column - endNode->column);
		float dy = abs(neighbour->row - endNode->row);

		return D * std::sqrt(dx * dx + dy * dy);
	};

	if (AStar.Run(mGraph, startX, startY, endX, endY, getCost, getHeuristic))
	{
		const auto& closedList = AStar.GetClosedList();
		auto node = closedList.back();
		while (node != nullptr)
		{
			path.push_back(GetPixelPosition(node->column, node->row));
			node = node->parent;
		}
		std::reverse(path.begin(), path.end());
	}
	return path;
}

X::Math::Vector2 TileMap::GetPixelPosition(int x, int y) const
{
	return
	{
		(x + 0.5f) * mTileWidth,
		(y + 0.5f) * mTileHeight
	};
}
