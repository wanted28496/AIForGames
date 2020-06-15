#pragma once
#include "ofMain.h"
#include "Structures.h"

class KinematicSeek
{
public:

	KinematicStructure mCharacter;
	KinematicStructure mTarget;

	float mMaxSpeed;

public:

	float GetNewOrientation(float iOrientation, ofVec2f iVelocity);

	SteeringOutputStructure getSteering();

	void UpdateKinematic(float iTime, SteeringOutputStructure iSteering, float iMaxSpeed);

	KinematicSeek();
	KinematicSeek(float iMaxSpeed);
	~KinematicSeek();
};

