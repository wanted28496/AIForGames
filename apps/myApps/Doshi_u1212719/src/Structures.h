#pragma once
#include "ofMain.h"

#define Width 1000
#define Height 1000
#define PIE 3.14159f

/*
	- Output Structure that would be returned at every frame
	- @member mLinear a vector 3 that has changes to velocity or accelration
	- @member mAngular a float that has changes to rotation
*/
struct SteeringOutputStructure
{
	ofVec2f mLinear;
	float mAngular;

	SteeringOutputStructure() : mAngular(0), mLinear(ofVec2f(0,0)) {}

	SteeringOutputStructure operator +(const SteeringOutputStructure& iRhs);
	SteeringOutputStructure& operator +=(const SteeringOutputStructure& iRhs);

	SteeringOutputStructure operator *(const float& iRhs);
	SteeringOutputStructure& operator *=(const float& iRhs);
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

	class Boid* mBoid;

	KinematicStructure()
	{
		mPosition = ofVec2f(0, 0);
		mVelocity = ofVec2f(0, 0);
		mOrientation = 0;
		mRotation = 0;
	}

	void ClampVelocity();
	void ClampOrientation();
	void UpdateDynamicMotion(SteeringOutputStructure iSteering, float iDeltaTime);

};

float MapToRange(float iAngle);


