#include "stdafx.h"
#include "TriangleIO.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

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

			std::vector<std::string> words;
			split(line, std::string(","), words);

			if (words.size() == 3) { // It is a point
				float x = atof(words.at(1).c_str());
				float y = atof(words.at(2).c_str());
				polygon.push_back(Vector2(x, y));
			}
			else if (words.size() == 4) { // It is a hole
				int holeId = atoi(words.at(1).c_str());
				float x = atof(words.at(2).c_str());
				float y = atof(words.at(3).c_str());
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
			myfile << "};\n\int[] indices = new int[] {";
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
