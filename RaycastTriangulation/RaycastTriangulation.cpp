// RaycastTriangulation.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#define NO_STDIO_REDIRECT
#include "stdafx.h"

#include "Vector2.h"
#include "Triangulator.h"

//const double PI = atan(1.0) * 4;

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Vector2> polygon;
	/*polygon.push_back(Vector2(0, 0));
	polygon.push_back(Vector2(10, 0));
	polygon.push_back(Vector2(10, 10));
	polygon.push_back(Vector2(0, 10));*/

	for (float i = 0; i < 360; i += 2.0f) {
		float x = 1000 * cos(i * PI / 180.0f);
		float y = 1000 * sin(i * PI / 180.0f);
		polygon.push_back(Vector2(x, y));
	}
	
	std::vector<std::vector<Vector2>*> holes;
	std::vector<Vector2> *hole1 = new std::vector<Vector2>();
	hole1->push_back(Vector2(2.5f, 2.5f));
	hole1->push_back(Vector2(7.5f, 2.5f));
	hole1->push_back(Vector2(7.5f, 7.5f));
	hole1->push_back(Vector2(2.5f, 7.5f));
	//holes.push_back(hole1);

	std::vector<Vector2> *vertices = NULL;
	std::vector<int> *indices = NULL;

	Triangulator::triangulate(&polygon, &holes, indices, vertices);

	if (indices && vertices) {
		printf("Indices: %i, Vertices: %i\n", indices->size(), vertices->size());
	}
	else {
		printf("Not initialized.\n");
	}
	return 0;
}

