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