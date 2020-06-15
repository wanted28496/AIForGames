#include "DTSleep.h"
#include "Boid.h"
#include "DynamicAlign.h"
#include "DynamicArrive.h"


DTSleep::DTSleep()
{
	mPriority = 11;
}


DTSleep::~DTSleep()
{
}

bool DTSleep::CanBeInterrupted()
{
	return false;
}

bool DTSleep::CanInterrupt()
{
	return true;
}

bool DTSleep::IsActionCompleted()
{
	if (mCharacter->mEnergy >= 90)
	{
		mCharacter->setColor(ofColor::aquamarine);
		return true;
	} 

	return false;
}

void DTSleep::Execute(float dt)
{
	float r = mCharacter->mEnergy / 100.0f;
	mCharacter->setColor(ofColor(20.f * r, 60.f * r, 120.f * r));
	mCharacter->mKinematic.mVelocity = ofVec2f(0, 0);
	if (mCharacter->mEnergy <= 100)
	{
		mCharacter->mEnergy += dt * 25;
	}
	else
	{
		mCharacter->mEnergy = 100;
	}
}
