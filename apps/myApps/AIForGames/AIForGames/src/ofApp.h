#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void SetupAlgorithms();
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
		bool playKinematicSeek = false;
		void DoKinematicSeek();

		bool playKinematicArrive = false;
		void DoKinematicArrive();

		bool playDynamicArrive = false;
		void DoDynamicArrive();

		bool playDynamicWander = false;
		void DoDynamicWander();

		bool playNormalFlocking = false;
		bool playLeaderFlocking = false;
		void DoNormalFlocking();
		void DoLeaderFlocking();

};

enum Algorithms
{
	Kinematicseek,
	KinematicArrive,
	DynamicArrive,
	Wander,
	NormalFlock,
	LeaderFlock,
};
