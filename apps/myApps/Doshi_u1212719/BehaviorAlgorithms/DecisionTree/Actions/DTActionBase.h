#pragma once
class DTActionBase
{
public:

	class AIBoid* mCharacter;

	float mQueuedTime = 0.0f;
	float mExpiryTime = 10.0f;
	unsigned int mPriority = 1;

	friend struct ActionList;

	virtual bool CanRunParrallelWith(DTActionBase* action);
	
	virtual bool CanInterrupt();
	virtual bool CanInterruptAction(DTActionBase* action);

	virtual bool CanBeInterrupted();
	virtual bool CanBeInterruptedByAction(DTActionBase* action);

	virtual bool IsActionCompleted();
	virtual void Execute(float dt);

	DTActionBase();
	~DTActionBase();
};

struct ActionList
{
	bool operator() (const DTActionBase& a, const DTActionBase& b) const {
		return (a.mPriority > b.mPriority);
	}
};
