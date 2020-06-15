#include "DynamicPathFollow.h"
#include "DynamicArrive.h"

float DynamicPathFollow::mMaxVel = 100;
float DynamicPathFollow::mMaxAccel = 20;
float DynamicPathFollow::mSmoothingRadius = 45;
float DynamicPathFollow::mSlowRadius = 125;
float DynamicPathFollow::mTargetRadius = 15;
float DynamicPathFollow::mTargetTime = 1;

SteeringOutputStructure DynamicPathFollow::GetSteering(KinematicStructure iPlayer, queue<KinematicStructure>& iPath, float iSmoothingRadius, float iMaxVel, float iMaxAccel, float iSlowRadius, float iTargetRadius, float iTargetTime)
{
	if (!iPath.empty())
	{
		auto currentTarget = iPath.front();
		auto d = iPlayer.mPosition - currentTarget.mPosition;
		if (d.length() < iSmoothingRadius)
		{
			iPath.pop();
		}
		if (!iPath.empty())
		{
			return DynamicArrive::GetSteering(iPlayer, currentTarget);
		}
	}

	iPlayer.mVelocity = ofVec2f(0.0f, 0.0f);
	iPlayer.mRotation = 0;
	return SteeringOutputStructure();
	//else
	//{
	//	return DynamicArrive::GetSteering(iPlayer, iPlayer, iMaxVel, iMaxAccel, iSlowRadius, iTargetRadius, iTargetTime);
	//}
}
