#pragma once
#include "Precompiled.h"
#include "AStar.h"

using namespace AI;

bool AStar::Run(GridBasedGraph& graph, int startX, int startY, int endX, int endY, GetCost getCost, GetHeuristic getHeuristic)
{
	graph.ResetSearchParams();
	mOpenList.clear();
	mClosedList.clear();

	auto node = graph.GetNode(startX, startY);
	mOpenList.push_back(node);
	node->opened = true;

	auto endNode = graph.GetNode(endX, endY);

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
					neighbour->opened = true;
					neighbour->parent = node;
					neighbour->cost = cost;
					neighbour->heuristic = getHeuristic(neighbour, endNode);
					mOpenList.push_back(neighbour);
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
			if (a->cost == b->cost)
			{
				return a->heuristic < b->heuristic;
			}
			return a->cost < b->cost;
		};
		mOpenList.sort(sortCost);
		mClosedList.push_back(node);
		node->closed = true;
	}

	return found;
}
