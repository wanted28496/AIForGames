#pragma once
#include "ofMain.h"
#include "Structures.h"
#include "Graph.h"
#include <vector>

class TileGraph
{
public:

	struct Obstacle
	{
		glm::vec2 mCenter;
		glm::vec2 mSize;
	};

	int mWorldHeight;
	int mWorldWidth;

	int mTileHeight;
	int mTileWidth;

	int mWidthCount;
	int mHeightCount;

	Graph mGraph;

	vector<vector<int>> mTileMap;

	TileGraph() : TileGraph(Width, Height, 20, 20) {}
	TileGraph(int iWorldHeight, int iWorldWidth, int iTileHeight, int iTileWidth);
	~TileGraph();

	void GenerateObstacles(int iObstaclesCount);
	void GenerateGraph();
	int QauntizePosition(ofVec2f iPosition);

	void RemoveObstacle(Obstacle iO);
	vector<Obstacle> mObstacles;

	int GetRandomMapLocation(int iLocation);

	ofVec2f LocalizedPosition(int iNodeId);
	Graph& GetGraph();
	void DrawTileGraph();
};

