#pragma once
#include "DTActionBase.h"
#include "Structures.h"

#include <functional>

using namespace std;

class DTDig : public DTActionBase
{
	int mDigTime = 0;

public:
	DTDig();
	~DTDig();

	function<void()> dig;

	virtual bool CanBeInterrupted() override;
	virtual bool CanInterrupt() override;
	virtual bool IsActionCompleted() override;
	virtual void Execute(float dt) override;
};

