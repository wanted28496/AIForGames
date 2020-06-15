#pragma once
#include <list>
using namespace std;

class BTTick
{
public:

	class BT* mTree;
	class BTBlackboard* mBlackboard;
	list<class BTTaskBase*> mTasks;

	BTTick(BT* iTree, BTBlackboard* iBlackboard);
	~BTTick();

	void ExecuteTask(BTTaskBase* iTask);
	void TerminateTask(BTTaskBase* iTask);

};

