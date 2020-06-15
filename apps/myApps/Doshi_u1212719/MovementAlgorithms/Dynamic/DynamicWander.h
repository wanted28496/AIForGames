#pragma once

#include "Structures.h"
#include <random>

class DynamicWander
{

	static float mOffset;
	static float mRadius;

	static float mRate;
	static float mOrientation;

	static float mMaxAccel;

	static std::default_random_engine mGenerator;
	static std::binomial_distribution<int> mDistribution;

public:
	static float GetBinomial(float iMin, float iMax);
	static SteeringOutputStructure GetSteering(KinematicStructure iKinematic);
};

