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