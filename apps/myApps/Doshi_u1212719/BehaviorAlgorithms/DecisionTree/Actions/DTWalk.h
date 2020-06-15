#pragma once

#include "DTActionBase.h"
#include "Structures.h"

class DTWalk : public DTActionBase
{
public:
	DTWalk();
	~DTWalk();

	virtual bool CanInterrupt() override;
	virtual bool IsActionCompleted() override;
	virtual void Execute(float dt) override;
};

class DTWander : public DTActionBase
{
public:
	DTWander();
	~DTWander();

	virtual bool IsActionCompleted() override;
	virtual void Execute(float dt) override;
};