#pragma once
#include "Structures.h"
#include <queue>

class DynamicPathFollow
{
	static float mMaxVel;
	static float mMaxAccel;
	static float mSmoothingRadius;
	static float mSlowRadius;
	static float mTargetRadius;
	static float mTargetTime;
public:
	static SteeringOutputStructure GetSteering(KinematicStructure iCharacter, queue<KinematicStructure> &iPath, float iSmoothingRadius = mSmoothingRadius, float iMaxVel = mMaxVel, float iMaxAccel = mMaxAccel, float iSlowRadius = mSlowRadius, float iTargetRadius = mTargetRadius, float iTargetTime = mTargetTime);
};

