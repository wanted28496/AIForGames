#pragma once
#include "AIManager.h"

class BT : public AIManager
{
public:

	unsigned int mNodeID;
	class BTTaskBase* mRootNode;
	class BTBlackboard* mBlackboard;

	BT(unsigned int iNodeID, BTTaskBase* iRootNode, BTBlackboard* iBlackboard);

	void SetTask(BTTaskBase* iTask, unsigned int& id);
	virtual void Update(float dt);

	~BT();
};

