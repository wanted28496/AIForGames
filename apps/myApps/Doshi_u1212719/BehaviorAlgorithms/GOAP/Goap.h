#pragma once

#include <map>
#include <functional>
#include <list>
#include <vector>
#include <array>
#include <string>

#include "BTBlackboard.h"

using namespace std;

class Goap
{

	unsigned int mActions;
	unsigned int mAttributes;
public:
	Goap(unsigned int iActions, unsigned int iAttributes) : mActions(iActions), mAttributes(iAttributes) {}
	~Goap();

	float CalculateOrder(vector<Tracer> iRoute);
	float CalculateOrderOfAttribute(vector<Tracer> iRoute, unsigned int iAttributeNumber);
	array<vector<Tracer>, 2> SplitRouteByAttribute(vector<Tracer> iRoute, unsigned int iAttributeNumber);

	class DTNode* CreateDecisionTreeBasedOnAttributes(vector<Tracer> iRoute, list<unsigned int> iAttributeNumber);

	function<DTNode*(unsigned int)> CreateNodeFromAttribute;
	function<DTNode*(unsigned int)> CreateNodeFromAction;

};

