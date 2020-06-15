#include "NodeRecord.h"



NodeRecord::NodeRecord()
{
}


NodeRecord::~NodeRecord()
{
}

vector<int> NodeRecord::CreatePathFromDestination()
{
	/// Using List to push into front of list as we are going in reverse order
	list<int> path;
	NodeRecord* i = this;
	while (i != nullptr)
	{
		path.push_front(i->mNodeID);
		i = i->mParent;
	}
	return vector<int>(path.begin(), path.end());
}

void NodeRecord::ChangeValues(int iCost, int iHueristic)
{
	mCostSoFar = iCost;
	mHuerstic = iHueristic;
	mTotalCost = iCost + iHueristic;
}
