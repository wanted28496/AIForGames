#include "ofApp.h"
#include "KinematicSeek.h"
#include "KinematicArrive.h"
#include "Structures.h"

ofVec2f position;
ofVec2f targetLocation;
int radius;
int diff;
int lastPoint;
KinematicSeek kSeekAlgo;
KinematicArrive kArriveAlgo;
bool startAlgo = false;
int caseIndex = 0;
float orientation;


void ofApp::SetupAlgorithms()
{
	kSeekAlgo = KinematicSeek(100);
	kArriveAlgo = KinematicArrive(100, 5, 2);
}

//--------------------------------------------------------------
void ofApp::setup(){
	SetupAlgorithms();
	position = ofVec2f(50, 50);
	radius = 20;
	diff = (radius / 3) * 2;
	lastPoint = radius + diff;
	orientation = 0;
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
		ofDrawSphere(targetLocation.x, targetLocation.y, radius * 1.1);
	}

	ofSetColor(0, 0, 0);
	//float degree = ofRadToDeg();
	ofDrawSphere(position.x, position.y, radius);
	ofDrawTriangle(position.x+ diff, position.y- diff, position.x+ diff, position.y+ diff, position.x+lastPoint, position.y);

	

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
			break;
		case 49:
			playKinematicSeek = false;
			playKinematicArrive = true;
			kArriveAlgo.mTarget.mPosition = kArriveAlgo.mCharacter.mPosition;
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
	position = kSeekAlgo.mCharacter.mPosition;
	orientation = kSeekAlgo.mCharacter.mOrientation;
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
	kArriveAlgo.UpdateKinematic(0.1f, steering);
	position = kArriveAlgo.mCharacter.mPosition;
	orientation = kArriveAlgo.mCharacter.mOrientation;
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
