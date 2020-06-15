#include "DynamicWander.h"
#include "DynamicAlign.h"
#include "DynamicArrive.h"


float DynamicWander::mOffset = 600 + 100;
float DynamicWander::mRadius = 600;
float DynamicWander::mRate = 5;
float DynamicWander::mOrientation = 0;
float DynamicWander::mMaxAccel = 50;

std::default_random_engine DynamicWander::mGenerator(std::chrono::high_resolution_clock::now().time_since_epoch().count());
std::binomial_distribution<int> DynamicWander::mDistribution = std::binomial_distribution<int>(1000, 0.5);


float DynamicWander::GetBinomial(float iMin, float iMax)
{
	float diff = iMax - iMin;
	return (mDistribution(mGenerator) * diff / 1000 - iMin);
}

SteeringOutputStructure DynamicWander::GetSteering(KinematicStructure iKinematic)
{
	KinematicStructure target;
	float condition = 0;
	do
	{
		mOrientation += GetBinomial(-PIE, PIE) * mRate;
		target.mOrientation = mOrientation + iKinematic.mOrientation;

		target.mPosition = iKinematic.mPosition + mOffset * ofVec2f(cosf(iKinematic.mOrientation + condition), sinf(iKinematic.mOrientation + condition));
		target.mPosition += mRadius * ofVec2f(cosf(target.mOrientation), sinf(target.mOrientation));

		condition += 0.1f;

	} while (target.mPosition.x <= 50 || target.mPosition.y <= 50 || target.mPosition.x >= 1000 || target.mPosition.y >= 650);

	auto steering = DynamicArrive::GetSteering(iKinematic, target);
	steering += DynamicLookWhereYouAreGoing::GetSteering(iKinematic);

	return steering;
}
