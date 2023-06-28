#ifndef AABB_TREE_H
#define AABB_TREE_H

#include <vector>

#include "sampling/Triangle.h"
#include "AABB.h"
#include <utils/TypeDefs.h>

struct Node {
	std::vector<Triangle> triangles;
	bool isLeaf;
	Vec3 min;
	Vec3 max;
};

struct AABBTree {
	AABBTree(std::vector<Triangle>& triangles, AABB& initBox) {
		int32_t splitComponent = 0;

		// Calculate how many triangles we have in the current box. If there are no triangles - exit the branch
		// When we have leafSize triangle in the current branch - exit
		// Before splitting the box into two, we need to check whether we have triangles in it.4
		// 
	}



	std::vector<Node> nodes;
	int32_t leafSize = 4;
};

#endif // !AABB_TREE_H
