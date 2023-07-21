#pragma once

namespace AI
{
	class GridBasedGraph
	{
	public:
		enum Direction
		{
			North,
			South,
			East,
			West,
			NorthEast,
			NorthWest,
			SouthEast,
			SouthWest
		};

		struct Node
		{
			std::array<Node*, 8> neighbours = {};
			int column = 0;
			int row = 0;

			//search parameters
			Node* parent = nullptr;
			bool opened = false;
			bool closed = false;
			float cost = 0;

			void Reset()
			{
				parent = nullptr;
				opened = false;
				closed = false;
				cost = 0;
			}
		};

		void Initialize(int columns, int row);

		void ResetSearchParams();

		Node* GetNode(int x, int y);
		const Node* GetNode(int x, int y) const;

		int GetColumns() const { return mColumns; }
		int GetRows() const { return mRows; }

	private:
		int GetIndex(int x, int y) const;

		std::vector<Node> mNodes;
		int mColumns = 0;
		int mRows = 0;
	};
}