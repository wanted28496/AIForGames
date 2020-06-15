#pragma once

#include "ofMain.h"

//#if _WIN32
//typedef uint32_t int;
//#elif _WIN64
//typedef uint64_t int;
//#endif


class ofApp : public ofBaseApp{

	public:

		void setup();
		void update();
		void draw();

		void SpawnBoids();
		void SpawnCopyForGoap();

		void CreateDT();
		void CreateBT();

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

public:
	class Boid* mPlayer;
	class Boid* mDigger;
	class Boid* mKiller;
	class TileGraph* mTileGraph;

	class AIManager* mDiggerManager;
	class AIManager* mKillerManager;

	bool mF1 = true;
	bool mF2 = true;
	bool mF3 = false;
		
};
