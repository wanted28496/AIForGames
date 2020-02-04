#pragma once
#include "ofMain.h"
#include "Structures.h"

class DynamicArrive
{
public:

	KinematicStructure mCharacter;
	KinematicStructure mTarget;

	float mMaxSpeed;

	float mMaxAccel;

	float mTargetRadius;

	float mSlowRadius;

	float mTimeToTarget;

public:
	float GetNewOrientation(float iOrientation, ofVec2f iVelocity);

	SteeringOutputStructure getSteering();

	void UpdateKinematic(float iTime, SteeringOutputStructure iSteering);
	
	DynamicArrive();
	DynamicArrive(float iMaxSpeed, float iMaxAccel, float iTargetRadius, float iSlowRadius, float iTimeToTarget);
	~DynamicArrive();
};

