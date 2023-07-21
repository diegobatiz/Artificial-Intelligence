#pragma once
#include "Precompiled.h"
#include "BFS.h"

using namespace AI;

bool AI::BFS::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY)
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
		//bfs is fifo
		node = mOpenList.front();
		mOpenList.pop_front();

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
