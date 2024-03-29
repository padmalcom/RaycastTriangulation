#include "stdafx.h"
#include "TriangleIO.h"
#include "Triangulator.h"
#include "bitmap_image.hpp"
#include "TinyMath.h"
#include "Triangle.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include <float.h>

void TriangleIO::split(std::string &line, std::string &delimiter, std::vector<std::string> &strings) {
	std::string lineCpy(line);
	size_t pos = 0;
	std::string token;
	while ((pos = lineCpy.find(delimiter)) != std::string::npos) {
		token = lineCpy.substr(0, pos);
		strings.push_back(token);
		lineCpy.erase(0, pos + delimiter.length());
	}
	if (lineCpy.length() > 0) {
		token = lineCpy.substr(0, pos);
		strings.push_back(token);
	}
}

void TriangleIO::readPolygon(std::string &fileName, std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes) {
	std::string line;
	std::ifstream myfile(fileName);

	int expectedHoleId = 0;
	int currentHoleId = -1;
	if (myfile.is_open()) {
		while(std::getline(myfile, line)) {
			if (line.length() == 0) continue;

			if (line.at(0) == '#') continue;

			std::vector<std::string> words;
			split(line, std::string(","), words);

			if (words.size() == 3) { // It is a point
				double x = atof(words.at(1).c_str());
				double y = atof(words.at(2).c_str());
				polygon.push_back(Vector2(x, y));
			}
			else if (words.size() == 4) { // It is a hole
				int holeId = atoi(words.at(1).c_str());
				double x = atof(words.at(2).c_str());
				double y = atof(words.at(3).c_str());
				if (holeId > currentHoleId) {
					holes.push_back(new std::vector<Vector2>());
					currentHoleId = holeId;
				}
				holes.at(holeId)->push_back(Vector2(x, y));
			}
		}
		myfile.close();
	}
}

void TriangleIO::writeTriangle(std::string &fileName, std::vector<int> &indices, std::vector<Vector2> &vertices, bool asArray) {
	std::ofstream myfile(fileName);
	if (myfile.is_open()) {

		if (asArray) {
			myfile << "Vector3[] vertices = new Vector3[] {";
		}
		for (std::vector<Vector2>::size_type i = 0; i < vertices.size(); i++) {
			if (asArray) {
				myfile << "new Vector3(";
			}
			myfile << vertices.at(i).x;
			if (asArray) {
				myfile << "f,0.0f,";
			}
			else {
				myfile << ",";
			}
			myfile << vertices.at(i).y;

			if (asArray) {
				if (i < vertices.size() - 1) {
					myfile << "f),";
				}
				else {
					myfile << "f)";
				}
			}
			else {
				myfile << "\n";
			}		
		}
		if (asArray) {
			myfile << "};\n\nint[] indices = new int[] {";
		}
		for (std::vector<int>::size_type i = 0; i < indices.size(); i+=3) {
			myfile << indices.at(i);
			myfile << ",";
			myfile << indices.at(i + 1);
			myfile << ",";
			myfile << indices.at(i + 2);
			if (asArray) {
				if (i < indices.size() - 3) {
					myfile << ",";
				}
			}
			else {
				myfile << "\n";
			}
		}
		if (asArray) {
			myfile << "};";
		}

		myfile.close();
	}
}

void TriangleIO::triangulationStepToBitmap(std::string _file, int _width, int _height, std::vector<PointAndNeighbours*> *_pan, std::vector<EdgeVec2> *_vecs,
	std::vector<Triangle> *_triangles) {
	//http://www.partow.net/programming/bitmap/index.html#simpleexample02

	// If neither _pan nor _vecs specified, create dummy image
	if (_pan == NULL && _vecs == NULL) {
		bitmap_image image(_width, _height);
		image.set_all_channels(255, 150, 50);
		image.save_image(_file);
		return;
	}

	// get min position to calculate bitmap dimensions
	double min_x = DBL_MAX;
	double min_y = DBL_MAX;
	double max_x = DBL_MIN;
	double max_y = DBL_MIN;

	if (_pan != NULL) {
		for (std::vector<PointAndNeighbours>::size_type i = 0; i < _pan->size(); i++) {
			if (_pan->at(i)->p->x < min_x) min_x = _pan->at(i)->p->x;
			if (_pan->at(i)->p->x > max_x) max_x = _pan->at(i)->p->x;
			if (_pan->at(i)->p->y < min_y) min_y = _pan->at(i)->p->y;
			if (_pan->at(i)->p->y > max_y) max_y = _pan->at(i)->p->y;
		}
	}

	// Create image
	bitmap_image image(_width, _height);
	image.set_all_channels(0, 0, 0);

	image_drawer draw(image);
	draw.pen_width(3);

	// Draw points and neighbors
	if (_pan != NULL) {
		for (std::vector<PointAndNeighbours>::size_type i = 0; i < _pan->size(); i++) {

			if (_pan->at(i)->holeId > -1) {
				draw.pen_color(255, 0, 0);
			}
			else {
				draw.pen_color(0, 255, 0);
			}

			int x = TinyMath::map(_pan->at(i)->p->x, min_x, max_x, 0 + (_width * 0.05f), _width * 0.9f);
			int y = TinyMath::map(_pan->at(i)->p->y, min_y, max_y, 0 + (_height * 0.05f), _height * 0.9f);
			draw.circle(x, y, 3);
		}
	}

	if (_vecs != NULL) {
		for (std::vector<EdgeVec2>::size_type i = 0; i < _vecs->size(); i++) {
			if (_vecs->at(i).isOuter == true) {
				// Draw green line
				draw.pen_color(0, 255, 0);
			}
			else {
				// Draw red line
				draw.pen_color(255, 0, 0);
			}

			int x1 = TinyMath::map(_vecs->at(i).a.x, min_x, max_x, 0 + (_width * 0.05f), _width * 0.9f);
			int y1 = TinyMath::map(_vecs->at(i).a.y, min_y, max_y, 0 + (_height * 0.05f), _height * 0.9f);
			int x2 = TinyMath::map(_vecs->at(i).b.x, min_x, max_x, 0 + (_width * 0.05f), _width * 0.9f);
			int y2 = TinyMath::map(_vecs->at(i).b.y, min_y, max_y, 0 + (_height * 0.05f), _height * 0.9f);

			draw.line_segment(x1, y1, x2, y2);

		}
	}

	if (_triangles != NULL) {
		for (std::vector<Triangle>::size_type i = 0; i < _triangles->size(); i++) {
			draw.pen_color(0, 0, 255);
			int x1 = TinyMath::map(_triangles->at(i).a.x, min_x, max_x, 0 + (_width * 0.05f), _width * 0.9f);
			int y1 = TinyMath::map(_triangles->at(i).a.y, min_y, max_y, 0 + (_height * 0.05f), _height * 0.9f);
			int x2 = TinyMath::map(_triangles->at(i).b.x, min_x, max_x, 0 + (_width * 0.05f), _width * 0.9f);
			int y2 = TinyMath::map(_triangles->at(i).b.y, min_y, max_y, 0 + (_height * 0.05f), _height * 0.9f);
			int x3 = TinyMath::map(_triangles->at(i).c.x, min_x, max_x, 0 + (_width * 0.05f), _width * 0.9f);
			int y3 = TinyMath::map(_triangles->at(i).c.y, min_y, max_y, 0 + (_height * 0.05f), _height * 0.9f);

			draw.triangle(x1, y1, x2, y2, x3, y3);
		}
	}

	// Save image
	image.save_image(_file);
}
