#include "stdafx.h"
#include "PointAndNeighbours.h"
#include "Vector2.h"


PointAndNeighbours::PointAndNeighbours(Vector2 *_p, Vector2 *_prev, Vector2 *_next, int _index, int _holeId)
{
	this->p = _p;
	this->prev = _prev;
	this->next = _next;
	this->index = _index;
	this->holeId = _holeId;
	this->angle = Vector2::angBetweenVecs(*this->prev, *this->next);
}

PointAndNeighbours::~PointAndNeighbours()
{
}
