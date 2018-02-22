#pragma once
#include <string>
#include <vector>
#include <windows.h>

#include "Vector2.h"
#include "Triangulator.h"

class TriangleIO
{
public:
	static void readPolygon(std::string &fileName, std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes);
	static void writeTriangle(std::string &fileName, std::vector<int> &indices, std::vector<Vector2> &vertices, bool asArray);
	static void triangulationStepToBitmap(std::string _file, int _width, int _height, std::vector<PointAndNeighbours*> *_pan, std::vector<EdgeVec2> *_vecs);

private:
	static void split(std::string &line, std::string &delimiter, std::vector<std::string> &strings);
};

