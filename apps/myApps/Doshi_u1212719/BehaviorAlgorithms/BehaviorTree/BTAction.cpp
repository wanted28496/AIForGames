#include "BTAction.h"
#include "BT.h"
#include "BTTick.h"
#include "BTBlackboard.h"
#include <string>


BTTaskBase::BTTaskBase()
{
}

BTTaskBase::~BTTaskBase()
{
}

void BTTaskBase::OnEnter(BTTick * iTick)
{
}

void BTTaskBase::OnOpen(BTTick * iTick)
{
}

void BTTaskBase::OnClose(BTTick * iTick)
{
}

void BTTaskBase::OnExit(BTTick * iTick)
{
}

BTStatus BTTaskBase::OnExecute(BTTick * iTick)
{
	return BTStatus();
}

void BTTaskBase::Enter(BTTick * iTick)
{
	OnEnter(iTick);
}

void BTTaskBase::Open(BTTick * iTick)
{
	OnOpen(iTick);
	iTick->ExecuteTask(this);
	iTick->mBlackboard->SetValueInBlackboard(openString, new bool(true), iTick->mTree->mNodeID, mNodeID);
}

void BTTaskBase::Close(BTTick * iTick)
{
	OnClose(iTick);
	iTick->TerminateTask(this);
	iTick->mBlackboard->SetValueInBlackboard(openString, new bool(false), iTick->mTree->mNodeID, mNodeID);
}

void BTTaskBase::Exit(BTTick * iTick)
{
	OnExit(iTick);
}

BTStatus BTTaskBase::Execute(BTTick * iTick)
{
	return OnExecute(iTick);
}

BTStatus BTTaskBase::Run(BTTick * iTick)
{
	Enter(iTick);

	bool* isNodeOpen = (bool*)(iTick->mBlackboard->GetValueFromBlackboard(openString, iTick->mTree->mNodeID, mNodeID));

	if (!isNodeOpen || !(*isNodeOpen))
	{
		Open(iTick);
	}

	auto status = Execute(iTick);

	if (status != Running)
	{
		Close(iTick);
	}

	return status;

}

vector<BTTaskBase*>& BTTaskBase::GetChildren()
{
	return mChildren;
}

void BTTaskBase::AddChild(BTTaskBase * iChild)
{
	mChildren.push_back(iChild);
}

void BTTaskBase::SetNodeID(unsigned int iNodeID)
{
	mNodeID = iNodeID;
}

unsigned int BTTaskBase::GetNodeID()
{
	return mNodeID;
}

void BTSequencer::OnOpen(BTTick * iTick)
{
	iTick->mBlackboard->SetValueInBlackboard(runningString, new int(0), iTick->mTree->mNodeID, mNodeID);
}

BTStatus BTSequencer::OnExecute(BTTick * iTick)
{
	int* currentChild = (int*)iTick->mBlackboard->GetValueFromBlackboard(runningString, iTick->mTree->mNodeID, mNodeID);

	for (int i = (currentChild ? *currentChild : 0); i < mChildren.size(); i++)
	{
		auto status = mChildren[i]->Run(iTick);
		if (status == Success)
		{
			continue;
		}
		else if (status == Running)
		{
			iTick->mBlackboard->SetValueInBlackboard(runningString, new int(i), iTick->mTree->mNodeID, mNodeID);
		}

		return status;
	}

	return Success;
}

void BTSelector::OnOpen(BTTick * iTick)
{
	iTick->mBlackboard->SetValueInBlackboard(runningString, new int(0), iTick->mTree->mNodeID, mNodeID);
}

BTStatus BTSelector::OnExecute(BTTick * iTick)
{
	int* currentChild = (int*)iTick->mBlackboard->GetValueFromBlackboard(runningString, iTick->mTree->mNodeID, mNodeID);

	for (int i = (currentChild ? *currentChild : 0); i < mChildren.size(); i++)
	{
		auto status = mChildren[i]->Run(iTick);
		if (status == Failure)
		{
			continue;
		}
		else if (status == Running)
		{
			iTick->mBlackboard->SetValueInBlackboard(runningString, new int(i), iTick->mTree->mNodeID, mNodeID);
		}

		return status;
	}

	return Failure;
}

BTStatus BTRepeatUntillFail::OnExecute(BTTick * iTick)
{
	auto status = mChildren[0]->Run(iTick);

	switch (status)
	{
		case Running:
		case Success:
			return Running;
		case Failure:
			return Success;
		case Error:
			return Error;

	}
}

BTStatus BTCondition::Run(BTTick * iTick)
{
	Enter(iTick);
	bool* isNodeOpen = (bool*)(iTick->mBlackboard->GetValueFromBlackboard(openString, iTick->mTree->mNodeID, mNodeID));

	if (!isNodeOpen || !(*isNodeOpen))
	{
		Open(iTick);
	}

	auto status = Execute(iTick);

	if (status == Success)
	{
		status = mChildren[0]->Run(iTick);
	}

	if (status != Running)
	{
		Close(iTick);
	}

	return status;
}
