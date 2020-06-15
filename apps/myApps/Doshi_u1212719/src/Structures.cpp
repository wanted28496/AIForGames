#include "Structures.h"
#include "Boid.h"
#define decayCoefficient 10000
#define BOIDMASS 5
#define LEADERMASS 30


ofVec2f AsVector(float radian)
{
	return ofVec2f(cosf(radian), sinf(radian));
}


void KinematicStructure::operator=(KinematicStructure iRHS)
{
	mPosition = iRHS.mPosition;
	mRotation = iRHS.mRotation;
	mOrientation = iRHS.mOrientation;
	mVelocity = iRHS.mVelocity;
}

bool KinematicStructure::operator==(KinematicStructure iRHS)
{
	
	return (mPosition == iRHS.mPosition && mRotation == iRHS.mRotation && mVelocity == iRHS.mVelocity && mOrientation == iRHS.mOrientation);
}

void KinematicStructure::ClampVelocity()
{
	if (mVelocity.length() > 60)
	{
		mVelocity = mVelocity.normalize() * 60;
	}

	mRotation = std::max(-PIE, std::min(mRotation, PIE));
}

void KinematicStructure::ClampOrientation()
{
	if (mOrientation < -PIE)
	{
		mOrientation += 2 * PIE;
	}
	if (mOrientation > PIE)
	{
		mOrientation -= 2 * PIE;
	}
}

void KinematicStructure::UpdateDynamicMotion(SteeringOutputStructure iSteering, float iDeltaTime)
{
	mVelocity += iSteering.mLinear * iDeltaTime;
	mRotation += iSteering.mAngular * iDeltaTime;

	ClampVelocity();

	mPosition += mVelocity * iDeltaTime;
	mOrientation += mRotation * iDeltaTime;
}


SteeringOutputStructure SteeringOutputStructure::operator+(const SteeringOutputStructure & iRhs)
{
	SteeringOutputStructure steering;
	steering.mAngular = iRhs.mAngular + this->mAngular;
	steering.mLinear = iRhs.mLinear + this->mLinear;
	return steering;
}

SteeringOutputStructure & SteeringOutputStructure::operator+=(const SteeringOutputStructure & iRhs)
{
	this->mAngular += iRhs.mAngular;
	this->mLinear += iRhs.mLinear;
	return *this;
}

SteeringOutputStructure SteeringOutputStructure::operator*(const float & iRhs)
{
	SteeringOutputStructure steering;
	steering.mAngular = iRhs * this->mAngular;
	steering.mLinear = iRhs * this->mLinear;
	return steering;
}

SteeringOutputStructure & SteeringOutputStructure::operator*=(const float & iRhs)
{
	this->mAngular *= iRhs;
	this->mLinear *= iRhs;
	return *this;
}

float MapToRange(float iAngle)
{
	if (iAngle > PIE)
	{
		int factor = (int)((iAngle + PIE) / (2 * PIE));
		return iAngle - (factor * 2 * PIE);
	}
	else if (iAngle < -PIE)
	{
		int factor = (int)((iAngle - PIE) / (2 * PIE));
		return iAngle - (factor * 2 * PIE);
	}
	else
	{
		return iAngle;
	}
	
}
