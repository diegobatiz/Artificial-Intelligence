#pragma once
#include "Dijkstra.h"
#include "Precompiled.h"

using namespace AI;

bool AI::Dijkstra::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY, GetCost getCost)
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
				if (neighbour == nullptr || neighbour->closed)
				{
					continue;
				}
				int cost = node->cost + getCost(node, neighbour);
				if (!neighbour->opened)
				{
					mOpenList.push_back(neighbour);
					neighbour->opened = true;
					neighbour->parent = node;
					neighbour->cost = cost;
				}
				else if (cost < neighbour->cost)
				{
					neighbour->parent = node;
					neighbour->cost = cost;
				}
			}
		}
		auto sortCost = [&](const GridBasedGraph::Node* a, const GridBasedGraph::Node* b)
		{
			return a->cost < b->cost;
		};
		std::sort(mOpenList.begin(), mOpenList.end(), sortCost);
		mClosedList.push_back(node);
		node->closed = true;
	}

	return found;
}
