#pragma once
#include "Structures.h"

class DynamicAlign
{
	static float mAngularVel;
	static float mAngularAccel;

	static float mSlowRadius;
	static float mTargetRadius;
	static float mTargetTime;

public:
	static SteeringOutputStructure GetSteering(KinematicStructure iPlayer, KinematicStructure iTarget, float iMaxAngularVel = mAngularVel, float iMaxAngularAccel = mAngularAccel, float iSlowRadius = mSlowRadius, float iTargetRadius = mTargetRadius, float iTargetTIme = mTargetTime);
};

class DynamicLookWhereYouAreGoing
{
public:
	static SteeringOutputStructure GetSteering(KinematicStructure iPlayer);
};

class DynamicFace
{
public:
	static SteeringOutputStructure GetSteering(KinematicStructure iPlayer, KinematicStructure iTarget);
};

