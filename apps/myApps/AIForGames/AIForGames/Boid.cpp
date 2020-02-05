#include "Boid.h"
#include "ofMain.h"



void Boid::DrawBoid()
{
	ofSetColor(0, 0, 0);
	//float degree = ofRadToDeg();
	int radius = 20;
	int diff = (radius / 3) * 2;
	int lastPoint = radius + diff;
	ofTranslate(mCharacter.mPosition.x, mCharacter.mPosition.y);
	ofRotateZRad(mCharacter.mOrientation);
	ofDrawSphere(0, 0, radius);
	ofDrawTriangle(diff, -diff, diff, diff, lastPoint, 0);
	ofRotateZRad(-mCharacter.mOrientation);
	ofTranslate(-mCharacter.mPosition.x, -mCharacter.mPosition.y);
}

void Boid::DrawBreadCrumps()
{
	ofSetColor(205, 150, 130);

	for each(ofVec2f var in breadCrumps)
	{
		ofDrawSphere(var.x, var.y, 10);
	}
}

void Boid::InitBreadCrumps()
{
	breadCrumps.push_back(mCharacter.mPosition);
}

void Boid::ClearBreadCrumps()
{
	breadCrumps.clear();
}

KinematicStructure Boid::GetBoidKinematic()
{
	return mCharacter;
}

void Boid::SetBoidKinematic(KinematicStructure iValue)
{
	mCharacter = iValue;
}

void Boid::SetBoidLocation(ofVec2f iPosition)
{
	mCharacter.mPosition = iPosition;
}

ofVec2f Boid::GetBoidLocation()
{
	return mCharacter.mPosition;
}

void Boid::SetBoidOrientation(float iOrientation)
{
	mCharacter.mOrientation = iOrientation;
}

float Boid::GetBoidOrientation()
{
	return mCharacter.mOrientation;
}

Boid::Boid()
{
}


Boid::~Boid()
{
}
