#include "BT.h"
#include "BTTick.h"
#include "BTAction.h"
#include "BTBlackboard.h"


BT::BT(unsigned int iNodeID, BTTaskBase * iRootNode, BTBlackboard * iBlackboard) : mNodeID(iNodeID), mRootNode(iRootNode), mBlackboard(iBlackboard)
{
	unsigned int i = 0;
	SetTask(mRootNode, i);
}

void BT::SetTask(BTTaskBase * iTask, unsigned int & id)
{
	iTask->SetNodeID(id);
	id++;

	auto& children = iTask->GetChildren();
	for (int i = 0; i < children.size(); i++)
	{
		SetTask(children[i], id);
	}
}

void BT::Update(float dt)
{
	BTTick* tick = new BTTick(this, mBlackboard);
	mRootNode->Run(tick);
	
}

BT::~BT()
{
}
