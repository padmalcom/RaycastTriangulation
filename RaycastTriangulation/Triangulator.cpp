#include "stdafx.h"
#include "Triangulator.h"
#include "Intersection.h"
#include "TinyMath.h"
#include <algorithm>


void Triangulator::triangulate(std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes, std::vector<int> *&indices, std::vector<Vector2> *&vertices)
{
	int idxCnt = 0;

	std::vector<PointAndNeighbours*> *pan = Triangulator::createPointsAndNeighbours(polygon, holes);
	vertices = new std::vector<Vector2>(/*pan->size()*/);	
	indices = new std::vector<int>(/*(pan->size() + (holes.size() - 1) * 2) * 3*/);

	printf("Expecting %i vertices and %i indices.\n", pan->size(), ((pan->size() + (holes.size() - 1) * 2) * 3));

	Vector2 line, bar1, bar2;
	for (std::vector<PointAndNeighbours>::size_type i = 0; i < pan->size(); i++) {
		
		for (std::vector<PointAndNeighbours>::size_type j = i + 1; j < pan->size(); j++) {

			//printf("-Checking connection from point %i at (%f,%f) to point %i at (%f,%f).\n", i, pan->at(i)->p->x, pan->at(i)->p->y, j, pan->at(j)->p->x, pan->at(j)->p->y);

			// No lines to direct neighbours
			if (*(pan->at(j)->p) == *(pan->at(i)->next) || *(pan->at(j)->p) == *(pan->at(i)->prev)) {
				//printf("--Points %i and %i are neighbours. Continuing.\n", i, j);
				continue;
			}

			line = *pan->at(j)->p - *pan->at(i)->p;
			bar1 = *pan->at(i)->prev - *pan->at(i)->p;
			bar2 = *pan->at(i)->next - *pan->at(i)->p;

			if (!Vector2::isLineInBetweenVectors(bar1, bar2, line)) {
				//printf("--Point (%f,%f): Line (%f,%f) is not between points (%f,%f) and (%f,%f).\n\n", pan->at(i)->p->x, pan->at(i)->p->y, line.x, line.y, bar1.x, bar1.y, bar2.x, bar2.y);
				continue;
			}

			bool lineIntersects = false;
			Vector2 *intersectionPoint = NULL;
			for (std::vector<PointAndNeighbours>::size_type k = 0; k < pan->size(); k++) {
				for (std::vector<PointAndNeighbours>::size_type l = 0; l < pan->at(k)->neighbours.size(); l++) {
					if (Intersection::line_intersection(*pan->at(i)->p, *pan->at(j)->p, *pan->at(k)->p, *pan->at(k)->neighbours.at(l)->p, intersectionPoint) && !(*(intersectionPoint) == *(pan->at(i)->p) || *(intersectionPoint) == *(pan->at(j)->p))) {
						//printf("Line (%f,%f) -> (%f,%f) intersects with line (%f,%f)->(%f,%f) at (%f,%f).\n\n", pan->at(i)->p->x, pan->at(i)->p->y,
						//	pan->at(j)->p->x, pan->at(j)->p->y, pan->at(k)->p->x, pan->at(k)->p->y, pan->at(k)->neighbours.at(l)->p->x, pan->at(k)->neighbours.at(l)->p->y,
						//	intersectionPoint->x, intersectionPoint->y);
						lineIntersects = true;
						break;
					}
				}
				if (lineIntersects) break;
			}

			if (!lineIntersects) {
				pan->at(i)->neighbours.push_back(pan->at(j));
				pan->at(j)->neighbours.push_back(pan->at(i));
				//printf("Adding line from (%f, %f) to (%f,%f).\n", pan->at(i)->p->x, pan->at(i)->p->y, pan->at(j)->p->x, pan->at(j)->p->y);
			}
		}
	}

	// Add all vertices
	for (std::vector<PointAndNeighbours>::size_type i = 0; i < pan->size(); i++) {
		vertices->push_back(*pan->at(i)->p);
		//vertices->at(i) = *pan->at(i)->p;
	}

	for (std::vector<PointAndNeighbours>::size_type i = 0; i < pan->size(); i++) {
		printf("Point % i at (%f,%f) has %i neighbours.\n", i, pan->at(i)->p->x, pan->at(i)->p->y, pan->at(i)->neighbours.size());
		for (std::vector<Vector2>::size_type j = 0; j < pan->at(i)->neighbours.size(); j++) {
			printf("\tNeighbour %i at (%f,%f).\n", j, pan->at(i)->neighbours.at(j)->p->x, pan->at(i)->neighbours.at(j)->p->y);
		}
	}

	int p1, p2, p3;
	bool pointInTris = false;

	for (std::vector<PointAndNeighbours*>::size_type i = 0; i < pan->size(); i++) {

		PointAndNeighbours *a = pan->at(i);
		//printf("-Scanning from (%f,%f).\n", a->p->x, a->p->y);

		//printf("Processing point %i with %i neighbours.\n", i, a->neighbours.size());

		for (std::vector<PointAndNeighbours>::size_type j = 0; j < a->neighbours.size(); j++) {

			PointAndNeighbours *b = a->neighbours.at(j);
			//printf("-- To from (%f,%f) with %i neighbours.\n", b->p->x, b->p->y, b->neighbours.size());

			for (std::vector<PointAndNeighbours>::size_type k = 0; k < b->neighbours.size(); k++) {


				PointAndNeighbours *c = b->neighbours.at(k);
				//printf("--- To from (%f,%f).\n", c->p->x, c->p->y);

				if (*(c->p) == *(a->p)) {
					//printf("Counter clockwise.\n");
					continue; // Do not walk triangles counter clockwise/backwards
				}
				

				// If angle a -> b -> c >= 180° => continue since not clockwise. TODO: Measure what's faster. TO compare or to sort?
				if (Vector2::angBetweenVecs(*a->p - *b->p, *c->p - *b->p) >= 180) {
					//printf("Wrong angle.\n");
					continue;
				}

				pointInTris = false;
				for (std::vector<PointAndNeighbours>::size_type l = 0; l < c->neighbours.size(); l++) {

					PointAndNeighbours *d = c->neighbours.at(l);
					//printf("---- To from (%f,%f).\n", d->p->x, d->p->y);

					if (*(d->p) == *(b->p)) {
						//printf("Counter clockwise 2.\n");
						continue;
					}
					//printf("HERE comparing (%f,%f) to (%f,%f).\n", d->p->x, d->p->y, a->p->x, a->p->y);

					if (*(d->p) == *(a->p)) {
						/*printf("Found triangle at (%f,%f), (%f,%f), (%f,%f).\n",
							vertices->at(a->neighbours.at(j)->index).x,
							vertices->at(a->neighbours.at(j)->index).y,
							vertices->at(b->neighbours.at(k)->index).x,
							vertices->at(b->neighbours.at(k)->index).y,
							vertices->at(c->neighbours.at(l)->index).x,
							vertices->at(c->neighbours.at(l)->index).y);*/

						//printf("Found triangle at %i,%i,%i.\n", a->neighbours.at(j)->index, b->neighbours.at(k)->index, c->neighbours.at(l)->index);

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
									//printf("Point %i at (%f,%f) is in current triangle. Continuing.\n", m, pan->at(m)->p->x, pan->at(m)->p->y);
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

							if (!containsTriangle(*indices, p1, p2, p3)) {
								indices->push_back(p1);
								indices->push_back(p2);
								indices->push_back(p3);

								//printf("Adding triangle at %i, %i, %i.\n", newTriangle.i[0], newTriangle.i[1], newTriangle.i[2]);
							}
						}
					}
				}
			}
		}
	}
}

