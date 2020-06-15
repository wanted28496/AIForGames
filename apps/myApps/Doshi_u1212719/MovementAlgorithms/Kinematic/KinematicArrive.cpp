#include "KinematicArrive.h"



float KinematicArrive::GetNewOrientation(float iOrientation, ofVec2f iVelocity)
{
	if (iVelocity.length() > 0)
	{
		return atan2f(iVelocity.y, iVelocity.x);
	}
	else
	{
		return iOrientation;
	}
}

SteeringOutputStructure KinematicArrive::getSteering()
{
	SteeringOutputStructure steering = SteeringOutputStructure();
	float distance = mCharacter.mPosition.distance(mTarget.mPosition);
	if (distance <= mTargetRadius)
	{
		mCharacter.mVelocity = ofVec2f(0, 0);
		steering.mLinear = ofVec2f(0, 0);
		steering.mAngular = 0;
		return steering;
	}
	else
	{
		steering.mLinear = mTarget.mPosition - mCharacter.mPosition;
		steering.mLinear /= mTimeToTarget;

		if (steering.mLinear.length() > mMaxSpeed)
		{
			steering.mLinear.normalize();
			steering.mLinear *= mMaxSpeed;
		}
		return steering;
	}

}

void KinematicArrive::UpdateKinematic(float iTime, SteeringOutputStructure iSteering)
{
	mCharacter.mVelocity += iSteering.mLinear * iTime;
	mCharacter.mRotation += iSteering.mAngular * iTime;

	if (mCharacter.mVelocity.length() > mMaxSpeed)
	{
		mCharacter.mVelocity.normalize();
		mCharacter.mVelocity *= mMaxSpeed;
	}
	
	mCharacter.mPosition += mCharacter.mVelocity * iTime;
	mCharacter.mOrientation += mCharacter.mRotation * iTime;
}

KinematicArrive::KinematicArrive()
{
}

KinematicArrive::KinematicArrive(float iMaxSpeed, float iTargetRadius, float iTimeToTarget) : 
	mMaxSpeed(iMaxSpeed), mTimeToTarget(iTimeToTarget), mTargetRadius(iTargetRadius)
{
	mCharacter = KinematicStructure();
	mCharacter.mPosition = ofVec2f(50, 50);
}


KinematicArrive::~KinematicArrive()
{
}