#include "ofApp.h"
#include "KinematicSeek.h"
#include "KinematicArrive.h"
#include "Structures.h"
#include "..//Boid.h"


ofVec2f targetLocation;
KinematicSeek kSeekAlgo;
KinematicArrive kArriveAlgo;
Boid sCharacter;
Boid sTarget;
bool startAlgo = false;
int caseIndex = 0;
float startTime;
bool isTimerActive = false;



void ofApp::SetupAlgorithms()
{
	//kSeekAlgo = KinematicSeek(30);
	//kArriveAlgo = KinematicArrive(100, 5, 2);
}

//--------------------------------------------------------------
void ofApp::setup(){
	SetupAlgorithms();
	startTime = ofGetElapsedTimeMillis();
}

//--------------------------------------------------------------
void ofApp::update(){
	if (playKinematicSeek)
	{
		DoKinematicSeek();
	}
	if (playKinematicArrive)
	{
		DoKinematicArrive();
	}
	if(playDynamicArrive)
	{
		DoDynamicArrive();
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (playKinematicArrive || playDynamicArrive)
	{
		ofSetColor(200, 115, 50);
		ofDrawSphere(targetLocation.x, targetLocation.y, 20 * 1.1);
	}

	/*ofSetColor(0, 0, 0);*/
	//float degree = ofRadToDeg();
	if(isTimerActive)
	{
		float timer = ofGetElapsedTimeMillis() - startTime;

		if(timer >= 200)
		{
			sCharacter.InitBreadCrumps();
			startTime = ofGetElapsedTimeMillis();
		}
	}
	sCharacter.DrawBoid();
	sCharacter.DrawBreadCrumps();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	/// Use Key to know which algorithm to follow
	/// 0 is Kinematic Seek around 4 edges
	switch (key)
	{
		case 48:
			playKinematicSeek = true;
			playDynamicArrive = false;
			playKinematicArrive = false;
			sCharacter.ClearBreadCrumps();
			isTimerActive = true;
			break;
		case 49:
			playKinematicSeek = false;
			playDynamicArrive = false;
			playKinematicArrive = true;
			sTarget.SetBoidLocation(sCharacter.GetBoidLocation());
			//kArriveAlgo.mTarget.mPosition = kArriveAlgo.mCharacter.mPosition;
			//sCharacter.SetBoidLocation(kArriveAlgo.mCharacter.mPosition);
			//sCharacter.SetBoidOrientation(kArriveAlgo.mCharacter.mOrientation);
			isTimerActive = true;
			sCharacter.ClearBreadCrumps();
			break;
		case 50:
			playKinematicSeek = false;
			playKinematicArrive = false;
			playDynamicArrive = true;
			sTarget.SetBoidLocation(sCharacter.GetBoidLocation());
			isTimerActive = true;
			sCharacter.ClearBreadCrumps();
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
	if (playKinematicArrive)
	{
		sTarget.SetBoidLocation(ofVec2f(x, y));
		//kArriveAlgo.mTarget.mPosition = ofVec2f(x, y);
		isTimerActive = true;
		targetLocation = sTarget.GetBoidLocation();
	}
	if(playDynamicArrive)
	{
		sTarget.SetBoidLocation(ofVec2f(x, y));
		//kArriveAlgo.mTarget.mPosition = ofVec2f(x, y);
		isTimerActive = true;
		targetLocation = sTarget.GetBoidLocation();
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

	SteeringOutputStructure steering = MovementAlgorithms::KinematicSeek(sCharacter.GetBoidKinematic(), sTarget.GetBoidKinematic(), 150);
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
	SteeringOutputStructure steering = MovementAlgorithms::KinematicArrive(sCharacter.GetBoidKinematic(), sTarget.GetBoidKinematic(), 150, 20, 2);
	if(steering.mLinear.length() <= 0)
	{
		isTimerActive = false;
	}

	sCharacter.SetBoidOrientation(MovementAlgorithms::GetNewOrientation(sCharacter.GetBoidOrientation(), steering.mLinear));

	sCharacter.SetBoidKinematic(MovementAlgorithms::UpdateKinematic(sCharacter.GetBoidKinematic(), 0.1f, steering));
	//kArriveAlgo.UpdateKinematic(0.1f, steering);
	//sCharacter.SetBoidLocation(kArriveAlgo.mCharacter.mPosition);
	//sCharacter.SetBoidOrientation(kArriveAlgo.mCharacter.mOrientation);
}

void ofApp::DoDynamicArrive()
{
	SteeringOutputStructure steering1 = MovementAlgorithms::DynamicArrive(sCharacter.GetBoidKinematic(), sTarget.GetBoidKinematic(), 20, 10, 20, 250, 3);
	if(steering1.mLinear.length() <= 0)
	{
		isTimerActive = false;
	}

	SteeringOutputStructure steering2 = MovementAlgorithms::LookWhereYouAreGoing(sCharacter.GetBoidKinematic(), sTarget.GetBoidKinematic(), 1, 0.5, 30, 10, 2);

	SteeringOutputStructure steering;
	steering.mAngular = (0 * steering1.mAngular) + (1 * steering2.mAngular);
	steering.mLinear = (1 * steering1.mLinear) + (0 * steering2.mLinear);
	/*sCharacter.SetBoidOrientation(MovementAlgorithms::GetNewOrientation(sCharacter.GetBoidOrientation(), steering.mLinear));*/

	sCharacter.SetBoidKinematic(MovementAlgorithms::UpdateDynamic(sCharacter.GetBoidKinematic(), 0.1f, steering, 250));
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
