#pragma once

#include "DTActionBase.h"
#include "Structures.h"

class DTSleep : public DTActionBase
{

public:

	DTSleep();
	~DTSleep();

	virtual bool CanBeInterrupted() override;
	virtual bool CanInterrupt() override;
	virtual bool IsActionCompleted() override;
	virtual void Execute(float dt) override;
};

