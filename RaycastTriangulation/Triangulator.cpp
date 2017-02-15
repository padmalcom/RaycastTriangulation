#include "stdafx.h"
#include "Triangulator.h"
#include "Intersection.h"
#include "TinyMath.h"
#include <algorithm>


void Triangulator::triangulate(std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes, std::vector<int> *&indices, std::vector<Vector2> *&vertices, bool _debug, bool _clockwise)
{
	int idxCnt = 0;

	std::vector<PointAndNeighbours*> *pan = Triangulator::createPointsAndNeighbours(polygon, holes);
	vertices = new std::vector<Vector2>();	
	indices = new std::vector<int>();

	printf("Expecting %i vertices and %i indices.\n", pan->size(), ((pan->size() + (holes.size() - 1) * 2) * 3));

	Vector2 line, bar1, bar2;
	for (std::vector<PointAndNeighbours>::size_type i = 0; i < pan->size(); i++) {

		// Add all vertices
		vertices->push_back(*pan->at(i)->p);
		
		for (std::vector<PointAndNeighbours>::size_type j = i + 1; j < pan->size(); j++) {

			if (_debug) printf("Checking connection from point %i at (%f,%f) to point %i at (%f,%f).\n", i, pan->at(i)->p->x, pan->at(i)->p->y, j, pan->at(j)->p->x, pan->at(j)->p->y);

			// No lines to direct neighbours
			if (*(pan->at(j)->p) == *(pan->at(i)->next) || *(pan->at(j)->p) == *(pan->at(i)->prev)) {
				if (_debug) printf("\tPoints %i and %i are neighbours. Continuing.\n", i, j);
				continue;
			}

			// Polygons scan to inner and outer, holes only to outer.
			if (pan->at(i)->holeId > -1 && pan->at(j)->holeId == pan->at(i)->holeId) {
				line = *pan->at(j)->p - *pan->at(i)->p;
				bar1 = *pan->at(i)->prev - *pan->at(i)->p;
				bar2 = *pan->at(i)->next - *pan->at(i)->p;

				if (TinyMath::crossProductZ(bar1, bar2) >= 0.0f) { // TODO: Was >
					Vector2 tmp = bar1;
					bar1 = bar2;
					bar2 = tmp;
				}

				if (!Vector2::isLineInBetweenVectors(bar2, bar1, line)) {
					if (_debug) printf("\tPoint (%f,%f): Line (%f,%f) is not between points (%f,%f) and (%f,%f).\n", pan->at(i)->p->x, pan->at(i)->p->y, line.x, line.y, bar1.x, bar1.y, bar2.x, bar2.y);
					continue;
				}
			}

			bool lineIntersects = false;
			Vector2 *intersectionPoint = NULL;
			for (std::vector<PointAndNeighbours>::size_type k = 0; k < pan->size(); k++) {
				for (std::vector<PointAndNeighbours>::size_type l = 0; l < pan->at(k)->neighbours.size(); l++) {
					if (Intersection::line_intersection(*pan->at(i)->p, *pan->at(j)->p, *pan->at(k)->p, *pan->at(k)->neighbours.at(l)->p, intersectionPoint) && !(*(intersectionPoint) == *(pan->at(i)->p) || *(intersectionPoint) == *(pan->at(j)->p))) {
						if (_debug) {
							printf("\t\tLine (%f,%f) -> (%f,%f) intersects with line (%f,%f)->(%f,%f) at (%f,%f).\n\n", pan->at(i)->p->x, pan->at(i)->p->y,
								pan->at(j)->p->x, pan->at(j)->p->y, pan->at(k)->p->x, pan->at(k)->p->y, pan->at(k)->neighbours.at(l)->p->x, pan->at(k)->neighbours.at(l)->p->y,
								intersectionPoint->x, intersectionPoint->y);
						}
						lineIntersects = true;
						break;
					}
				}
				if (lineIntersects) break;
			}

			if (!lineIntersects) {
				pan->at(i)->neighbours.push_back(pan->at(j));
				pan->at(j)->neighbours.push_back(pan->at(i));
				if (_debug) printf("\tAdding line from (%f, %f) to (%f,%f).\n", pan->at(i)->p->x, pan->at(i)->p->y, pan->at(j)->p->x, pan->at(j)->p->y);
			}
		}
	}

	if (_debug) {
		for (std::vector<PointAndNeighbours>::size_type i = 0; i < pan->size(); i++) {
			printf("Point % i at (%f,%f) has %i neighbours.\n", i, pan->at(i)->p->x, pan->at(i)->p->y, pan->at(i)->neighbours.size());
			for (std::vector<Vector2>::size_type j = 0; j < pan->at(i)->neighbours.size(); j++) {
				printf("\tNeighbour %i at (%f,%f).\n", j, pan->at(i)->neighbours.at(j)->p->x, pan->at(i)->neighbours.at(j)->p->y);
			}
		}
	}

	int p1, p2, p3;
	bool pointInTris = false;

	if (_debug) printf("\n\nStarting to collect triangles.\n\n");

	for (std::vector<PointAndNeighbours*>::size_type i = 0; i < pan->size(); i++) {

		PointAndNeighbours *a = pan->at(i);
		if (_debug) printf("Scanning from point %i at (%f,%f) to all %i neighbours.\n", i, a->p->x, a->p->y, a->neighbours.size());


		for (std::vector<PointAndNeighbours>::size_type j = 0; j < a->neighbours.size(); j++) {

			PointAndNeighbours *b = a->neighbours.at(j);
			if (_debug) printf("\t\t ... to from (%f,%f) with %i neighbours.\n", b->p->x, b->p->y, b->neighbours.size());

			for (std::vector<PointAndNeighbours>::size_type k = 0; k < b->neighbours.size(); k++) {


				PointAndNeighbours *c = b->neighbours.at(k);
				if (_debug) printf("\t\t\t ... to from (%f,%f).\n", c->p->x, c->p->y);

				if (*(c->p) == *(a->p)) {
					if (_debug) printf("\t\t\tTraversing counter clockwise. Skipping.\n");
					continue; // Do not walk triangles counter clockwise/backwards
				}
				

				// If angle a -> b -> c >= 180° => continue since not clockwise. TODO: Measure what's faster. TO compare or to sort?
				if (Vector2::angBetweenVecs(*a->p - *b->p, *c->p - *b->p) >= 180) {
					if (_debug) printf("\t\t\tAngle is not pointing in clockwise order. Skipping.\n");
					continue;
				}

				pointInTris = false;
				for (std::vector<PointAndNeighbours>::size_type l = 0; l < c->neighbours.size(); l++) {

					PointAndNeighbours *d = c->neighbours.at(l);
					if (_debug) printf("\t\t\t\t ... to (%f,%f).\n", d->p->x, d->p->y);

					if (*(d->p) == *(b->p)) {
						if (_debug) printf("\t\t\t\tTraversing counter clockwise. Skipping.\n");
						continue;
					}
				
					if (*(d->p) == *(a->p)) {
						if (_debug) {
							printf("\t\t\t\tFound triangle at points(%f,%f), (%f,%f) and (%f,%f).\n",
								vertices->at(a->neighbours.at(j)->index).x,
								vertices->at(a->neighbours.at(j)->index).y,
								vertices->at(b->neighbours.at(k)->index).x,
								vertices->at(b->neighbours.at(k)->index).y,
								vertices->at(c->neighbours.at(l)->index).x,
								vertices->at(c->neighbours.at(l)->index).y);
						}

						// If there are holes within the polygon, check that there is no hole within the current triangle.
						if (holes.size() > 0) {
							for (std::vector<PointAndNeighbours>::size_type m = 0; m < pan->size(); m++) {

								if (pan->at(m)->index == a->neighbours.at(j)->index ||
									pan->at(m)->index == b->neighbours.at(k)->index ||
									pan->at(m)->index == c->neighbours.at(l)->index) {
									continue;
								}

								if (TinyMath::pointInTriangle(*pan->at(m)->p,
									vertices->at(a->neighbours.at(j)->index),
									vertices->at(b->neighbours.at(k)->index),
									vertices->at(c->neighbours.at(l)->index))) {
									if (_debug) printf("\t\t\t\tPoint %i at (%f,%f) is in current triangle so triangle cannot be closed. Continuing.\n", m, pan->at(m)->p->x, pan->at(m)->p->y);
									pointInTris = true;
									break;
								}
							}
						}

						if (!pointInTris) {

							p1 = a->neighbours.at(j)->index;
							p2 = b->neighbours.at(k)->index;
							p3 = c->neighbours.at(l)->index;

							if (TinyMath::orientation(vertices->at(p1), vertices->at(p2), vertices->at(p3)) >= 0.0f) {
								std::swap(p1, p2);
							}

							if (!containsTriangle(*indices, p1, p2, p3, _clockwise)) {

								if (_clockwise) {
									indices->push_back(p1);
									indices->push_back(p2);
									indices->push_back(p3);

									if (_debug) printf("\t\t\t\tAdding triangle with vertices %i, %i and %i to index array (clockwise).\n", p1, p2, p3);
								}
								else {
									indices->push_back(p3);
									indices->push_back(p2);
									indices->push_back(p1);

									if (_debug) printf("\t\t\t\tAdding triangle with vertices %i, %i and %i to index array (counter clockwise).\n", p1, p2, p3);
								}
							}
							else {
								if (_debug) printf("\t\t\t\tTriangle with indices %i, %i and %i is already in index array.\n", p1, p2, p3);
							}
						}
					}
				}
			}
		}
	}
}

