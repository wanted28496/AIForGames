#pragma once
#include "ofMain.h"

#define Width 1024
#define Height 768

/*
	- Output Structure that would be returned at every frame
	- @member mLinear a vector 3 that has changes to velocity or accelration
	- @member mAngular a float that has changes to rotation
*/
struct SteeringOutputStructure
{
	ofVec2f mLinear;
	float mAngular;
};

/*
	- A structure to store the data structure of kinematic data of character
	- @member mPosition a vector 3 that stores the position of character
	- @member mOrientation a float that stores the orientation of character
	- @member mVelocity a vector 3 that stores the velocity of character
	- @member mRotation a float that stores the roation of character
*/
struct KinematicStructure
{
	ofVec2f mPosition;
	float mOrientation = 0;
	ofVec2f mVelocity;
	float mRotation = 0;

	void operator=(KinematicStructure iRHS);
	bool operator==(KinematicStructure iRHS);
	KinematicStructure()
	{
		mPosition = ofVec2f(0, 0);
		mVelocity = ofVec2f(0, 0);
		mOrientation = 0;
		mRotation = 0;
	}

};

struct MovementAlgorithms
{
	static KinematicStructure UpdateKinematic(KinematicStructure iObject, float iDeltaTime, SteeringOutputStructure iSteering);

	static float GetNewOrientation(float iOrientation, ofVec2f iVelocity);
	static SteeringOutputStructure KinematicSeek(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxVel);
	static SteeringOutputStructure KinematicArrive(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxVel, float iTargetRadius, float iTimeToTarget);
	static SteeringOutputStructure KinematicWander(KinematicStructure iCharacter, float iMaxSpeed, float iMaxRotation);
	static KinematicStructure UpdateDynamic(KinematicStructure iObject, float iDeltaTime, SteeringOutputStructure iSteering, float iMaxSpeed);
	static SteeringOutputStructure DynamicArrive(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxVel, float maxAccel, float iTargetRadius, float slowRadius, float iTimeToTarget);
	static SteeringOutputStructure DynamicFlee(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxAccel);
	static SteeringOutputStructure DynamicEvade(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxAccel, float iPersonalRadius);
	static SteeringOutputStructure DynamicSepration(KinematicStructure iCharacter, std::vector< KinematicStructure> iTarget, float iMaxAccel, float iPersonalRadius, int index);
	static SteeringOutputStructure Allign(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxRotation, float iMaxAngular, float iSlowAngle, float iTargetAngle, float iTime);
	static SteeringOutputStructure LookWhereYouAreGoing(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxRotation, float iMaxAngular, float iSlowAngle, float iTargetAngle, float iTime);
	static SteeringOutputStructure DynamicFace(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxRotation, float iMaxAngular, float iSlowAngle, float iTargetAngle, float iTime);
	static SteeringOutputStructure DynamicWander(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxRotation, float iMaxAngular, float iSlowAngle, float iTargetAngle, float iTime, float iWanderOffset, float iWanderRadius, float iWanderRate, float iWanderOrientation, float iMaxAccel);
	static SteeringOutputStructure VelocityMatch(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxAccel, float iTime);

	static std::vector<SteeringOutputStructure> NormalFlock(std::vector< KinematicStructure> iTarget, float iMaxAccel, float iTime, float iRadius, float iMaxRotation);
	static std::vector <SteeringOutputStructure> LeaderFlock(std::vector< KinematicStructure> iTarget, float iMaxAccel, float iTime, float iRadius, float iMaxRotation, int ileaderIndex);

};

