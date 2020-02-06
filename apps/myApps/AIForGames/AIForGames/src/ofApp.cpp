#include "ofApp.h"
#include "KinematicSeek.h"
#include "KinematicArrive.h"
#include "Structures.h"
#include "..//Boid.h"

Boid sCharacter;
Boid sTarget;
std::vector<Boid> flock;
bool startAlgo = false;
int caseIndex = 0;
float startTime;
bool isTimerActive = true;
int leaderIndex = 0;
Algorithms currentAlgorithm = Algorithms::Kinematicseek;

void ofApp::SetupAlgorithms()
{
	//kSeekAlgo = KinematicSeek(30);
	//kArriveAlgo = KinematicArrive(100, 5, 2);
}

//--------------------------------------------------------------
void ofApp::setup(){
	SetupAlgorithms();
	startTime = ofGetElapsedTimeMillis();
	
	sTarget.SetBoidLocation(ofVec2f(950, 50));
	//sTarget.SetBoidLocation(ofVec2f(500, 500));
	/*flock.push_back(sCharacter);
	flock.push_back(sTarget);*/
	for(int i = 0; i < 10; i++)
	{
		Boid b = Boid();
		b.SetBoidLocation(ofVec2f(ofRandom(Width), ofRandom(Height)));
		flock.push_back(b);
	}

}

