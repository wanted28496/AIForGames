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
	KinematicStructure mKinematic;
	queue<KinematicStructure> mTargets;
	BreadCrumps mTrail;

	std::function<SteeringOutputStructure()> mSteering;

	virtual void DrawBoid();
	virtual void UpdateBoid(float iDeltaTime);

	void SetBoidPosition(float iX, float iY);
	void SetBoidPosition(ofVec2f iPosition);

	void SetTarget(KinematicStructure iTarget);
	void SetTaregts(queue<KinematicStructure> targets);

	Boid();
	Boid(ofVec2f position);
	~Boid();
};

