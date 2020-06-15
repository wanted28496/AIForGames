#include "BTBlackboard.h"



BTBlackboard::BTBlackboard()
{
}


BTBlackboard::~BTBlackboard()
{
	ResetBlackboard();
}

void BTBlackboard::SetValueInBlackboard(string iKey, void * iValue, unsigned int iTreeID, int iNodeID)
{
	if (mTreeData[iTreeID][iNodeID][iKey])
	{
		delete mTreeData[iTreeID][iNodeID][iKey];
	}

	mTreeData[iTreeID][iNodeID][iKey] = iValue;
}

void * BTBlackboard::GetValueFromBlackboard(string iKey, unsigned int iTreeID, int iNodeID)
{
	return mTreeData[iTreeID][iNodeID][iKey];
}

void BTBlackboard::AddNodeToRoute(Tracer iNode)
{
	mRoute.push_back(iNode);
}

vector<Tracer> BTBlackboard::GetRoute()
{
	return mRoute;
}

void BTBlackboard::ResetBlackboard()
{
	for (auto& treeData : mTreeData)
	{
		for (auto& nodeData : treeData.second)
		{
			for (auto& value : nodeData.second)
			{
				delete value.second;
				value.second = nullptr;
			}
		}
	}
}
