#include "Boid.h"
#include "ofMain.h"

const ofColor Boid::mDefaultColor = ofColor::darkBlue;

void Boid::changeSteeringFunction(std::function<SteeringOutputStructure()>& iFunction)
{
	mSteering = iFunction;
	mCanSteer = true;
}

void Boid::DrawBoid()
{
	mTrail.DrawCrumps();
	ofSetColor(mColor);

	ofPushMatrix();

	ofTranslate(mKinematic.mPosition.x, mKinematic.mPosition.y);
	ofRotateZRad(mKinematic.mOrientation);
	ofDrawCircle(0, 0, 10.0f);
	ofDrawTriangle(0, -10.0f, 0, 10.0f, 10.0f * 2.0f, 0);

	ofPopMatrix();

}

void Boid::UpdateBoid(float iDeltaTime)
{

	if (mCanSteer)
	{
		mKinematic.UpdateDynamicMotion(mSteering(), iDeltaTime);
	}

	mTrail.UpdateCrumps();
}

void Boid::setColor(ofColor iColor)
{
	mColor = iColor;
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

void Boid::SetTargets(queue<KinematicStructure> iTargets)
{
	mTargets = iTargets;
}

Boid::Boid() : Boid(ofVec2f(10,10), mDefaultColor)
{
}

Boid::Boid(ofVec2f iPosition) : Boid(iPosition, mDefaultColor)
{
}

Boid::Boid(ofVec2f iPosition, ofColor iColor)
{
	mKinematic.mPosition = iPosition;
	mColor = iColor;
	mKinematic.mBoid = this;
	mTargets.push(mKinematic);
	mTrail.mBoid = this;
	mSteering = []() { return SteeringOutputStructure(); };
}


Boid::~Boid()
{
}

void BreadCrumps::DrawCrumps()
{
	for (auto point : mCrumps)
	{
		ofSetColor(mBoid->mColor);
		ofDrawCircle(point.x - mSize / 2, point.y - mSize / 2, mSize / 2);
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

void AIBoid::DrawBoid()
{
	mTrail.DrawCrumps();

	ofSetColor(mColor);

	ofPushMatrix();

	ofTranslate(mKinematic.mPosition.x, mKinematic.mPosition.y);
	ofRotateZRad(mKinematic.mOrientation);
	ofDrawCircle(0, 0, 10.0f);
	ofDrawTriangle(0, -10.0f, 0, 10.0f, 10.0f * 2.0f, 0);

	/// Energy Consumed each turn
	float x = mEnergy * 5.0f;
	float y = abs(tan(mEnergy * 0.3f * PI / 180)) * x;

	///Drawing Detection Radius
	ofSetColor(ofColor(255, 255, 0, 100));
	ofDrawTriangle(0, 0, x, y, x, -y);


	ofPopMatrix();
}
