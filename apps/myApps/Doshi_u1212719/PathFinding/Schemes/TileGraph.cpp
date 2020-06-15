#include "TileGraph.h"
#include <chrono>
#include <random>

TileGraph::TileGraph(int iWorldHeight, int iWorldWidth, int iTileHeight, int iTileWidth) :
	mWorldHeight(iWorldHeight), mWorldWidth(iWorldWidth), mTileHeight(iTileHeight), mTileWidth(iTileWidth)
{
	mHeightCount = mWorldHeight / mTileHeight;
	mWidthCount = mWorldWidth / mTileWidth;
}

TileGraph::~TileGraph()
{
}

void TileGraph::GenerateObstacles(int iObstaclesCount)
{
	set<int> obstacles;

	std::default_random_engine mGenerator(chrono::high_resolution_clock::now().time_since_epoch().count());
	std::uniform_int_distribution<int> mDistribution(1, mWidthCount * mHeightCount);

	while (obstacles.size() != iObstaclesCount)
	{
		obstacles.insert(mDistribution(mGenerator));
	}

	vector<Obstacle> obstacleList;
	for (auto obstacle : obstacles)
	{
		TileGraph::Obstacle o;
		o.mCenter = glm::vec2(obstacle % mWidthCount, obstacle / mWidthCount);
		obstacleList.push_back(o);
	}

	GenerateGraph(obstacleList);
}

void TileGraph::GenerateGraph(vector<Obstacle> iObstacles)
{
	mTileMap.resize(mHeightCount);
	for (int i = 0; i < mHeightCount; i++)
	{
		mTileMap[i].resize(mWidthCount);
		for (int j = 0; j < mWidthCount; j++)
		{
			auto obstacle = find_if(iObstacles.begin(), iObstacles.end(), [i, j](Obstacle o) { 
				return o.mCenter.x == i && o.mCenter.y == j; });

			if (obstacle != iObstacles.end())
			{
				mTileMap[i][j] = 0;
			}
			else
			{
				mTileMap[i][j] = 1;
			}
		}
	}

	for (int i = 0; i < mHeightCount; i++)
	{
		for (int j = 0; j < mWidthCount; j++)
		{
			if (i + 1 < mHeightCount && mTileMap[i + 1][j])
			{
				mGraph.mEdges.push_back(DWEdge(i * mWidthCount + j, (i + 1) * mWidthCount + j, 1));
			}
			if (i - 1 >= 0 && mTileMap[i - 1][j])
			{
				mGraph.mEdges.push_back(DWEdge(i * mWidthCount + j, (i - 1) * mWidthCount + j, 1));
			}
			if (j + 1 < mWidthCount && mTileMap[i][j + 1])
			{
				mGraph.mEdges.push_back(DWEdge(i * mWidthCount + j, i * mWidthCount + j + 1, 1));
			}
			if (j - 1 >= 0 && mTileMap[i][j - 1])
			{
				mGraph.mEdges.push_back(DWEdge(i * mWidthCount + j, i * mWidthCount + j - 1, 1));
			}
		}
	}

	mGraph.getHeuristic = [this](int iSource, int iDestination)
	{
		return 0;
	};

}

int TileGraph::QauntizePosition(ofVec2f iPosition)
{
	return (int)(iPosition.y / mTileHeight) * mWidthCount + (int)(iPosition.x / mTileWidth);

}


ofVec2f TileGraph::LocalizedPosition(int iNodeId)
{
	return ofVec2f((iNodeId % mWidthCount) * mTileWidth + (mTileWidth / 2.0f), (iNodeId / mHeightCount) * mTileHeight + (mTileHeight / 2.0f));
}

Graph & TileGraph::GetGraph()
{
	return mGraph;
}

void TileGraph::DrawTileGraph()
{
	for (int i = 0; i < mHeightCount; i++)
	{
		for (int j = 0; j < mWidthCount; j++)
		{
			if (mTileMap[i][j])
			{
				ofSetColor(ofColor::blanchedAlmond);
			}
			else
			{
				ofSetColor(ofColor::darkKhaki);
			}

			ofPushMatrix();
			ofTranslate(j * mTileWidth, i * mTileHeight);
			ofDrawCircle(mTileWidth / 2.0f, mTileHeight / 2.0f, std::min(mTileWidth / 2.0f, mTileHeight / 2.0f));
			ofPopMatrix();
		}
	}
}
