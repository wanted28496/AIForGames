#include "ofApp.h"
#include "KinematicSeek.h"
#include "KinematicArrive.h"
#include "Structures.h"
#include "..//Boid.h"


ofVec2f targetLocation;
KinematicSeek kSeekAlgo;
KinematicArrive kArriveAlgo;
Boid character;
bool startAlgo = false;
int caseIndex = 0;
float startTime;
bool isTimerActive = false;

void ofApp::SetupAlgorithms()
{
	kSeekAlgo = KinematicSeek(30);
	kArriveAlgo = KinematicArrive(100, 5, 2);
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
}

//--------------------------------------------------------------
void ofApp::draw(){

	if (playKinematicArrive)
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
			character.InitBreadCrumps();
			startTime = ofGetElapsedTimeMillis();
		}
	}
	character.DrawBoid();
	character.DrawBreadCrumps();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	/// Use Key to know which algorithm to follow
	/// 0 is Kinematic Seek around 4 edges
	switch (key)
	{
		case 48:
			playKinematicSeek = true;
			playKinematicArrive = false;
			character.SetBoidLocation(kSeekAlgo.mCharacter.mPosition);
			character.SetBoidOrientation(kSeekAlgo.mCharacter.mOrientation);
			character.ClearBreadCrumps();
			isTimerActive = true;
			break;
		case 49:
			playKinematicSeek = false;
			playKinematicArrive = true;
			kArriveAlgo.mTarget.mPosition = kArriveAlgo.mCharacter.mPosition;
			character.SetBoidLocation(kArriveAlgo.mCharacter.mPosition);
			character.SetBoidOrientation(kArriveAlgo.mCharacter.mOrientation);
			isTimerActive = true;
			character.ClearBreadCrumps();
			break;
		case 50:
			playKinematicSeek = false;
			playKinematicArrive = false;
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
		kArriveAlgo.mTarget.mPosition = ofVec2f(x, y);
		isTimerActive = true;
		targetLocation = kArriveAlgo.mTarget.mPosition;
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
	SteeringOutputStructure steering = kSeekAlgo.getSteering();
	kSeekAlgo.UpdateKinematic(0.1f, steering, kSeekAlgo.mMaxSpeed);
	character.SetBoidLocation(kSeekAlgo.mCharacter.mPosition);
	character.SetBoidOrientation(kSeekAlgo.mCharacter.mOrientation);

	if (caseIndex == 0)
	{
		if (kSeekAlgo.mCharacter.mPosition.x >= kSeekAlgo.mTarget.mPosition.x)
		{
			kSeekAlgo.mTarget.mPosition = ofVec2f(950, 700);
			kSeekAlgo.mCharacter.mVelocity = ofVec2f(0, 0);
			caseIndex = 1;
		}
	}
	else if (caseIndex == 1)
	{
		if (kSeekAlgo.mCharacter.mPosition.y >= kSeekAlgo.mTarget.mPosition.y)
		{
			kSeekAlgo.mTarget.mPosition = ofVec2f(50, 700);
			kSeekAlgo.mCharacter.mVelocity = ofVec2f(0, 0);
			caseIndex = 2;
		}
	}
	else if (caseIndex == 2)
	{
		if (kSeekAlgo.mCharacter.mPosition.x <= kSeekAlgo.mTarget.mPosition.x)
		{
			kSeekAlgo.mTarget.mPosition = ofVec2f(50, 50);
			kSeekAlgo.mCharacter.mVelocity = ofVec2f(0, 0);
			caseIndex = 3;
		}
	}
	else if (caseIndex == 3)
	{
		if (kSeekAlgo.mCharacter.mPosition.y <= kSeekAlgo.mTarget.mPosition.y)
		{
			kSeekAlgo.mTarget.mPosition = ofVec2f(950, 50);
			kSeekAlgo.mCharacter.mVelocity = ofVec2f(0, 0);
			caseIndex = 0;
		}
	}
}

void ofApp::DoKinematicArrive()
{
	SteeringOutputStructure steering = kArriveAlgo.getSteering();
	if(steering.mLinear.length() <= 0)
	{
		isTimerActive = false;
	}
	kArriveAlgo.UpdateKinematic(0.1f, steering);
	character.SetBoidLocation(kArriveAlgo.mCharacter.mPosition);
	character.SetBoidOrientation(kArriveAlgo.mCharacter.mOrientation);
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
