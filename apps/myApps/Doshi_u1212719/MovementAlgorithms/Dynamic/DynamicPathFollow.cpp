#include "DynamicPathFollow.h"
#include "DynamicArrive.h"

float DynamicPathFollow::mMaxVel = 100;
float DynamicPathFollow::mMaxAccel = 20;
float DynamicPathFollow::mSmoothingRadius = 40;
float DynamicPathFollow::mSlowRadius = 125;
float DynamicPathFollow::mTargetRadius = 15;
float DynamicPathFollow::mTargetTime = 1;

SteeringOutputStructure DynamicPathFollow::GetSteering(KinematicStructure iPlayer, queue<KinematicStructure>& iPath, float iSmoothingRadius, float iMaxVel, float iMaxAccel, float iSlowRadius, float iTargetRadius, float iTargetTime)
{
	if (!iPath.empty())
	{
		auto currentTarget = iPath.front();
		auto d = iPlayer.mPosition - currentTarget.mPosition;
		if (iPath.size() != 1 && d.length() < iSmoothingRadius)
		{
			iPath.pop();
			currentTarget = iPath.front();
		}
		return DynamicArrive::GetSteering(iPlayer, currentTarget);
	}
	else
	{
		return DynamicArrive::GetSteering(iPlayer, iPlayer, iMaxVel, iMaxAccel, iSlowRadius, iTargetRadius, iTargetTime);
	}
}
