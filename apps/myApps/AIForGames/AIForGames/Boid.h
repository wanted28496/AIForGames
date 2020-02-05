#pragma once
#include "ofMain.h"
#include "src/Structures.h"

class Boid
{
	
public:
	KinematicStructure mCharacter;
	std::vector<ofVec2f> breadCrumps;

	void DrawBoid();
	void DrawBreadCrumps();
	void InitBreadCrumps();
	void ClearBreadCrumps();

	KinematicStructure GetBoidKinematic();
	void SetBoidKinematic(KinematicStructure);
	void SetBoidLocation(ofVec2f);
	ofVec2f GetBoidLocation();
	void SetBoidOrientation(float);
	float GetBoidOrientation();

	Boid();
	~Boid();
};

