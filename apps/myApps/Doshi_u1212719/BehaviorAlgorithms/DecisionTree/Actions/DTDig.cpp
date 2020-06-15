#include "DTDig.h"
#include "Boid.h"


DTDig::DTDig()
{
	mPriority = 10;
	mExpiryTime = 2.0;
}


DTDig::~DTDig()
{
}

bool DTDig::CanBeInterrupted()
{
	return false;
}

bool DTDig::CanInterrupt()
{
	return true;
}

bool DTDig::IsActionCompleted()
{
	if (mDigTime == 101)
	{
		cout << "-> Dig complete" << endl;
		mDigTime = 0;
		mCharacter->setColor(ofColor::aquamarine);
		return true;
	}

	return false;
}

void DTDig::Execute(float dt)
{
	mDigTime++;

	if (mDigTime < 100)
	{
		mCharacter->setColor(ofColor::brown);
		mCharacter->mKinematic.mVelocity = ofVec2f(0.0f, 0.0f);
		mCharacter->mKinematic.mRotation = 0;
	}
	else if (mDigTime == 100)
	{
		dig();

		mCharacter->mEnergy -= 10;
		while (!mCharacter->mTargets.empty())
		{
			mCharacter->mTargets.pop();
		}
		mDigTime = 101;
	}
}