std::vector<PointAndNeighbours*> *Triangulator::createPointsAndNeighbours(std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes) {
	std::vector<PointAndNeighbours*> *result = new std::vector<PointAndNeighbours*>();

	for (std::vector<Vector2>::size_type i = 0; i < polygon.size(); i++) {
		if (i == 0) {
			result->push_back(new PointAndNeighbours(new Vector2(polygon.at(i)), new Vector2(polygon.at(polygon.size() - 1)), new Vector2(polygon.at(i + 1)), result->size(), -1));
		}
		else if (i == polygon.size() - 1) {
			result->push_back(new PointAndNeighbours(new Vector2(polygon.at(i)), new Vector2(polygon.at(i - 1)), new Vector2(polygon.at(0)), result->size(), -1));
			
			result->at(result->size() - 1)->neighbours.push_back(result->at(0));	
			result->at(result->size() - 2)->neighbours.push_back(result->at(result->size() - 1));
			result->at(result->size() - 1)->nextPan = result->at(0);
			result->at(result->size() - 2)->nextPan = result->at(result->size() - 1);
		}
		else {
			result->push_back(new PointAndNeighbours(new Vector2(polygon.at(i)), new Vector2(polygon.at(i - 1)), new Vector2(polygon.at(i + 1)), result->size(), -1));
			result->at(result->size() - 2)->neighbours.push_back(result->at(result->size() - 1));
			result->at(result->size() - 2)->nextPan = result->at(result->size() - 1);
		}
	}
	for (std::vector<std::vector<Vector2>*>::size_type i = 0; i < holes.size(); i++) {
		int start = result->size();
		int end = start + holes.at(i)->size() - 1;
		for (std::vector<Vector2>::size_type j = holes.at(i)->size(); j-- > 0;) {
		
			if (j == 0) {
				Vector2 a = holes.at(i)->at(j);
				Vector2 b = holes.at(i)->at(j + 1);
				Vector2 c = holes.at(i)->at(holes.at(i)->size() - 1);
				result->push_back(new PointAndNeighbours(new Vector2(a.x, a.y), new Vector2(b.x, b.y), new Vector2(c.x, c.y), result->size(), i));
				
				result->at(result->size() - 1)->neighbours.push_back(result->at(start));
				result->at(result->size() - 1)->nextPan = result->at(start);

				result->at(result->size() - 2)->neighbours.push_back(result->at(result->size() - 1));
				result->at(result->size() - 2)->nextPan = result->at(result->size() - 1);
			}
			else if (j == holes.at(i)->size() - 1) {
				Vector2 a = holes.at(i)->at(j);
				Vector2 e = holes.at(i)->at(0);
				Vector2 d = holes.at(i)->at(j - 1);
				result->push_back(new PointAndNeighbours(new Vector2(a.x, a.y), new Vector2(e.x, e.y), new Vector2(d.x, d.y), result->size(), i));
			}
			else {
				Vector2 a = holes.at(i)->at(j);
				Vector2 d = holes.at(i)->at(j - 1);
				Vector2 e = holes.at(i)->at(0);
				result->push_back(new PointAndNeighbours(new Vector2(a.x, a.y), new Vector2(e.x, e.y), new Vector2(d.x, d.y), result->size(), i));
				result->at(result->size() - 2)->neighbours.push_back(result->at(result->size() - 1));
				result->at(result->size() - 2)->nextPan = result->at(result->size() - 1);
			}
		}
	}

	return result;
}

bool Triangulator::containsTriangle(std::vector<int> &indices, int a, int b, int c, bool _clockwise)
{
	if (_clockwise) {
		for (std::vector<int>::size_type i = 0; i < indices.size(); i += 3)
		{
			if ((indices.at(i) == a && indices.at(i + 1) == b && indices.at(i + 2) == c) ||
				(indices.at(i) == b && indices.at(i + 1) == c && indices.at(i + 2) == a) ||
				(indices.at(i) == c && indices.at(i + 1) == a && indices.at(i + 2) == b)
				)
			{
				return true;
			}
		}
	}
	else {
		for (std::vector<int>::size_type i = 0; i < indices.size(); i += 3)
		{
			if ((indices.at(i) == c && indices.at(i + 1) == b && indices.at(i + 2) == a) ||
				(indices.at(i) == b && indices.at(i + 1) == a && indices.at(i + 2) == c) ||
				(indices.at(i) == a && indices.at(i + 1) == c && indices.at(i + 2) == b)
				)
			{
				return true;
			}
		}
	}
	return false;
}
