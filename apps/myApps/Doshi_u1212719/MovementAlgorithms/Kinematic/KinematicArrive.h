#pragma once
#include "ofMain.h"
#include "Structures.h"


class KinematicArrive
{
public:

	KinematicStructure mCharacter;
	KinematicStructure mTarget;

	float mMaxSpeed;

	float mTargetRadius;

	float mTimeToTarget;

public:

	float GetNewOrientation(float iOrientation, ofVec2f iVelocity);

	SteeringOutputStructure getSteering();

	void UpdateKinematic(float iTime, SteeringOutputStructure iSteering);

	KinematicArrive();
	KinematicArrive(float iMaxSpeed, float iTargetRadius, float iTimeToTarget);
	~KinematicArrive();
};

