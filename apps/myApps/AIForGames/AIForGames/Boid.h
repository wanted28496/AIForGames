#pragma once
#include "ofMain.h"
#include "src/Structures.h"

class Boid
{
	KinematicStructure mCharacter;
	std::vector<ofVec2f> breadCrumps;



public:

	void DrawBoid();
	void DrawBreadCrumps();
	void InitBreadCrumps();
	void ClearBreadCrumps();

	void SetBoidLocation(ofVec2f);
	void SetBoidOrientation(float);

	Boid();
	~Boid();
};

