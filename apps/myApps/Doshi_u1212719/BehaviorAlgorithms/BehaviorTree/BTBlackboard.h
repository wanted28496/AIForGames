#pragma once

#include <map>
#include <vector>
#include <string>

using namespace std;

struct Tracer
{
	unsigned int mNodeID;
	vector<bool> mAttributes;

	Tracer() {

	}

	Tracer(unsigned int iNodeID, vector<bool> iAttributes) : mNodeID(iNodeID), mAttributes(iAttributes) {

	}
};


class BTBlackboard
{

private:
	typedef map<string, void*> NodeData;
	typedef map<int, NodeData> TreeData;

	map<int, TreeData> mTreeData;

	vector<Tracer> mRoute;

public:
	BTBlackboard();
	~BTBlackboard();

	void SetValueInBlackboard(string iKey, void* iValue, unsigned int iTreeID = -1, int iNodeID = -1);
	void* GetValueFromBlackboard(string iKey, unsigned int iTreeID = -1, int iNodeID = -1);

	void AddNodeToRoute(Tracer iNode);
	vector<Tracer> GetRoute();

	void ResetBlackboard();
};

