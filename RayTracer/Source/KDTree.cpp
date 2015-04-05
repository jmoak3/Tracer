#include "KDTree.h"
#include <assert.h>
#include <set>


struct AxisComparatorX
{
	bool operator()(Triangle* p1, Triangle* p2) const
	{
		return p1->WorldBound().GetCenter()[0] < p2->WorldBound().GetCenter()[0];
	}
};
struct AxisComparatorY
{
	bool operator()(Triangle* p1, Triangle* p2) const
	{
		return p1->WorldBound().GetCenter()[1] < p2->WorldBound().GetCenter()[1];
	}
};
struct AxisComparatorZ
{
	bool operator()(Triangle* p1, Triangle* p2) const
	{
		return p1->WorldBound().GetCenter()[2] < p2->WorldBound().GetCenter()[2];
	}
};

KDNode* KDNode::Build(std::vector<Triangle*>* scene, int depth)
{
	if (scene->size() == 0)
	{
		KDNode * node = new KDNode();
		node->Bounds;
		node->Left = new KDNode();
		node->Right = new KDNode();
		node->Leaf = true;
		node->Objects = *scene;
		return node;
	}
	int total = scene->size();
	if (depth > 25 || total < 8)
	{
		BoundingBox bounds = (*scene->begin())->WorldBound();
		std::vector<Triangle*>::iterator iScene;
		for (iScene = scene->begin(); iScene != scene->end(); ++iScene)
			bounds = Union(bounds, (*iScene)->WorldBound());

		KDNode * node = new KDNode();
		node->Bounds = bounds;
		node->Left = new KDNode();
		node->Right = new KDNode();
		node->Leaf = true;
		node->Objects = *scene;
		return node;
	}
	//get axis
	int axis = depth % 3;

	//get median-ish axis value from scene!
	int medLoc = (scene->size())/2;
	std::vector<Triangle*> sortedObj;
	for (int i=0;i<total;++i)
		sortedObj.push_back((*scene)[i]);
	if (axis==0)
		std::sort(sortedObj.begin(), sortedObj.end(), AxisComparatorX());
	if (axis==1)
		std::sort(sortedObj.begin(), sortedObj.end(), AxisComparatorY());
	if (axis==2)
		std::sort(sortedObj.begin(), sortedObj.end(), AxisComparatorZ());

	std::vector<Triangle*>::iterator iSort;
	//for (iSort = sortedObj.begin(); iSort != sortedObj.end(); ++iSort)
	//	printf("%f, %f, %f\n", 
	//	(*iSort)->WorldBound().GetCenter()[0], 
	//	(*iSort)->WorldBound().GetCenter()[1],
	//	(*iSort)->WorldBound().GetCenter()[2]);
	//printf("sorted %i\n", axis);


	std::vector<Triangle*> leftObjs(sortedObj.begin(), sortedObj.begin()+medLoc);
	std::vector<Triangle*> rightObjs(sortedObj.begin()+medLoc, sortedObj.end());
	assert(leftObjs.size()+rightObjs.size()==scene->size());

	BoundingBox bounds = (*scene->begin())->WorldBound();
	std::vector<Triangle*>::iterator iScene;
	for (iScene = scene->begin(); iScene != scene->end(); ++iScene)
		bounds = Union(bounds, (*iScene)->WorldBound());

	std::vector<Triangle*>::iterator iLeft;
	std::vector<Triangle*>::iterator iRight;
	std::vector<Triangle*> leftQueue;
	std::vector<Triangle*> rightQueue;
	
	/*for (iLeft = leftObjs.begin(); iLeft != leftObjs.end(); ++iLeft)
	{
		for (iRight = rightObjs.begin(); iRight != rightObjs.end(); ++iRight)
		{
			if ((*iRight)->WorldBound().Touches((*iLeft)->WorldBound()))
				rightQueue.push_back(*iLeft);
			if ((*iLeft)->WorldBound().Touches((*iRight)->WorldBound()))
				leftQueue.push_back(*iRight);
		}
	}
	for (iLeft = leftQueue.begin(); iLeft != leftQueue.end(); ++iLeft)
		leftObjs.push_back(*iLeft);
	for (iRight = rightQueue.begin(); iRight != rightQueue.end(); ++iRight)
		rightObjs.push_back(*iRight);*/
		
	KDNode * node = new KDNode();
	node->Bounds = bounds;
	node->Left = KDNode::Build(&leftObjs, depth+1);
	node->Right = KDNode::Build(&rightObjs, depth+1);
	node->Leaf = false;
	node->Objects = *scene;

	return node;
}

bool KDNode::Intersect(const Ray & ray, Hit * hit)
{
	if (Bounds.IntersectFast(ray))
	{
		if (!Leaf)
		{
			Hit left, right;
			bool l = Left->Intersect(ray, &left);
			bool r = Right->Intersect(ray, &right);
			*hit = left.tHit < right.tHit ? left : right;
			return l || r;
		}
		else
		{
			bool didWeHit = false;
			std::vector<Triangle*>::iterator iScene;
			Hit currHit, bestHit;

			//printf("%i num\n", Objects.size());
			for (iScene = Objects.begin(); iScene!=Objects.end(); ++iScene)
			{	
				if ((*iScene)->Intersect(ray, &currHit) && currHit.tHit < bestHit.tHit)
				{
					bestHit = currHit;
					didWeHit = true;
				}
			}
			*hit = bestHit;
			//if (didWeHit) printf("%i num\n", Objects.size());
			return didWeHit;
		}
	}
	return false;
}
