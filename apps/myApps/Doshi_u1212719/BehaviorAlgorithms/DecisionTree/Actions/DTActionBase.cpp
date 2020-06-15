#include "DTActionBase.h"
#include "Boid.h"


DTActionBase::DTActionBase()
{
}


DTActionBase::~DTActionBase()
{
}

bool DTActionBase::CanRunParrallelWith(DTActionBase * action)
{
	return false;
}

bool DTActionBase::CanInterrupt()
{
	return false;
}

bool DTActionBase::CanInterruptAction(DTActionBase * action)
{
	return false;
}

bool DTActionBase::CanBeInterrupted()
{
	return false;
}

bool DTActionBase::CanBeInterruptedByAction(DTActionBase * action)
{
	return false;
}

bool DTActionBase::IsActionCompleted()
{
	return false;
}

void DTActionBase::Execute(float dt)
{
}
