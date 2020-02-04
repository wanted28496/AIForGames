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
