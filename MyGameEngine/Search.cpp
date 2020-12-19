#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>

struct GraphNode
{
	// �� ���� ���� ����� �����͵��� ������ �ִ�
	std::vector<GraphNode*> mAdjacent;
};

struct Graph
{
	// �׷����� ������ �����Ѵ�
	std::vector<GraphNode*> mNodes;
};

// ���� �׷���
struct WeightedEdge
{
	// �� ������ � ��尡 ����� �ִ°�?
	struct WeightedGraphNode* mFrom;
	struct WeightedGraphNode* mTo;
	// �� ������ ����ġ
	float mWeight;
};

struct WeightedGraphNode
{
	// �ܺη� ���ϴ� ������ �����Ѵ�
	std::vector<WeightedEdge*> mEdges;
};

// (���� �׷����� WeightedGraphNode�� ������)
struct WeightedGraph
{
	std::vector<WeightedGraphNode*> mNodes;
};

struct GBFSScratch
{
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.0f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

struct AStarScratch
{
	const WeightedEdge* mParentEdge = nullptr;
	float mHeuristic = 0.0f;
	float mActualFromStart = 0.0f;
	bool mInOpenSet = false;
	bool mInClosedSet = false;
};

using AStarMap =
std::unordered_map<const WeightedGraphNode*, AStarScratch>;

float ComputeHeuristic(const WeightedGraphNode* a, const WeightedGraphNode* b)
{
	return 0.0f;
}

using GBFSMap = 
std::unordered_map < const WeightedGraphNode*, GBFSScratch>;

bool AStar(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, AStarMap& outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	// ���� ��忡 ���۳�带 �߰��Ѵ�	
	// �׸��� ���� ���տ� ǥ���Ѵ�
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do
	{
		// ���� �������� ���� ��带 �߰��Ѵ�
		for (const WeightedEdge* edge : current->mEdges)
		{
			const WeightedGraphNode* neighbor = edge->mTo;
			// �� ����� �߰� �����͸� ��´�
			AStarScratch& data = outMap[neighbor];
			// ���� ���տ� �������� Ȯ��
			if (!data.mInClosedSet)
			{
				if (!data.mInOpenSet)
				{
					// ���� ���տ� ���ٸ� �θ�� �ݵ�� ���� ���տ� �־�� �Ѵ�
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					// ���� ����� �θ��� ���� ��� + �θ𿡼� �ڽ����� �̵��ϴ� ������ ����ġ��
					data.mActualFromStart = outMap[current].mActualFromStart + edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else
				{
					// ���� ��尡 �θ� ��尡 ������ �Ǵ��ϰ��� ���ο� ���� ����� ����Ѵ�
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart)
					{
						// ���� ��尡 �� ����� �θ� ���� ä�õ�
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}
		}

		// ���� ��尡 ����ִٸ� ���� ��尡 ����
		if (openSet.empty())
		{
			break;
		}

		// ���� ���տ��� ���� ���� ����� ���� ��带 ã�´�
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			// 
			float f0fA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
			float f0fB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
			return f0fA < f0fB;
		});
		// ���� ��带 �����ϰ� �� ��带 ���� ���տ��� ���� �������� �̵���Ų��
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = true;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	// ���� ã�Ҵ°�
	return (current == goal) ? true : false;
}

// Ž�� �ֿ켱 Ž��
// GBFS, Greedy Best-First Search
bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, GBFSMap outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	// ���� ���� �� ���� ���� ���۳���
	// �׸��� �� ��带 ���� ���տ� �ִٰ� ǥ���Ѵ�
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do
	{
		// ���� ��带 ���� ���տ� �߰��Ѵ�
		for (const WeightedEdge* edge : current->mEdges)
		{
			// �� ���� ����� �߰� �����͸� ��´�
			GBFSScratch& data = outMap[edge->mTo];
			// �� ��尡 ���� ���տ� ���ٸ� �߰��Ѵ�
			if (!data.mInClosedSet)
			{
				// ���� ����� �θ� ������ ����
				data.mParentEdge = edge;
				if (!data.mInOpenSet)
				{
					// �� ����� �޸���ƽ�� ����ϰ� ���� ���տ� �߰��Ѵ�
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}

		// ���� ������ ����ִٸ� ���� ��尡 ����
		if (openSet.empty())
		{
			break; // �������� �����
		}

		// ���� ���տ��� ���� ����� ���� ��带 ã��
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b)
		{
			return outMap[a].mHeuristic < outMap[b].mHeuristic;
		});

		// ���� ��带 �����ϰ� �� ��带 ���� ���տ��� ���� �������� �̵���Ų��
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;

	} while (current != goal);


	return (current == goal) ? true : false;
}

using NodeToParentmap = std::unordered_map<const GraphNode*, const GraphNode*>;

// �ʺ� �켱 Ž��
// BFS, Breadth-First Search
bool BFS(const Graph& graph, const GraphNode* start,
	const GraphNode* goal, NodeToParentmap& outMap)
{
	// ��θ� ã�Ҵ����� �� �� �ִ� �÷���
	bool pathFound = false;
	// ����ؾ� �� ���
	std::queue<const GraphNode*> q;
	// ���� ��带 ť�� �ִ´�
	q.emplace(start);

	while (!q.empty())
	{
		// ť���� ��带 ������
		const GraphNode* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}

		// ť���� ���� ���� ��带 ������
		for (const GraphNode* node : current->mAdjacent)
		{
			// ���� ��带 �����ϰ�
			// �θ� null�̸� ť�� ���� ���� ����
			const GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start)
			{
				// �� ����� �θ� ��带 �����ϰ� ť�� �ִ´�
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}

	return pathFound;
}