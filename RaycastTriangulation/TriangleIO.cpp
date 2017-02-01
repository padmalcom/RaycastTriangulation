#include "stdafx.h"
#include "TriangleIO.h"

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>


void TriangleIO::readPolygon(std::string &fileName, std::vector<Vector2> *&polygon, std::vector<std::vector<Vector2>> *&holes) {
	std::string line;
	std::ifstream myfile(fileName);
	if (polygon == NULL) polygon = new std::vector<Vector2>();
	if (holes == NULL) holes = new std::vector<std::vector<Vector2>>();

	int expectedHoleId = 0;
	if (myfile.is_open()) {
		while(std::getline(myfile, line)) {
			printf("Processing line %s...\n", line.c_str());

			std::stringstream strStream;
			strStream.str(line);
			std::string item;

			std::getline(strStream, item, ',');
			if (item.compare("p")) { // It is a point
				std::getline(strStream, item, ',');
				float x = atof(item.c_str());
				std::getline(strStream, item, ',');
				float y = atof(item.c_str());
				polygon->push_back(Vector2(x, y));
			}
			else if (item.compare("h")) { // It is a hole
				std::getline(strStream, item, ',');
				int holeId = atoi(item.c_str());
				std::getline(strStream, item, ',');
				float x = atof(item.c_str());
				std::getline(strStream, item, ',');
				float y = atof(item.c_str());

				if (holes->size() <= holeId) {
					if (holeId != expectedHoleId) {
						printf("Expected hole id %i but found %i. Exit.\n", expectedHoleId, holeId);
						return;
					}
					holes->push_back(std::vector<Vector2>());
					holes->at(holeId).push_back(Vector2(x, y));
				}
			}
		}
		myfile.close();
	}
	printf("Found %i vertices in polygon and %i holes.\n", polygon->size(), holes->size());
}

void TriangleIO::writeTriangle(std::string &fileName, std::vector<int> &indices, std::vector<Vector2> &vertices) {
	std::ofstream myfile(fileName);
	if (myfile.is_open()) {

		for (std::vector<Vector2>::size_type i = 0; i < vertices.size(); i++) {
			myfile << vertices.at(i).x;
			myfile << ",";
			myfile << vertices.at(i).y;
			myfile << "\n";
		}
		for (std::vector<int>::size_type i = 0; i < indices.size(); i+=3) {
			myfile << indices.at(i);
			myfile << ",";
			myfile << indices.at(i + 1);
			myfile << ",";
			myfile << indices.at(i + 2);
			myfile << "\n";
		}

		myfile.close();
	}
}
