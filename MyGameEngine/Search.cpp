#include <vector>
#include <queue>
#include <iostream>
#include <unordered_map>

struct GraphNode
{
	// 각 노드는 인접 노드의 포인터들을 가지고 있다
	std::vector<GraphNode*> mAdjacent;
};

struct Graph
{
	// 그래프는 노드들을 포함한다
	std::vector<GraphNode*> mNodes;
};

// 가중 그래프
struct WeightedEdge
{
	// 이 에지에 어떤 노드가 연결돼 있는가?
	struct WeightedGraphNode* mFrom;
	struct WeightedGraphNode* mTo;
	// 이 에지의 가중치
	float mWeight;
};

struct WeightedGraphNode
{
	// 외부로 향하는 에지를 저장한다
	std::vector<WeightedEdge*> mEdges;
};

// (가중 그래프는 WeightedGraphNode를 가진다)
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

	// 현재 노드에 시작노드를 추가한다	
	// 그리고 닫힌 집합에 표시한다
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do
	{
		// 열린 집합으로 인접 노드를 추가한다
		for (const WeightedEdge* edge : current->mEdges)
		{
			const WeightedGraphNode* neighbor = edge->mTo;
			// 이 노드의 추가 데이터를 얻는다
			AStarScratch& data = outMap[neighbor];
			// 닫힌 집합에 없는지를 확인
			if (!data.mInClosedSet)
			{
				if (!data.mInOpenSet)
				{
					// 열린 집합에 없다면 부모는 반드시 열린 집합에 있어야 한다
					data.mParentEdge = edge;
					data.mHeuristic = ComputeHeuristic(neighbor, goal);
					// 실제 비용은 부모의 실제 비용 + 부모에서 자신으로 이동하는 에지의 가중치다
					data.mActualFromStart = outMap[current].mActualFromStart + edge->mWeight;
					data.mInOpenSet = true;
					openSet.emplace_back(neighbor);
				}
				else
				{
					// 현재 노드가 부모 노드가 될지를 판단하고자 새로운 실제 비용을 계산한다
					float newG = outMap[current].mActualFromStart + edge->mWeight;
					if (newG < data.mActualFromStart)
					{
						// 현재 노드가 이 노드의 부모 노드로 채택됨
						data.mParentEdge = edge;
						data.mActualFromStart = newG;
					}
				}
			}
		}

		// 열린 노드가 비어있다면 평가할 노드가 없다
		if (openSet.empty())
		{
			break;
		}

		// 열린 집합에서 가장 낮은 비용을 가진 노드를 찾는다
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b) {
			// 
			float f0fA = outMap[a].mHeuristic + outMap[a].mActualFromStart;
			float f0fB = outMap[b].mHeuristic + outMap[b].mActualFromStart;
			return f0fA < f0fB;
		});
		// 현재 노드를 설정하고 이 노드를 열린 집합에서 닫힌 집합으로 이동시킨다
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = true;
		outMap[current].mInClosedSet = true;
	} while (current != goal);

	// 길을 찾았는가
	return (current == goal) ? true : false;
}

// 탐욕 최우선 탐색
// GBFS, Greedy Best-First Search
bool GBFS(const WeightedGraph& g, const WeightedGraphNode* start,
	const WeightedGraphNode* goal, GBFSMap outMap)
{
	std::vector<const WeightedGraphNode*> openSet;

	// 최초 시작 시 현재 노드는 시작노드다
	// 그리고 이 노드를 닫힌 집합에 있다고 표시한다
	const WeightedGraphNode* current = start;
	outMap[current].mInClosedSet = true;

	do
	{
		// 인접 노드를 열린 집합에 추가한다
		for (const WeightedEdge* edge : current->mEdges)
		{
			// 이 인접 노드의 추가 데이터를 얻는다
			GBFSScratch& data = outMap[edge->mTo];
			// 이 노드가 닫힌 집합에 없다면 추가한다
			if (!data.mInClosedSet)
			{
				// 인접 노드의 부모 에지를 설정
				data.mParentEdge = edge;
				if (!data.mInOpenSet)
				{
					// 이 노드의 휴리스틱을 계산하고 열린 집합에 추가한다
					data.mHeuristic = ComputeHeuristic(edge->mTo, goal);
					data.mInOpenSet = true;
					openSet.emplace_back(edge->mTo);
				}
			}
		}

		// 열린 집합이 비어있다면 평가할 노드가 없다
		if (openSet.empty())
		{
			break; // 루프에서 벗어난다
		}

		// 열린 집합에서 가장 비용이 낮은 노드를 찾자
		auto iter = std::min_element(openSet.begin(), openSet.end(),
			[&outMap](const WeightedGraphNode* a, const WeightedGraphNode* b)
		{
			return outMap[a].mHeuristic < outMap[b].mHeuristic;
		});

		// 현재 노드를 설정하고 이 노드를 열린 집합에서 닫힌 집합으로 이동시킨다
		current = *iter;
		openSet.erase(iter);
		outMap[current].mInOpenSet = false;
		outMap[current].mInClosedSet = true;

	} while (current != goal);


	return (current == goal) ? true : false;
}

using NodeToParentmap = std::unordered_map<const GraphNode*, const GraphNode*>;

// 너비 우선 탐색
// BFS, Breadth-First Search
bool BFS(const Graph& graph, const GraphNode* start,
	const GraphNode* goal, NodeToParentmap& outMap)
{
	// 경로를 찾았는지를 알 수 있는 플래그
	bool pathFound = false;
	// 고려해야 될 노드
	std::queue<const GraphNode*> q;
	// 시작 노드를 큐에 넣는다
	q.emplace(start);

	while (!q.empty())
	{
		// 큐에서 노드를 꺼낸다
		const GraphNode* current = q.front();
		q.pop();
		if (current == goal)
		{
			pathFound = true;
			break;
		}

		// 큐에는 없는 인접 노드를 꺼낸다
		for (const GraphNode* node : current->mAdjacent)
		{
			// 시작 노드를 제외하고
			// 부모가 null이면 큐에 넣지 않은 노드다
			const GraphNode* parent = outMap[node];
			if (parent == nullptr && node != start)
			{
				// 이 노드의 부모 노드를 설정하고 큐에 넣는다
				outMap[node] = current;
				q.emplace(node);
			}
		}
	}

	return pathFound;
}