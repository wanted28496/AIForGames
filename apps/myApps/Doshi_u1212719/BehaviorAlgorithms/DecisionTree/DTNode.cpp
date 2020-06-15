#include "DTNode.h"

DTActionNode::DTActionNode()
{}

DTNode::~DTNode()
{
}

vector<DTNode*>& DTDecisionNode::GetChildren()
{
	return mChildren;
}

void DTDecisionNode::AddChild(DTNode * iChild)
{
	mChildren.push_back(iChild);
}

void DTDecisionNode::SetChildren(std::initializer_list<DTNode*> iChildren)
{
	for (auto child : iChildren)
	{
		mChildren.push_back(child);
	}
}

void DTDecisionNode::SetChildren()
{
}

DTDecisionNode::~DTDecisionNode()
{
	for (auto child : mChildren)
	{
		delete child;
	}
}
