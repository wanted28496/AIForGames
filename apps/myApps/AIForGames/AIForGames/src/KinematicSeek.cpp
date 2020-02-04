#include "KinematicSeek.h"
#include "Structures.h"



float KinematicSeek::GetNewOrientation(float iOrientation, ofVec2f iVelocity)
{
	if (iVelocity.length() > 0)
	{
		return atan2f(iVelocity.y, iVelocity.x);
	}
	else
	{
		return iOrientation;
	}
	return 0.0f;
}

SteeringOutputStructure KinematicSeek::getSteering()
{
	SteeringOutputStructure steering = SteeringOutputStructure();

	steering.mLinear = mTarget.mPosition - mCharacter.mPosition;
	
	steering.mLinear.normalize();

	steering.mLinear *= mMaxSpeed;

	mCharacter.mOrientation = GetNewOrientation(mCharacter.mOrientation, steering.mLinear);

	steering.mAngular = 0;

	return steering;
}

void KinematicSeek::UpdateKinematic(float iTime, SteeringOutputStructure iSteering, float iMaxSpeed)
{
	mCharacter.mVelocity += iSteering.mLinear * iTime;
	mCharacter.mRotation += iSteering.mAngular * iTime;

	mCharacter.mVelocity.normalize();
	mCharacter.mVelocity *= iMaxSpeed;

	mCharacter.mPosition += mCharacter.mVelocity * iTime;
	mCharacter.mOrientation += mCharacter.mRotation * iTime;

}

KinematicSeek::KinematicSeek()
{
	mCharacter = KinematicStructure();
	mCharacter.mPosition = ofVec2f(50, 50);
	mTarget = KinematicStructure();
	mTarget.mPosition = ofVec2f(600, 100);
}

KinematicSeek::KinematicSeek(float iMaxSpeed)
{
	mCharacter = KinematicStructure();
	mCharacter.mPosition = ofVec2f(50, 50);
	mTarget = KinematicStructure();
	mTarget.mPosition = ofVec2f(950, 50);
	mMaxSpeed = iMaxSpeed;
}


KinematicSeek::~KinematicSeek()
{
}
