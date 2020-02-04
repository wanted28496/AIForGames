#include "DynamicArrive.h"



float DynamicArrive::GetNewOrientation(float iOrientation, ofVec2f iVelocity)
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

SteeringOutputStructure DynamicArrive::getSteering()
{
	SteeringOutputStructure steering = SteeringOutputStructure();
	float distance = mCharacter.mPosition.distance(mTarget.mPosition);
	float factor;
	if(distance > mSlowRadius)
	{
		factor = 1;
	} else if(distance < mTargetRadius)
	{
		factor = 0;
	} else
	{
		factor = distance / mSlowRadius;
	}

	steering.mLinear = mTarget.mPosition - mCharacter.mPosition;
	steering.mLinear /= mTimeToTarget;

	if(steering.mLinear.length() > mMaxAccel)
	{

	}
	steering.mLinear *= factor;


	return SteeringOutputStructure();
}

void DynamicArrive::UpdateKinematic(float iTime, SteeringOutputStructure iSteering)
{
}

DynamicArrive::DynamicArrive()
{
}

DynamicArrive::DynamicArrive(float iMaxSpeed, float iMaxAccel, float iTargetRadius, float iSlowRadius, float iTimeToTarget) :
	mMaxSpeed(iMaxSpeed), mMaxAccel(iMaxAccel), mTargetRadius(iTargetRadius), mSlowRadius(iSlowRadius), mTimeToTarget(iTimeToTarget)
{
	mCharacter = KinematicStructure();
	mCharacter.mPosition = ofVec2f(50, 50);
}


DynamicArrive::~DynamicArrive()
{
}
