// RaycastTriangulation.cpp : Definiert den Einstiegspunkt für die Konsolenanwendung.
//
#define NO_STDIO_REDIRECT
#include "stdafx.h"
#include <ctime>
#include <fstream>

#include "Vector2.h"
#include "Triangulator.h"
#include "TriangleIO.h"
#include "TinyMath.h"

int _tmain(int argc, _TCHAR* argv[])
{
	std::vector<Vector2> polygon;
	std::vector<std::vector<Vector2>*> holes;
	bool asArray = false;

	printf("Raycast Triangulator\n====================\nWritten by Jonas Freiknecht 2017 (www.jofre.de)\n\n");

	std::vector<std::string> arguments;
	bool printHelp = false;

	std::string input("");
	std::string output("");
	bool circle = false;
	int circleCount = -1;
	bool debug = false;
	bool outputClockwise = true;
	for (int i = 0; i < argc; i++) {
		std::wstring wtype(argv[i]);
		std::string value(wtype.begin(), wtype.end());
		arguments.push_back(value);
		if (value == "--help" || value == "-h") {
			printf("--help or -h: print this help text\n");
			printf("-i: input file\n");
			printf("-o: output file(required)\n");
			printf("-r: reverse triangle order (default: clockwise, if specified counter clockwise)\n");
			printf("-c: calculate circular mesh(requires - cc\n");
			printf("-cc: circle point count (must be larger at least 3).\n");
			printf("-f: output format(required, 'list' or array')\n");
			printf("-d: debug output\n");
			return 0;
		}
	}

	int i = 0;
	while (i < arguments.size()) {

		// Get input file
		if (arguments.at(i) == "-i") {
			if (i < arguments.size() - 1) {
				i++;
				input = arguments.at(i);
				printf("Input is %s.\n", input.c_str());
			}
		}
		// Get output file
		else if (arguments.at(i) == "-o") {
			if (i < arguments.size() - 1) {
				i++;
				output = arguments.at(i);
				printf("Output is %s.\n", output.c_str());
			}
		}
		// Output format
		else if (arguments.at(i) == "-f") {
			if (i < arguments.size() - 1) {
				i++;
				if (arguments.at(i) == "array") {
					asArray = true;
				}
				else if (arguments.at(i) == "list") {
					asArray == false;
				}
				else {
					printf("Could not recognize output format. exit.");
					return 0;
				}
			}
		}
		else if (arguments.at(i) == "-d") {
			debug = true;
		}
		else if (arguments.at(i) == "-c") {
			circle = true;
		}
		else if (arguments.at(i) == "-r") {
			outputClockwise = false;
		}
		else if (arguments.at(i) == "-cc") {
			if (i < arguments.size() - 1) {
				i++;
				circleCount = atoi(arguments.at(i).c_str());
			}
		}
		i++;
	}

	if (output == "") {
		printf("No output file specified. Exit.\n");
		return 0;
	}

	if (input != "") {
		printf("Reading input file %s...\n", input.c_str());

		std::ifstream inputFile(input.c_str());
		if (!inputFile.good()) {
			printf("Cannot read input file. Exit.\n");
			return 0;
		}

		TriangleIO::readPolygon(input, polygon, holes);
	}
	else if (circle) {
		if (circleCount > 2) {
			printf("Drawing circle with %i points.\n", circleCount);
			double slice = 2 * PI / circleCount;
			for (int i = 0; i < circleCount; i++) {
				double angle = slice * i;
				float x = 1000 * cos(angle);
				float y = 1000 * sin(angle);
				polygon.push_back(Vector2(x, y));
			}
		}
		else {
			printf("Circular polygon defined but '-cc' parameter was not found or smaller than 3.");
			return 0;
		}
	}
	else {
		printf("Neither an input file nor a circular mesh count was specified. Exiting.\n");
		return 0;
	}

	printf("Found polygon with %i points and %i holes.\n\n", polygon.size(), holes.size());

	unsigned int start = clock();


	if (debug) {
		for (std::vector<Vector2>::size_type i = 0; i < polygon.size(); i++) {
			printf("Added point (%f,%f).\n", polygon.at(i).x, polygon.at(i).y);
		}
	}
	
	std::vector<Vector2> *vertices = NULL;
	std::vector<int> *indices = NULL;

	printf("Starting triangulation ...\n");
	Triangulator::triangulate(polygon, holes, indices, vertices, debug, outputClockwise);

	unsigned int stop = clock() - start;

	printf("Found %i vertices and %i indices in %i milliseconds.\n\n", vertices->size(), indices->size(), stop);

	TriangleIO::writeTriangle(output, *indices, *vertices, asArray);
	printf("Vertices and indices written to %s as %s. Done.\n", output.c_str(), (asArray ? "array" : "list"));

	return 0;
}

