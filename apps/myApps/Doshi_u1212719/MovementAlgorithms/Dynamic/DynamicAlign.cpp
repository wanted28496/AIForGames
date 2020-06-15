#include "DynamicAlign.h"


float DynamicAlign::mAngularVel = 5;
float DynamicAlign::mAngularAccel = 10;
float DynamicAlign::mSlowRadius = 0.7;
float DynamicAlign::mTargetRadius = 0.001;
float DynamicAlign::mTargetTime = 0.3;

SteeringOutputStructure DynamicAlign::GetSteering(KinematicStructure iPlayer, KinematicStructure iTarget, float iMaxAngularVel, float iMaxAngularAccel, float iSlowRadius, float iTargetRadius, float iTargetTIme)
{
	SteeringOutputStructure steering;

	float finalOrientation = MapToRange(iTarget.mOrientation - iPlayer.mOrientation);
	int sign = (finalOrientation >= 0) ? 1 : -1;
	float absoluteOrientation = abs(finalOrientation);

	float vel;
	if (absoluteOrientation < iTargetRadius)
	{
		vel = 0;
	}
	else if (absoluteOrientation > iSlowRadius)
	{
		vel = sign * iMaxAngularVel;
	}
	else
	{
		vel = sign * (iMaxAngularVel * absoluteOrientation / iSlowRadius);
	}

	steering.mAngular = (vel - iPlayer.mRotation) / iTargetTIme;

	steering.mAngular = std::max(-iMaxAngularAccel, std::min(iMaxAngularAccel, steering.mAngular));

	return steering;
}

SteeringOutputStructure DynamicLookWhereYouAreGoing::GetSteering(KinematicStructure iPlayer)
{
	if (iPlayer.mVelocity.length() == 0)
	{
		return SteeringOutputStructure();
	}
	else
	{
		KinematicStructure target;
		target.mOrientation = atan2f(iPlayer.mVelocity.y, iPlayer.mVelocity.x);
		return DynamicAlign::GetSteering(iPlayer, target);
	}
}

SteeringOutputStructure DynamicFace::GetSteering(KinematicStructure iPlayer, KinematicStructure iTarget)
{
	auto directionVector = iTarget.mPosition - iPlayer.mPosition;
	if (directionVector.length() == 0)
	{
		return SteeringOutputStructure();
	}
	else
	{
		iTarget.mOrientation = atan2f(directionVector.y, directionVector.x);
		return DynamicAlign::GetSteering(iPlayer, iTarget);
	}
}