std::vector<EdgeVec2*> *Triangulator::createForbiddenLines(std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes) {
	std::vector<EdgeVec2*> *result = new std::vector<EdgeVec2*>();
	for (std::vector<Vector2>::size_type i = 0; i < polygon.size(); i++) {
		if (i < polygon.size() - 1) {
			result->push_back(new EdgeVec2(polygon.at(i), polygon.at(i + 1), true));
		}
		else {
			result->push_back(new EdgeVec2(polygon.at(i), polygon.at(0), true));
		}
	}

	for (std::vector<Vector2>::size_type i = 0; i < holes.size(); i++) {
		for (std::vector<Vector2>::size_type j = 0; j < holes.at(i)->size(); j++) {
			if (j < holes.at(i)->size() - 1) {
				result->push_back(new EdgeVec2(holes.at(i)->at(j), holes.at(i)->at(j + 1), true));
			}
			else {
				result->push_back(new EdgeVec2(holes.at(i)->at(j), holes.at(i)->at(0), true));
			}
		}
	}
	return result;
}

std::vector<PointAndNeighbours*> *Triangulator::createPointsAndNeighbours(std::vector<Vector2> &polygon, std::vector<std::vector<Vector2>*> &holes) {
	std::vector<PointAndNeighbours*> *result = new std::vector<PointAndNeighbours*>();

	for (std::vector<Vector2>::size_type i = 0; i < polygon.size(); i++) {
		if (i == 0) {
			result->push_back(new PointAndNeighbours(new Vector2(polygon.at(i)), new Vector2(polygon.at(polygon.size() - 1)), new Vector2(polygon.at(i + 1)), result->size(), false));
		}
		else if (i == polygon.size() - 1) {
			result->push_back(new PointAndNeighbours(new Vector2(polygon.at(i)), new Vector2(polygon.at(i - 1)), new Vector2(polygon.at(0)), result->size(), false));
			
			result->at(result->size() - 1)->neighbours.push_back(result->at(0));	
			result->at(result->size() - 2)->neighbours.push_back(result->at(result->size() - 1));
			result->at(result->size() - 1)->nextPan = result->at(0);
			result->at(result->size() - 2)->nextPan = result->at(result->size() - 1);
		}
		else {
			result->push_back(new PointAndNeighbours(new Vector2(polygon.at(i)), new Vector2(polygon.at(i - 1)), new Vector2(polygon.at(i + 1)), result->size(), false));
			result->at(result->size() - 2)->neighbours.push_back(result->at(result->size() - 1));
			result->at(result->size() - 2)->nextPan = result->at(result->size() - 1);
		}
	}
	//printf("Found %n points.", result->size());
	for (std::vector<std::vector<Vector2>*>::size_type i = 0; i < holes.size(); i++) {
		int start = result->size();
		int end = start + holes.at(i)->size() - 1;
		for (std::vector<Vector2>::size_type j = holes.at(i)->size(); j-- > 0;) {
		
			if (j == 0) {
				Vector2 a = holes.at(i)->at(j);
				Vector2 b = holes.at(i)->at(j + 1);
				Vector2 c = holes.at(i)->at(holes.at(i)->size() - 1);
				result->push_back(new PointAndNeighbours(new Vector2(a.x, a.y), new Vector2(b.x, b.y), new Vector2(c.x, c.y), result->size(), true));
				
				result->at(result->size() - 1)->neighbours.push_back(result->at(start));
				result->at(result->size() - 1)->nextPan = result->at(start);

				result->at(result->size() - 2)->neighbours.push_back(result->at(result->size() - 1));
				result->at(result->size() - 2)->nextPan = result->at(result->size() - 1);
			}
			else if (j == holes.at(i)->size() - 1) {
				Vector2 a = holes.at(i)->at(j);
				Vector2 e = holes.at(i)->at(0);
				Vector2 d = holes.at(i)->at(j - 1);
				result->push_back(new PointAndNeighbours(new Vector2(a.x, a.y), new Vector2(e.x, e.y), new Vector2(d.x, d.y), result->size(), true));
			}
			else {
				Vector2 a = holes.at(i)->at(j);
				Vector2 d = holes.at(i)->at(j - 1);
				Vector2 e = holes.at(i)->at(0);
				result->push_back(new PointAndNeighbours(new Vector2(a.x, a.y), new Vector2(e.x, e.y), new Vector2(d.x, d.y), result->size(), true));
				result->at(result->size() - 2)->neighbours.push_back(result->at(result->size() - 1));
				result->at(result->size() - 2)->nextPan = result->at(result->size() - 1);
			}
		}
	}

	return result;
}

bool Triangulator::containsTriangle(std::vector<int> &indices, int a, int b, int c)
{
	for (std::vector<int>::size_type i = 0; i<indices.size(); i += 3)
	{
		if ((indices.at(i) == a && indices.at(i + 1) == b && indices.at(i + 2) == c) ||
			(indices.at(i) == b && indices.at(i + 1) == c && indices.at(i + 2) == a) ||
			(indices.at(i) == c && indices.at(i + 1) == a && indices.at(i + 2) == b)
			)
		{
			return true;
		}
	}
	return false;
}
