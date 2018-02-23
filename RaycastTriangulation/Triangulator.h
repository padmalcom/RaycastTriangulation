#pragma once

#include <vector>
#include "EdgeVec2.h"
#include "PointAndNeighbours.h"

class Triangulator
{
public:
	static void triangulate(std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes, std::vector<int> *&indices, std::vector<Vector2> *&vertices,
		bool _debug, bool _clockwise, std::string videoFile);

private:
	static std::vector<PointAndNeighbours*> *createPointsAndNeighbours(std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes);
	static bool containsTriangle(std::vector<int> &indices, int a, int b, int c, bool _clockwise);
};

