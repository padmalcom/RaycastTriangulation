#include "stdafx.h"
#include "TinyMath.h"


TinyMath::TinyMath()
{
}


TinyMath::~TinyMath()
{
}

bool TinyMath::compareFloat(float a, float b) {
	return fabs(a - b) < 0.01f;
}
