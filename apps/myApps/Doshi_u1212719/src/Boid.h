#pragma once
#include "ofMain.h"
#include "Structures.h"
#include <functional>
#include <deque>
#include <queue>

class BreadCrumps
{
public:
	class Boid* mBoid;
	std::deque<ofVec2f> mCrumps;
	float mSize = 5;
	int mFrequency = 25;
	int mLength = 40;
	int mCounter = 0;
	void DrawCrumps();
	void UpdateCrumps();
};

class Boid
{
	
public:

	static const ofColor mDefaultColor;

	KinematicStructure mKinematic;
	queue<KinematicStructure> mTargets;
	BreadCrumps mTrail;

	/// Created Variables to making it modular for AI behavior
	ofColor mColor = ofColor::black;
	bool mCanSteer = false;

	std::function<SteeringOutputStructure()> mSteering;

	void changeSteeringFunction(std::function<SteeringOutputStructure()>& iFunction);

	virtual void DrawBoid();
	virtual void UpdateBoid(float iDeltaTime);

	void setColor(ofColor color);

	void SetBoidPosition(float iX, float iY);
	void SetBoidPosition(ofVec2f iPosition);

	void SetTarget(KinematicStructure iTarget);
	void SetTargets(queue<KinematicStructure> targets);

	Boid();
	Boid(ofVec2f iPosition);
	Boid(ofVec2f iPosition, ofColor iColor);
	~Boid();
};


class AIBoid : public Boid
{
public:
	float mEnergy = 100.0f;

	AIBoid() : Boid() {};
	AIBoid(ofVec2f iPosition) : Boid(iPosition) {};
	AIBoid(ofVec2f iPosition, ofColor iColor) : Boid(iPosition, iColor) {};
	
	virtual void DrawBoid();
};
