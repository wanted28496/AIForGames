#include "Graph.h"



Graph::Graph()
{
}

Graph::Graph(vector<DWEdge> iEdges) : mEdges(iEdges)
{
	SortDirectWeightedEdges();
}


Graph::~Graph()
{
}

void Graph::SortDirectWeightedEdges()
{
	sort(mEdges.begin(), mEdges.end(), [](DWEdge lhs, DWEdge rhs) { return lhs.mSource < rhs.mSource; });
}

int Graph::GetCost(vector<int> iPath)
{
	int cost = 0;
	for (size_t i = 0; i < iPath.size() - 1; i++)
	{
		auto nextEdge = find_if(mEdges.begin(), mEdges.end(), 
			[iPath, i](DWEdge edge) { return edge.mSource == iPath[i] && edge.mSink == iPath[i + 1]; });
		cost += nextEdge->mCost;
	}
	return cost;
}

vector<DWEdge> Graph::GetOutgoingEdgesFromSource(int iNode)
{
	vector<DWEdge> outEdges;
	/// Finds the first sink edge
	for (size_t i = 0; i < mEdges.size(); i++)
	{
		if (mEdges[i].mSource == iNode)
		{
			/// From First sink edge find all the other sink edges
			for (size_t j = i; j < mEdges.size() && mEdges[j].mSource == iNode; j++)
			{
				outEdges.push_back(mEdges[j]);
			}
			break;
		}
	}
	return outEdges;
}
