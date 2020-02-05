#include "Structures.h"
#include "..//Boid.h"
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

SteeringOutputStructure MovementAlgorithms::KinematicWander(KinematicStructure iCharacter, float iMaxSpeed, float iMaxRotation)
{
	SteeringOutputStructure result = SteeringOutputStructure();

	result.mLinear = iMaxSpeed * AsVector(iCharacter.mOrientation);
	result.mAngular = ofRandomf() * iMaxRotation;

	return result;
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

SteeringOutputStructure MovementAlgorithms::DynamicFlee(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxAccel)
{

	SteeringOutputStructure steering = SteeringOutputStructure();

	steering.mLinear = iCharacter.mPosition - iTarget.mPosition;

	steering.mLinear.normalize();
	steering.mLinear *= iMaxAccel;

	steering.mAngular = 0;
	return steering;
}

SteeringOutputStructure MovementAlgorithms::DynamicEvade(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxAccel, float iPersonalRadius)
{
	auto distance = iCharacter.mPosition - iTarget.mPosition;

	/// Inverse Square Law
	 float repulsion = decayCoefficient / (distance.length() * distance.length());
	if(repulsion > iMaxAccel)
	{
		repulsion = iMaxAccel;
	}
	SteeringOutputStructure steering = SteeringOutputStructure();
	steering.mLinear = distance;
	steering.mLinear.normalize();
	steering.mLinear *= repulsion;
	return steering;
}

SteeringOutputStructure MovementAlgorithms::DynamicSepration(KinematicStructure iCharacter, std::vector< KinematicStructure> iTarget, float iMaxAccel, float iPersonalRadius, int index)
{
	SteeringOutputStructure finalSteering = SteeringOutputStructure();
	int length = iTarget.size();
	float proportion = 1.0f / (length - 1);
	for(int i = 0; i < length; i++)
	{
		if(i == index)
		{
			continue;
		}
		SteeringOutputStructure steering = DynamicEvade(iCharacter, iTarget[i], iMaxAccel, iPersonalRadius);
		//finalSteering.mAngular += proportion * GetNewOrientation(steering.mAngular, steering.mLinear);
		finalSteering.mLinear += steering.mLinear;
	}

	if(finalSteering.mLinear.length() > iMaxAccel)
	{
		finalSteering.mLinear.normalize();
		finalSteering.mLinear *= iMaxAccel;
	}



	return finalSteering;

}

float MapToRange(float angle)
{
	while(angle > PI && angle < -PI)
	{
		if(angle > PI)
		{
			angle -= (2 * PI);
		}
		if(angle < -PI)
		{
			angle += (2 * PI);
		}
	}
	return angle;
}

SteeringOutputStructure MovementAlgorithms::Allign(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxRotation, float iMaxAngular, float iSlowAngle, float iTargetAngle, float iTime)
{
	SteeringOutputStructure steering = SteeringOutputStructure();

	//auto rotation = iTarget.mOrientation - iCharacter.mOrientation;
	auto rotation = MapToRange(iTarget.mOrientation) - MapToRange(iCharacter.mOrientation);
	std::cout << "Target Orientation: " << iTarget.mOrientation << std::endl << "Character Orienttation: " << iCharacter.mOrientation << std::endl;
	/*if(rotation > PI)
	{
		rotation -= (2*PI);
	} else if(rotation < -PI)
	{
		rotation += (2 * PI);
	}*/
	std::cout << "Rotation: " << rotation << std::endl;
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
	/*if(rotation > 0)
	{
	} */
	if(rotation < 0)
	{
		targetRotation *= -1;

	} else
	{
		targetRotation *= 1;
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

SteeringOutputStructure MovementAlgorithms::DynamicWander(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxRotation, float iMaxAngular, float iSlowAngle, float iTargetAngle, float iTime, float iWanderOffset, float iWanderRadius, float iWanderRate, float iWanderOrientation, float iMaxAccel)
{

	iWanderOrientation += ofRandomf() * iWanderRate;
	auto targetOrinentation = iWanderOrientation + iCharacter.mOrientation;


	auto target = iCharacter.mPosition + iWanderOffset * AsVector(iCharacter.mOrientation);
	target += iWanderRadius * AsVector(iTarget.mOrientation);

	SteeringOutputStructure result = DynamicFace(iCharacter, iTarget, iMaxRotation, iMaxAccel, iSlowAngle, iTargetAngle, iTime);

	result.mLinear = iMaxAccel * AsVector(iCharacter.mOrientation);

	return result;
}

SteeringOutputStructure MovementAlgorithms::VelocityMatch(KinematicStructure iCharacter, KinematicStructure iTarget, float iMaxAccel, float iTime)
{
	SteeringOutputStructure steering = SteeringOutputStructure();

	steering.mLinear = iTarget.mVelocity - iCharacter.mVelocity;
	steering.mLinear /= iTime;

	if(steering.mLinear.length() > iMaxAccel)
	{
		steering.mLinear.normalize();
		steering.mLinear *= iMaxAccel;
	}

	steering.mAngular = 0;
	return steering;
}

std::vector<SteeringOutputStructure> MovementAlgorithms::NormalFlock(std::vector<KinematicStructure> iTarget, float iMaxAccel, float iTime, float iRadius, float iMaxRotation)
{
	KinematicStructure target = KinematicStructure();
	std::vector<SteeringOutputStructure> finalSteeringList;
	float boidMass = 5;
	for(int i = 0; i < (int)iTarget.size(); i++)
	{
		target.mPosition.x += (iTarget[i].mPosition.x * boidMass) / (iTarget.size() * boidMass);
		target.mPosition.y += (iTarget[i].mPosition.y * boidMass) / (iTarget.size() * boidMass);
		target.mVelocity.x += (iTarget[i].mVelocity.x * boidMass) / (iTarget.size() * boidMass);
		target.mVelocity.y += (iTarget[i].mVelocity.y * boidMass) / (iTarget.size() * boidMass);
	}

	for(int i = 0; i < (int)iTarget.size(); i++)
	{
		SteeringOutputStructure finalSteering = SteeringOutputStructure();
		SteeringOutputStructure steering1 = MovementAlgorithms::DynamicArrive(iTarget[i], target, 50, iMaxAccel, iRadius, 50, iTime);
		SteeringOutputStructure steering2 = MovementAlgorithms::LookWhereYouAreGoing(iTarget[i], target, iMaxRotation, 0.5, 3, iRadius, 0.1f);

		SteeringOutputStructure steering;
		steering.mAngular = (0 * steering1.mAngular) + (1 * steering2.mAngular);
		steering.mLinear = (1 * steering1.mLinear) + (0 * steering2.mLinear);
		
		/*SteeringOutputStructure seekSteering = MovementAlgorithms::KinematicArrive(iTarget[i], target, iMaxAccel, iRadius, iTime);*/

		SteeringOutputStructure seperationSteering = MovementAlgorithms::DynamicSepration(iTarget[i], iTarget, 50, iRadius, i);

		SteeringOutputStructure velocityMatchSteering = MovementAlgorithms::VelocityMatch(iTarget[i], target, iMaxAccel, iTime);

		finalSteering.mLinear = (0.6 * steering.mLinear) + (0.8 * seperationSteering.mLinear) + (0.4 * velocityMatchSteering.mLinear);

		finalSteering.mAngular = (0.6 * steering.mAngular) + (0.8 * seperationSteering.mAngular) + (0.4 * velocityMatchSteering.mAngular);

		if(finalSteering.mLinear.length() > iMaxAccel)
		{
			finalSteering.mLinear.normalize();
			finalSteering.mLinear *= iMaxAccel;
		}
		
		if(abs(finalSteering.mAngular) > iMaxRotation)
		{
			finalSteering.mAngular /= abs(finalSteering.mAngular);
			finalSteering.mAngular *= iMaxRotation;
		}
		
		finalSteeringList.push_back(finalSteering);
	}
	return finalSteeringList;
}

std::vector<SteeringOutputStructure> MovementAlgorithms::LeaderFlock(std::vector<KinematicStructure> iTarget, float iMaxAccel, float iTime, float iRadius, float iMaxRotation)
{
	std::vector<SteeringOutputStructure> finalSteeringList;
	return finalSteeringList;
}



