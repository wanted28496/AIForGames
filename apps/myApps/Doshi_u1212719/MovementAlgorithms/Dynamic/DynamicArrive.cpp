#include "DynamicArrive.h"

float DynamicArrive::mMaxVel = 100;
float DynamicArrive::mMaxAccel = 20;
float DynamicArrive::mSlowRadius = 175;
float DynamicArrive::mTargetRadius = 45;
float DynamicArrive::mTargetTime = 1;

SteeringOutputStructure DynamicArrive::GetSteering(KinematicStructure iPlayer, KinematicStructure iTarget, float iMaxVel, float iMaxAccel, float iSlowRadius, float iTargetRadius, float iTargetTime)
{
	SteeringOutputStructure steering;

	auto distanceVector = iTarget.mPosition - iPlayer.mPosition;
	auto distanceLength = distanceVector.length();

	float vel;

	if (distanceLength < iTargetRadius)
	{
		vel = 0;
	}
	else if (distanceLength > iSlowRadius)
	{
		vel = iMaxVel;
	}
	else
	{
		vel = iMaxVel * distanceLength / iSlowRadius;
	}

	auto targetVelocity = distanceVector.normalize() * vel;

	steering.mLinear = (targetVelocity - iPlayer.mVelocity) / iTargetTime;
	if (steering.mLinear.length() > iMaxAccel)
	{
		steering.mLinear = steering.mLinear.normalize() * iMaxAccel;
	}

	return steering;
}
