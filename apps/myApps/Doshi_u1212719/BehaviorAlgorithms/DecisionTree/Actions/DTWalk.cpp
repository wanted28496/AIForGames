#include "DTWalk.h"
#include "Boid.h"
#include "DynamicAlign.h"
#include "DynamicArrive.h"
#include "DynamicWander.h"

DTWalk::DTWalk()
{
	mPriority = 5;
	mExpiryTime = 1;
}


DTWalk::~DTWalk()
{
}

bool DTWalk::CanInterrupt()
{
	return true;
}

bool DTWalk::IsActionCompleted()
{
	auto dis = mCharacter->mTargets.front().mPosition - mCharacter->mKinematic.mPosition;
	if (dis.length() <= 50)
	{
		cout << "-> Walking complete" << endl;
		return true;
	}

	return false;
}

void DTWalk::Execute(float dt)
{
	auto posSteering = DynamicArrive::GetSteering(mCharacter->mKinematic, mCharacter->mTargets.front());
	auto rotSterring = DynamicLookWhereYouAreGoing::GetSteering(mCharacter->mKinematic);

	mCharacter->mKinematic.UpdateDynamicMotion(posSteering + rotSterring, dt);
	mCharacter->mEnergy -= dt * 10;
}

DTWander::DTWander()
{
	mExpiryTime = 1;
}

DTWander::~DTWander()
{
}

bool DTWander::IsActionCompleted()
{
	return false;
}

void DTWander::Execute(float dt)
{
	auto steering = DynamicWander::GetSteering(mCharacter->mKinematic);
	mCharacter->mKinematic.UpdateDynamicMotion(steering, dt);
	mCharacter->mEnergy -= dt * 10;
}
