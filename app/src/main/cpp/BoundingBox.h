#pragma once
#include "Vector3.h"
class BoundingBox {
public:
	Vector3 min;
	Vector3 max;
	bool contains(Vector3 vec);
	Vector3 getSizes();
	Vector3 getCenter();
	BoundingBox(Vector3 min, Vector3 max);
	BoundingBox() = default;
};