#pragma once
#include "ofMain.h"
#include "Structures.h"

class DynamicArrive
{
	static float mMaxVel;
	static float mMaxAccel;
	static float mSlowRadius;
	static float mTargetRadius;
	static float mTargetTime;
public:
	static SteeringOutputStructure GetSteering(KinematicStructure iPlayer, KinematicStructure iTarget, float iMaxVel = mMaxVel, float iMaxAccel = mMaxAccel, float iSlowRadius = mSlowRadius, float iTargetRadius = mTargetRadius, float iTargetTime = mTargetTime);
};

