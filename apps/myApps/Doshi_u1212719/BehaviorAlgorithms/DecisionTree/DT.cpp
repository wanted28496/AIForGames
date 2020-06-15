#include "DT.h"


DT::DT(DTNode * iRootNode) : mRootNode(iRootNode)
{
}

DT::~DT()
{
	delete mRootNode;
}
