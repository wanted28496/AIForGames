#pragma once
#include <vector>
#include <functional>

using namespace std;

const string openString = "Open";
const string runningString = "Running";
const string closedString = "Close";
const string targetString = "TargetFound";

enum BTStatus
{
	Success,
	Failure,
	Running,
	Error
};

class BTTick;

class BTTaskBase
{
public:
	unsigned int mNodeID;
	vector< BTTaskBase*> mChildren;

	BTTaskBase();
	~BTTaskBase();

	virtual void OnEnter(BTTick* iTick);
	virtual void OnOpen(BTTick* iTick);
	virtual void OnClose(BTTick* iTick);
	virtual void OnExit(BTTick* iTick);

	virtual BTStatus OnExecute(BTTick* iTick) = 0;

	void Enter(BTTick* iTick);
	void Open(BTTick* iTick);
	void Close(BTTick* iTick);
	void Exit(BTTick* iTick);

	BTStatus Execute(BTTick* iTick);

	virtual BTStatus Run(BTTick* iTick);

	vector<BTTaskBase*>& GetChildren();
	void AddChild(BTTaskBase* iChild);

	void SetNodeID(unsigned int iNodeID);
	unsigned int GetNodeID();

};

class BTSequencer : public BTTaskBase
{
	virtual void OnOpen(BTTick* iTick);
	virtual BTStatus OnExecute(BTTick* iTick);
};

class BTSelector : public BTTaskBase
{
	virtual void OnOpen(BTTick* iTick);
	virtual BTStatus OnExecute(BTTick* iTick);
};

class BTRepeatUntillFail : public BTTaskBase
{
	virtual BTStatus OnExecute(BTTick* iTick);
};

class BTCondition : public BTTaskBase
{
public:
	function<BTStatus(BTTick*)> ExecuteAction;

	virtual BTStatus OnExecute(BTTick* iTick) {
		return ExecuteAction(iTick);
	}

	virtual BTStatus Run(BTTick* iTick);

};

class BTAction : public BTTaskBase
{
public:
	function<void(BTTick*)> OpenAction = [](BTTick* iTick) {};
	virtual void OnOpen(BTTick* iTick) {
		OpenAction(iTick);
	}

	function<BTStatus(BTTick*)> ExecuteAction;

	virtual BTStatus OnExecute(BTTick* iTick) {
		return ExecuteAction(iTick);
	}
};

