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
	ofRotateZ(mCharacter.mOrientation);
	ofDrawSphere(mCharacter.mPosition.x, mCharacter.mPosition.y, radius);
	ofDrawTriangle(mCharacter.mPosition.x + diff, mCharacter.mPosition.y - diff, mCharacter.mPosition.x + diff, mCharacter.mPosition.y + diff, mCharacter.mPosition.x + lastPoint, mCharacter.mPosition.y);
	ofRotateZ(-mCharacter.mOrientation);
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

void Boid::SetBoidLocation(ofVec2f iPosition)
{
	mCharacter.mPosition = iPosition;
}

void Boid::SetBoidOrientation(float iOrientation)
{
	mCharacter.mOrientation = iOrientation;
}

Boid::Boid()
{
}


Boid::~Boid()
{
}
