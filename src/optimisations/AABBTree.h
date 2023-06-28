#ifndef AABB_TREE_H
#define AABB_TREE_H

#include <vector>

#include "sampling/Triangle.h"
#include "AABB.h"
#include <utils/TypeDefs.h>

struct Node {
	Node(std::vector<Triangle>& tris, bool isLeaf, Vec3& min, Vec3& max, int child1, int child2) :
		triangles(tris),
		isLeaf(isLeaf),
		min(min),
		max(max),
		child1Idx(child1),
		child2Idx(child2) {}
	Node() = default;

	std::vector<Triangle> triangles;
	bool isLeaf;
	Vec3 min{};
	Vec3 max{};
	int child1Idx{};
	int child2Idx{};
};

struct AABBTree {
	AABBTree(std::vector<Object>& objects, AABB& initBox): objects(objects) {
		buildTree(initBox, 0, -1, -1, 0);
	}

	std::vector<Node> nodes;
	int32_t leafSize = 1;
	std::vector<Object> objects;
	int32_t maxDepth = 10;
private:
	void buildTree(AABB& box, int component, int parentIdx, int child, int depth) {
		std::vector<Triangle> trianglesInBox;

		for (Object& obj : objects) {
			for (Triangle& tri : obj.triangles) {
				if (box.intersect(tri)) {
					trianglesInBox.push_back(tri);
				}
			}
		}

		int idx = nodes.size();

		if (parentIdx > -1) {
			if (child == 1) {
				nodes[parentIdx].child1Idx = idx;
			}
			else {
				nodes[parentIdx].child2Idx = idx;
			}
		}

		if (depth > maxDepth || trianglesInBox.size() <= leafSize) {
			nodes.emplace_back(trianglesInBox, true, box.min, box.max, -1, -1);
			
			return;
		}

		nodes.emplace_back(std::vector<Triangle>{}, false, box.min, box.max, -1, -1);

		AABB firstChildBox = box;
		firstChildBox.max[component] = firstChildBox.min[component] + ((firstChildBox.max[component] - firstChildBox.min[component]) / 2);
		buildTree(firstChildBox, (component + 1) % 3, idx, 1, depth + 1);
		AABB secondChildBox = box;
		secondChildBox.min[component] = firstChildBox.max[component];
		buildTree(secondChildBox, (component + 1) % 3, idx, 2, depth + 1);
	}
};

#endif // !AABB_TREE_H
