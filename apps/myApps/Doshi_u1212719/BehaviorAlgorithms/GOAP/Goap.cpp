#include "Goap.h"
#include "DTNode.h"
#include <assert.h>


Goap::~Goap()
{
}

float Goap::CalculateOrder(vector<Tracer> iRoute)
{
	vector<float> sum(mActions, 0.0f);

	float order = 0;

	for (auto route : iRoute)
	{
		sum[route.mNodeID]++;
	}

	for (auto i : sum)
	{
		float p = i / iRoute.size();
		if (p != 0)
		{
			order += -(p * log(p));
		}
	}

	return order;
}

float Goap::CalculateOrderOfAttribute(vector<Tracer> iRoute, unsigned int iAttributeNumber)
{
	vector<float> sumYes(mActions, 0.f);
	vector<float> sumNo(mActions, 0.f);

	float yesOrder = 0.f;
	float noOrder = 0.f;

	float yesCount = 0.f;
	float noCount = 0.f;

	for (auto route : iRoute)
	{
		if (route.mAttributes[iAttributeNumber])
		{
			sumYes[route.mNodeID]++;
			yesCount++;
		}
		else
		{
			sumNo[route.mNodeID]++;
			noCount++;
		}
	}

	for (auto yes : sumYes)
	{
		float p = yes / yesCount;

		if (p != 0)
			yesOrder += -(p * log(p));
	}

	for (auto no : sumNo)
	{
		float p = no / noCount;

		if (p != 0)
			noOrder += -(p * log(p));
	}

	float order = (yesCount / iRoute.size() * yesOrder) + (noCount / iRoute.size() * noOrder);

	return order;
}

array<vector<Tracer>, 2> Goap::SplitRouteByAttribute(vector<Tracer> iRoute, unsigned int iAttributeNumber)
{
	array<vector<Tracer>, 2> result;

	for (auto route : iRoute)
	{
		if (route.mAttributes[iAttributeNumber])
		{
			result[0].push_back(route);
		}
		else
		{
			result[1].push_back(route);
		}
	}

	return result;

}

DTNode * Goap::CreateDecisionTreeBasedOnAttributes(vector<Tracer> iRoute, list<unsigned int> iAttributeNumber)
{
	float order = CalculateOrder(iRoute);

	if (order == 0)
	{
		DTNode* node = CreateNodeFromAction(iRoute[0].mNodeID);
		return node;
	}

	assert(iAttributeNumber.size());

	float bestInfoGained = 0;
	int bestAttribute = -1;

	vector<unsigned int> worstAttribute;

	for (auto attribute : iAttributeNumber)
	{
		float infoGained = order - CalculateOrderOfAttribute(iRoute, attribute);
		if (infoGained == 0)
		{
			worstAttribute.push_back(attribute);
		}

		if (infoGained > bestInfoGained)
		{
			bestInfoGained = infoGained;
			bestAttribute = attribute;
		}
	}

	for (auto wAttri : worstAttribute)
	{
		iAttributeNumber.remove(wAttri);
	}

	assert(bestAttribute != -1);

	DTNode* node = CreateNodeFromAttribute(bestAttribute);

	auto bestSets = SplitRouteByAttribute(iRoute, bestAttribute);
	iAttributeNumber.remove(bestAttribute);
	for (auto set : bestSets)
	{
		((DTDecisionNode*)node)->AddChild(CreateDecisionTreeBasedOnAttributes(set, iAttributeNumber));
	}

	return node;
}
