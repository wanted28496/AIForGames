#include "Boid.h"
#include "ofMain.h"



void Boid::DrawBoid()
{
	mTrail.DrawCrumps();
	ofSetColor(ofColor::darkBlue);
	ofPushMatrix();
	ofTranslate(mKinematic.mPosition.x, mKinematic.mPosition.y);
	ofRotateZRad(mKinematic.mOrientation);
	ofDrawCircle(0, 0, 13.0f);
	ofDrawTriangle(0, -13.0f, 0, 13.0f, 13.0f * 2.0f, 0);
	ofPopMatrix();
	//float degree = ofRadToDeg();
	/*int radius = 20;
	int diff = (radius / 3) * 2;
	int lastPoint = radius + diff;
	if(mCharacter.mPosition.x > Width + radius)
	{
		mCharacter.mPosition.x = 0 - radius;
	} else if(mCharacter.mPosition.x < 0 - radius)
	{
		mCharacter.mPosition.x = Width + radius;
	}
	if(mCharacter.mPosition.y > Height + radius)
	{
		mCharacter.mPosition.y = 0 - radius;
	} else if(mCharacter.mPosition.y < 0 - radius)
	{
		mCharacter.mPosition.y = Height + radius;
	}
	ofTranslate(mCharacter.mPosition.x, mCharacter.mPosition.y);
	ofRotateZRad(mCharacter.mOrientation);
	ofDrawSphere(0, 0, radius);
	ofDrawTriangle(diff, -diff, diff, diff, lastPoint, 0);
	ofRotateZRad(-mCharacter.mOrientation);
	ofTranslate(-mCharacter.mPosition.x, -mCharacter.mPosition.y);*/
}

void Boid::UpdateBoid(float iDeltaTime)
{
	mKinematic.UpdateDynamicMotion(mSteering(), iDeltaTime);
	mTrail.UpdateCrumps();
}

void Boid::SetBoidPosition(float iX, float iY)
{
	mKinematic.mPosition = ofVec2f(iX, iY);
}

void Boid::SetBoidPosition(ofVec2f iPosition)
{
	mKinematic.mPosition = iPosition;
}

void Boid::SetTarget(KinematicStructure iTarget)
{
	mTargets.push(iTarget);
}

void Boid::SetTaregts(queue<KinematicStructure> iTargets)
{
	mTargets = iTargets;
}

Boid::Boid() : Boid(ofVec2f(10,10))
{
}

Boid::Boid(ofVec2f iPosition)
{
	mKinematic.mPosition = iPosition;
	mKinematic.mBoid = this;
	mTargets.push(mKinematic);
	mTrail.mBoid = this;
}


Boid::~Boid()
{
}

void BreadCrumps::DrawCrumps()
{
	for (auto point : mCrumps)
	{
		ofSetColor(ofColor::darkBlue);
		ofDrawCircle(point.x - mSize / 2, point.y - mSize / 2, mSize / 2);
		//ofDrawRectangle(point.x - mSize / 2, point.y - mSize / 2, mSize, mSize);
	}
}

void BreadCrumps::UpdateCrumps()
{
	if (mCounter >= mFrequency)
	{
		if (mCrumps.size() > mLength)
		{
			mCrumps.pop_front();
		}
		mCrumps.push_back(mBoid->mKinematic.mPosition);
		mCounter = 0;
	}
	else
	{
		mCounter++;
	}
}
