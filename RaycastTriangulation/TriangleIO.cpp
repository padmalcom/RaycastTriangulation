#include "stdafx.h"
#include "TriangleIO.h"

#include <fstream>
#include <string>
#include <iostream>


void TriangleIO::readPolygon(std::string &fileName, std::vector<Vector2> *&polygon, std::vector<std::vector<Vector2>> *&holes) {
	std::string line;
	std::ifstream myfile(fileName);
	if (myfile.is_open()) {
		while(std::getline(myfile, line)) {
			std::cout << line << '\n';
			if (line.length > 0) {
				if (line[0] == 'p') {
				} else if (line[0] == 'h') {
				}
			}
		}
		myfile.close();
	}
}

void TriangleIO::writeTriangle(std::string &fileName, std::vector<int> &indices, std::vector<Vector2> &vertices) {

}
