#include "Structures.h"


void KinematicStructure::operator=(KinematicStructure iRHS)
{
	mPosition = iRHS.mPosition;
	mRotation = iRHS.mRotation;
	mOrientation = iRHS.mOrientation;
	mVelocity = iRHS.mVelocity;
}


KinematicStructure MovementAlgorithms::UpdateKinematic(KinematicStructure iObject, float iDeltaTime, SteeringOutputStructure iSteering)
{
	iObject.mVelocity = iSteering.mLinear * iDeltaTime;
	iObject.mRotation = iSteering.mAngular * iDeltaTime;

	iObject.mPosition += iObject.mVelocity * iDeltaTime;
	iObject.mOrientation += iObject.mRotation * iDeltaTime;
	return iObject;
}

float MovementAlgorithms::GetNewOrientation(float iOrientation, ofVec2f iVelocity)
{
	if(iVelocity.length() > 0)
	{
		return atan2f(iVelocity.y, iVelocity.x);
	} else
	{
		return iOrientation;
	}
}

SteeringOutputStructure MovementAlgorithms::KinematicSeek(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxVel)
{
	SteeringOutputStructure steering = SteeringOutputStructure();

	steering.mLinear = iTarget.mPosition - iCharacter.mPosition;

	steering.mLinear.normalize();

	steering.mLinear *= iMaxVel;

	steering.mAngular = 0;

	return steering;
}

SteeringOutputStructure MovementAlgorithms::KinematicArrive(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxVel, float iTargetRadius, float iTimeToTarget)
{
	SteeringOutputStructure steering = SteeringOutputStructure();
	float distance = iCharacter.mPosition.distance(iTarget.mPosition);
	if(distance <= iTargetRadius)
	{
		steering.mLinear = ofVec2f(0, 0);
		steering.mAngular = 0;

	} else
	{
		steering.mLinear = iTarget.mPosition - iCharacter.mPosition;
		steering.mLinear /= iTimeToTarget;

		if(steering.mLinear.length() > iMaxVel)
		{
			steering.mLinear.normalize();
			steering.mLinear *= iMaxVel;
		}
	}
	return steering;
}

KinematicStructure MovementAlgorithms::UpdateDynamic(KinematicStructure iObject, float iDeltaTime, SteeringOutputStructure iSteering, float iMaxSpeed)
{
	iObject.mVelocity += iSteering.mLinear * iDeltaTime;
	iObject.mRotation += iSteering.mAngular * iDeltaTime;

	if(iObject.mVelocity.length() > iMaxSpeed)
	{
		iObject.mVelocity.normalize();
		iObject.mVelocity *= iMaxSpeed;
	}

	iObject.mPosition += iObject.mVelocity * iDeltaTime;
	iObject.mOrientation += iObject.mRotation * iDeltaTime;
	return iObject;
}


SteeringOutputStructure MovementAlgorithms::DynamicArrive(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxVel, float iMaxAccel, float iTargetRadius, float iSlowRadius, float iTimeToTarget)
{
	SteeringOutputStructure steering = SteeringOutputStructure();
	auto direction = iTarget.mPosition - iCharacter.mPosition;
	auto distance = direction.length();

	if(distance < iTargetRadius)
	{
		steering.mAngular = 0;
		steering.mLinear = ofVec2f(0, 0);
	}
	float targetSpeed = 0;
	if(distance > iSlowRadius)
	{
		targetSpeed = iMaxVel;
	} else
	{
		targetSpeed = (iMaxVel * distance) / iSlowRadius;
	}

	auto targetVelocity = direction;
	targetVelocity.normalize();
	targetVelocity *= targetSpeed;

	steering.mLinear = targetVelocity - iCharacter.mVelocity;
	steering.mLinear /= iTimeToTarget;

	if(steering.mLinear.length() > iMaxAccel)
	{
		steering.mLinear.normalize();
		steering.mLinear *= iMaxAccel;
	}

	steering.mAngular = 0;

	return steering;
}

SteeringOutputStructure MovementAlgorithms::Allign(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxRotation, float iMaxAngular, float iSlowAngle, float iTargetAngle, float iTime)
{
	SteeringOutputStructure steering = SteeringOutputStructure();
	auto rotation = iTarget.mOrientation - iCharacter.mOrientation;
	
	if(rotation > PI)
	{
		rotation -= (2*PI);
	} else if(rotation < -PI)
	{
		rotation += (2 * PI);
	}
	auto rotationSize = abs(rotation);

	if(rotationSize < iTargetAngle)
	{
		steering.mLinear = ofVec2f(0, 0);
		steering.mAngular = 0;
	}
	auto targetRotation = 0.0f;
	if(rotationSize > iSlowAngle)
	{
		targetRotation = iMaxRotation;
	} else
	{
		targetRotation = (iMaxRotation * rotationSize) / iSlowAngle;
	}

	steering.mAngular = targetRotation - iCharacter.mRotation;
	steering.mAngular /= iTime;

	auto angularAccel = abs(steering.mAngular);
	if(angularAccel > iMaxAngular)
	{
		steering.mAngular /= angularAccel;
		steering.mAngular *= iMaxAngular;
	}

	steering.mLinear = ofVec2f(0, 0);

	return steering;
}

SteeringOutputStructure MovementAlgorithms::LookWhereYouAreGoing(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxRotation, float iMaxAngular, float iSlowAngle, float iTargetAngle, float iTime)
{
	SteeringOutputStructure steering = SteeringOutputStructure();

	if(iCharacter.mVelocity.length() == 0)
	{
		steering.mLinear = ofVec2f(0, 0);
		steering.mAngular = 0;
		return steering;
	}

	iTarget.mOrientation = atan2(iCharacter.mVelocity.y, iCharacter.mVelocity.x);

	return Allign(iCharacter, iTarget, iMaxRotation, iMaxAngular, iSlowAngle, iTargetAngle, iTime);
}

SteeringOutputStructure MovementAlgorithms::DynamicFace(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxRotation, float iMaxAngular, float iSlowAngle, float iTargetAngle, float iTime)
{
	auto direction = iTarget.mPosition - iCharacter.mPosition;
	
	iTarget.mOrientation = atan2(direction.y, direction.x);

	return Allign(iCharacter, iTarget, iMaxRotation, iMaxAngular, iSlowAngle, iTargetAngle, iTime);
}
