#pragma once
#include "ofApp.h"
#include "Graph.h"


class AStar
{
public:
	AStar();
	~AStar();

	static vector<int> findPath(int iStart, int iGoal, Graph& iGraph);
};

