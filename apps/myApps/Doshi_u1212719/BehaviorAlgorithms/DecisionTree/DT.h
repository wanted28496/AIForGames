#pragma once
#include "DTNode.h"

class DT
{

	DTNode* mRootNode;

public:

	DTActionBase* GetAction() {
		return ((DTActionNode*)(mRootNode->MakeDecision()))->GetAction();
	}

	DT(DTNode* iRootNode);
	~DT();
};

