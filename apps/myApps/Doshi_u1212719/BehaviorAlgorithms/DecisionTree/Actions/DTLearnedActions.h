#pragma once
#include "DTActionBase.h"
#include "Boid.h"
#include "DynamicPathFollow.h"
#include "DynamicAlign.h"
#include "Structures.h"
#include "DTNode.h"

class DTActionTarget : public DTActionBase
{
	bool mCompleted = false;

public:
	DTActionTarget() {
		mExpiryTime = 1;
	}

	~DTActionTarget() {

	}

	function<void()> GetTarget;

	virtual bool CanInterrupt() override {
		return true;
	}

	virtual bool IsActionCompleted() override {

		if (mCompleted)
		{
			mCompleted = false;
			return true;
		}

		return false;

	}

	virtual void Execute(float dt) override {
		mCompleted = true;
		GetTarget();
	}
};

class DTMoveToTarget : public DTActionBase
{
	bool mCompleted = false;

public:
	DTMoveToTarget() {
		mExpiryTime = 1;
	}

	~DTMoveToTarget() {

	}

	function<void()> GetTarget;

	virtual bool CanInterrupt() override {
		return true;
	}

	virtual bool IsActionCompleted() override {
		return mCharacter->mTargets.empty();
	}

	virtual void Execute(float dt) override {
		auto posSteering = DynamicPathFollow::GetSteering(mCharacter->mKinematic, mCharacter->mTargets);
		auto rotSteering = DynamicLookWhereYouAreGoing::GetSteering(mCharacter->mKinematic);

		mCharacter->mKinematic.UpdateDynamicMotion(posSteering + rotSteering, dt);
		((AIBoid*)mCharacter)->mEnergy -= 0.166666667f * 0.9f;
	}
};