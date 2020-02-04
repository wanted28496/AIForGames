#pragma once
#include "ofMain.h"


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
	float mOrientation;
	ofVec2f mVelocity;
	float mRotation;

};

