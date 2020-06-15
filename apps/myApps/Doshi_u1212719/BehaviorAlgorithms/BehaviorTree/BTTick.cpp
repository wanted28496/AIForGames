#include "BTTick.h"
#include "BT.h"
#include "BTBlackboard.h"
#include "BTAction.h"


BTTick::BTTick(BT * iTree, BTBlackboard * iBlackboard) : mTree(iTree), mBlackboard(iBlackboard)
{
}

BTTick::~BTTick()
{
}

void BTTick::ExecuteTask(BTTaskBase * iTask)
{
	mTasks.remove(iTask);
}

void BTTick::TerminateTask(BTTaskBase * iTask)
{
	mTasks.push_back(iTask);
}
