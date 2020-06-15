#pragma once

#include "DTActionBase.h"

#include <functional>
#include <vector>

using namespace std;

class DTNode
{

public:
	virtual DTNode* MakeDecision() = 0;
	virtual ~DTNode();
};

class DTActionNode : public DTNode
{
	DTActionNode();

public:
	template<typename T>
	static DTActionNode* Create();

	virtual DTNode* MakeDecision() {
		return this;
	}

	DTActionBase* mAction;
	

	DTActionBase* GetAction() {
		return mAction; 
	}


	virtual ~DTActionNode()
	{
		delete mAction;
	}
};

class DTDecisionNode : public DTNode
{
public:
	vector<DTNode*> mChildren;

	function<bool()> isConditionTrue;

	virtual DTNode* MakeDecision() {
		if (isConditionTrue())
		{
			return mChildren[0]->MakeDecision();
		}
		else
		{
			return mChildren[1]->MakeDecision();
		}
	}

	vector<DTNode*>& GetChildren();

	void AddChild(DTNode* iChild);
	void SetChildren(std::initializer_list<DTNode*> iChildren);

	template<typename... Args>
	void SetChildren(DTNode* iChild, Args... iArgs) {
		AddChild(iChild);
		SetChildren(iArgs...);
	}

	void SetChildren();

	virtual ~DTDecisionNode();

};

template<typename T>
inline DTActionNode* DTActionNode::Create() {
	DTActionNode* node = new DTActionNode();
	node->mAction = new T();
	return node;
}

