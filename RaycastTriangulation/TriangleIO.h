#pragma once
#include <string>
#include <vector>
#include "Vector2.h"
class TriangleIO
{
public:
	static void TriangleIO::split(std::string &line, std::string &delimiter, std::vector<std::string> &strings);
	static void readPolygon(std::string &fileName, std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes);
	static void writeTriangle(std::string &fileName, std::vector<int> &indices, std::vector<Vector2> &vertices, bool asArray);
};

