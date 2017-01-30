#pragma once

#include <vector>
#include "EdgeVec2.h"
#include "PointAndNeighbours.h"

struct Tris
{
	int i[3];
};

class Triangulator
{
public:
	Triangulator();
	~Triangulator();

	static std::vector<EdgeVec2*> *triangulate(std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes, std::vector<int> *&indices, std::vector<Vector2> *&vertices);

private:
	static std::vector<EdgeVec2*> *createForbiddenLines(std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes);
	static std::vector<PointAndNeighbours*> *createPointsAndNeighbours(std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes);
	static Tris sortIndicesClockwise(std::vector<Vector2> &vertices, int a, int b, int c);
	static bool containsTriangle(std::vector<int> &indices, int a, int b, int c);
	static void sortVectorsClockwiseAroundCenter(std::vector<Vector2> &vecs);
};