//--------------------------------------------------------------
void ofApp::update(){
	switch(currentAlgorithm)
	{
	case Algorithms::DynamicArrive:
		DoDynamicArrive();
		break;
	case Algorithms::KinematicArrive:
		DoKinematicArrive();
		break;
	case Algorithms::Kinematicseek:
		DoKinematicSeek();
		break;
	case Algorithms::LeaderFlock:
		DoLeaderFlocking();
		break;
	case Algorithms::NormalFlock:
		DoNormalFlocking();
		break;
	case Algorithms::DynamicWander:
		DoDynamicWander();
		break;
	case Algorithms::KinematicWander:
		DoKinematicWander();
		break;

	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	
	if(currentAlgorithm != Algorithms::NormalFlock && currentAlgorithm != Algorithms::LeaderFlock)
	{
		if(currentAlgorithm == Algorithms::DynamicArrive || currentAlgorithm == Algorithms::KinematicArrive)
		{
			ofSetColor(200, 115, 50);
			sTarget.DrawBoid();
		}

		/*ofSetColor(0, 0, 0);*/
		//float degree = ofRadToDeg();
		float timer = ofGetElapsedTimeMillis() - startTime;

		if(timer >= 200)
		{
			sCharacter.InitBreadCrumps();
			startTime = ofGetElapsedTimeMillis();
		}
		ofSetColor(0, 0, 0);
		sCharacter.DrawBoid();
		sCharacter.DrawBreadCrumps();
	} else
	{
		float timer = ofGetElapsedTimeMillis() - startTime;

		if(timer >= 200)
		{
			for(int i = 0; i < ( int) flock.size(); i++)
			{
				flock[i].InitBreadCrumps();
			}
			startTime = ofGetElapsedTimeMillis();
		}
		
		for(int i = 0; i < (int)flock.size(); i++)
		{
			flock[i].DrawBreadCrumps();
		}

		for(int i = 0; i < ( int) flock.size(); i++)
		{
			if(currentAlgorithm == Algorithms::LeaderFlock && i == leaderIndex)
			{
				ofSetColor(0, 250, 0);
			} else
			{
				ofSetColor(0, 0, 0);
			}
			flock[i].DrawBoid();
		}
		
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

	/// Use Key to know which algorithm to follow
	/// 0 is Kinematic Seek around 4 edges
	switch (key)
	{
		case 48:
			currentAlgorithm = Algorithms::Kinematicseek;
			sCharacter.ClearBreadCrumps();
			isTimerActive = true;
			break;
		case 49:
			currentAlgorithm = Algorithms::KinematicArrive;
			sTarget.SetBoidLocation(sCharacter.GetBoidLocation());
			//kArriveAlgo.mTarget.mPosition = kArriveAlgo.mCharacter.mPosition;
			//sCharacter.SetBoidLocation(kArriveAlgo.mCharacter.mPosition);
			//sCharacter.SetBoidOrientation(kArriveAlgo.mCharacter.mOrientation);
			isTimerActive = true;
			sCharacter.ClearBreadCrumps();
			break;
		case 50:
			currentAlgorithm = Algorithms::DynamicArrive;
			sTarget.SetBoidLocation(sCharacter.GetBoidLocation());
			isTimerActive = true;
			sCharacter.ClearBreadCrumps();
			break;
		case 51:
			currentAlgorithm = Algorithms::KinematicWander;
			sTarget.SetBoidLocation(sCharacter.GetBoidLocation());
			isTimerActive = true;
			sCharacter.ClearBreadCrumps();
			break;
		case 52:
			currentAlgorithm = Algorithms::DynamicWander;
			sTarget.SetBoidLocation(sCharacter.GetBoidLocation());
			isTimerActive = true;
			sCharacter.ClearBreadCrumps();
			break;
			break;
		case 53:
			currentAlgorithm = Algorithms::NormalFlock;
			isTimerActive = true;
			for(int i = 0; i < (int)flock.size(); i++)
			{
				flock[i].ClearBreadCrumps();
			}
			break;
		case 54:
			currentAlgorithm = Algorithms::LeaderFlock;
			isTimerActive = true;
			for(int i = 0; i < (int)flock.size(); i++)
			{
				flock[i].ClearBreadCrumps();
			}
			break;
		case 110:
			if(currentAlgorithm == Algorithms::LeaderFlock)
			{
				leaderIndex = (int)ofRandom(flock.size());
			}
			break;

	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	if(currentAlgorithm == Algorithms::KinematicArrive || currentAlgorithm == Algorithms::DynamicArrive)
	{
		sTarget.SetBoidLocation(ofVec2f(x, y));
		//kArriveAlgo.mTarget.mPosition = ofVec2f(x, y);
		isTimerActive = true;
	}
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

void ofApp::DoKinematicSeek()
{

	SteeringOutputStructure steering = MovementAlgorithms::KinematicSeek(sCharacter.GetBoidKinematic(), sTarget.GetBoidKinematic(), 250);
	sCharacter.SetBoidOrientation(MovementAlgorithms::GetNewOrientation(sCharacter.GetBoidOrientation(), steering.mLinear));
	sCharacter.SetBoidKinematic(MovementAlgorithms::UpdateKinematic(sCharacter.GetBoidKinematic(), 0.116, steering));
	//kSeekAlgo.UpdateKinematic(0.1f, steering, kSeekAlgo.mMaxSpeed);
	//sCharacter.SetBoidLocation(kSeekAlgo.mCharacter.mPosition);
	//sCharacter.SetBoidOrientation(kSeekAlgo.mCharacter.mOrientation);

	if (caseIndex == 0)
	{
		if (sCharacter.GetBoidLocation().x >= sTarget.GetBoidLocation().x)
		{
			sTarget.SetBoidLocation(ofVec2f(950, 700));
			//kSeekAlgo.mTarget.mPosition = ofVec2f(950, 700);
			//kSeekAlgo.mCharacter.mVelocity = ofVec2f(0, 0);
			caseIndex = 1;
		}
	}
	else if (caseIndex == 1)
	{
		if (sCharacter.GetBoidLocation().y >= sTarget.GetBoidLocation().y)
		{
			sTarget.SetBoidLocation(ofVec2f(50, 700));
			//kSeekAlgo.mTarget.mPosition = ofVec2f(50, 700);
			//kSeekAlgo.mCharacter.mVelocity = ofVec2f(0, 0);
			caseIndex = 2;
		}
	}
	else if (caseIndex == 2)
	{
		if (sCharacter.GetBoidLocation().x <= sTarget.GetBoidLocation().x)
		{
			sTarget.SetBoidLocation(ofVec2f(50, 50));
			//kSeekAlgo.mTarget.mPosition = ofVec2f(50, 50);
			//kSeekAlgo.mCharacter.mVelocity = ofVec2f(0, 0);
			caseIndex = 3;
		}
	}
	else if (caseIndex == 3)
	{
		if (sCharacter.GetBoidLocation().y <= sTarget.GetBoidLocation().y)
		{
			sTarget.SetBoidLocation(ofVec2f(950, 50));
			//kSeekAlgo.mTarget.mPosition = ofVec2f(950, 50);
			//kSeekAlgo.mCharacter.mVelocity = ofVec2f(0, 0);
			caseIndex = 0;
		}
	}
}

void ofApp::DoKinematicArrive()
{
	SteeringOutputStructure steering1 = MovementAlgorithms::KinematicArrive(sCharacter.GetBoidKinematic(), sTarget.GetBoidKinematic(), 150, 20, 2);
	if(steering1.mLinear.length() <= 0)
	{
		isTimerActive = false;
	}

	SteeringOutputStructure steering2 = MovementAlgorithms::LookWhereYouAreGoing(sCharacter.GetBoidKinematic(), sTarget.GetBoidKinematic(), 3, 0.5, 3, 1, 0.1f);

	SteeringOutputStructure steering;
	steering.mAngular = (0 * steering1.mAngular) + (1 * steering2.mAngular);
	steering.mLinear = (1 * steering1.mLinear) + (0 * steering2.mLinear);
	/*sCharacter.SetBoidOrientation(MovementAlgorithms::GetNewOrientation(sCharacter.GetBoidOrientation(), steering.mLinear));*/

	sCharacter.SetBoidKinematic(MovementAlgorithms::UpdateKinematic(sCharacter.GetBoidKinematic(), 0.1f, steering));
	//kArriveAlgo.UpdateKinematic(0.1f, steering);
	//sCharacter.SetBoidLocation(kArriveAlgo.mCharacter.mPosition);
	//sCharacter.SetBoidOrientation(kArriveAlgo.mCharacter.mOrientation);
}

void ofApp::DoDynamicArrive()
{
	SteeringOutputStructure steering1 = MovementAlgorithms::DynamicArrive(sCharacter.GetBoidKinematic(), sTarget.GetBoidKinematic(), 20, 5, 20, 150, 0.1);
	if(steering1.mLinear.length() <= 0)
	{
		isTimerActive = false;
	}

	SteeringOutputStructure steering2 = MovementAlgorithms::LookWhereYouAreGoing(sCharacter.GetBoidKinematic(), sTarget.GetBoidKinematic(), 3, 0.5, 3, 1, 0.1f);

	SteeringOutputStructure steering;
	steering.mAngular = (0 * steering1.mAngular) + (1 * steering2.mAngular);
	steering.mLinear = (1 * steering1.mLinear) + (0 * steering2.mLinear);
	/*sCharacter.SetBoidOrientation(MovementAlgorithms::GetNewOrientation(sCharacter.GetBoidOrientation(), steering.mLinear));*/

	sCharacter.SetBoidKinematic(MovementAlgorithms::UpdateDynamic(sCharacter.GetBoidKinematic(), 0.1f, steering, 250));
}

void ofApp::DoDynamicWander()
{
	SteeringOutputStructure steering = MovementAlgorithms::DynamicWander(sCharacter.GetBoidKinematic(), sTarget.GetBoidKinematic(), 10, 10, 50, 10, 0.1, 0.1, 0.1, 0.1, 30, 2);
	sCharacter.SetBoidKinematic(MovementAlgorithms::UpdateDynamic(sCharacter.GetBoidKinematic(), 0.1f, steering, 10));

}

void ofApp::DoKinematicWander()
{
	SteeringOutputStructure steering = MovementAlgorithms::KinematicWander(sCharacter.GetBoidKinematic(), 100, 30);
	sCharacter.SetBoidKinematic(MovementAlgorithms::UpdateKinematic(sCharacter.GetBoidKinematic(), 0.1f, steering));
}

void ofApp::DoNormalFlocking()
{
	std::vector<KinematicStructure> kinematics;
	for(int i = 0; i < (int)flock.size(); i++)
	{
		kinematics.push_back(flock[i].GetBoidKinematic());
	}
	std::vector<SteeringOutputStructure> steeringList = MovementAlgorithms::NormalFlock(kinematics, 10, 0.1, 30, 10);

	for(int i = 0; i < (int)flock.size(); i++)
	{
		flock[i].SetBoidKinematic(MovementAlgorithms::UpdateDynamic(flock[i].GetBoidKinematic(), 0.1, steeringList[i], 20));
	}

}

void ofApp::DoLeaderFlocking()
{
	std::vector<KinematicStructure> kinematics;
	for(int i = 0; i < ( int) flock.size(); i++)
	{
		kinematics.push_back(flock[i].GetBoidKinematic());
	}
	std::vector<SteeringOutputStructure> steeringList = MovementAlgorithms::LeaderFlock(kinematics, 10, 0.1, 30, 10, leaderIndex);

	flock[leaderIndex].SetBoidKinematic(MovementAlgorithms::UpdateKinematic(flock[leaderIndex].GetBoidKinematic(), 0.1f, steeringList[leaderIndex]));

	for(int i = 0; i < ( int) flock.size(); i++)
	{
		if(i == leaderIndex)
		{
			continue;
		}
		flock[i].SetBoidKinematic(MovementAlgorithms::UpdateDynamic(flock[i].GetBoidKinematic(), 0.1, steeringList[i], 20));
	}
}


//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
