#pragma once
#include "Precompiled.h"
#include "DFS.h"

using namespace AI;

bool AI::DFS::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY)
{
	graph.ResetSearchParams();
	mOpenList.clear();
	mClosedList.clear();

	auto node = graph.GetNode(startX, startY);
	mOpenList.push_back(node);
	node->opened = true;

	bool found = false;

	//search end node
	while (!found && !mOpenList.empty())
	{
		//bfs is lifo
		node = mOpenList.back();
		mOpenList.pop_back();

		if (node->column == endX && node->row == endY)
		{
			found = true;
		}
		else
		{
			for (auto neighbour : node->neighbours)
			{
				if (neighbour == nullptr || neighbour->opened)
				{
					continue;
				}

				mOpenList.push_back(neighbour);
				neighbour->opened = true;
				neighbour->parent = node;
			}
		}

		mClosedList.push_back(node);
		node->closed = true;
	}

	return found;
}