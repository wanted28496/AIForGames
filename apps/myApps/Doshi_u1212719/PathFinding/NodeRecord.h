#pragma once
#include "ofApp.h"
#include "Graph.h"
#include <vector>
#include <algorithm>
#include <queue>
#include <list>
class NodeRecord
{
public:

	NodeRecord* mParent;
	int mCostSoFar, mTotalCost, mHuerstic;
	int mNodeID;

	bool operator() (const NodeRecord* lhs, const NodeRecord* rhs) {
		return (lhs->mTotalCost > rhs->mTotalCost);
	}

	NodeRecord();
	NodeRecord(int iNodeID, NodeRecord* iParent, int iCost, int iHueristic) :
		mParent(iParent), mNodeID(iNodeID), mCostSoFar(iCost), mHuerstic(iHueristic) 
	{
		mTotalCost = mCostSoFar + mHuerstic;
	}
	~NodeRecord();

	vector<int> CreatePathFromDestination();
	void ChangeValues(int iCost, int iHueristic = 0);
	
};

template <class T, class S, class C>
S& Container(priority_queue<T, S, C>& q) {
	struct HackedQueue : private priority_queue<T, S, C>
	{
		static S& Container(priority_queue<T, S, C>& q)
		{
			return q.*&HackedQueue::c;
		}
	};
	return HackedQueue::Container(q);
}

