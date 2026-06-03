#include "BoundingBox.h"
BoundingBox::BoundingBox(Vector3 min, Vector3 max) {
	this->min = min;
	this->max = max;
}
bool BoundingBox::contains(Vector3 vec) {
	return (vec.x >= min.x && vec.x <= max.x) &&
		(vec.y >= min.y && vec.y <= max.y) &&
		(vec.z >= min.z && vec.z <= max.z);
}
Vector3 BoundingBox::getSizes() {
	float sizeX = max.x - min.x;
	float sizeY = max.y - min.y;
	float sizeZ = max.z - min.z;
	return Vector3(sizeX, sizeY, sizeZ);
}
Vector3 BoundingBox::getCenter() {
	return Vector3((min.x + max.x) / 2.0f,
		(min.y + max.y) / 2.0f,
		(min.z + max.z) / 2.0f);
}