#pragma once
#include "Vector3.h"
class BoundingBox {
private:
	Vector3 min;
	Vector3 max;
public:
	void setMin(Vector3 minV);
	void setMax(Vector3 maxV);
	Vector3& getMin();
	Vector3& getMax();
	bool contains(Vector3 vec);
	Vector3 getSizes();
	Vector3 getCenter();
	BoundingBox(Vector3 min, Vector3 max);
	BoundingBox() = default;
};