#include "BoundingBox.h"
BoundingBox::BoundingBox(Vector3 min, Vector3 max) {
	this->min = min;
	this->max = max;
}
void BoundingBox::setMin(Vector3 minV) {
	min = minV;
}
void BoundingBox::setMax(Vector3 maxV) {
	max = maxV;
}
Vector3& BoundingBox::getMin() {
	return min;
}
Vector3& BoundingBox::getMax() {
	return max;
}
bool BoundingBox::contains(Vector3 vec) {
	return (vec.getX() >= min.getX() && vec.getX() <= max.getX()) &&
		(vec.getY() >= min.getY() && vec.getY() <= max.getY()) &&
		(vec.getZ() >= min.getZ() && vec.getZ() <= max.getZ());
}
Vector3 BoundingBox::getSizes() {
	float sizeX = max.getX() - min.getX();
	float sizeY = max.getY() - min.getY();
	float sizeZ = max.getZ() - min.getZ();
	return Vector3(sizeX, sizeY, sizeZ);
}
Vector3 BoundingBox::getCenter() {
	return Vector3((min.getX() + max.getX()) / 2.0f,
		(min.getY() + max.getY()) / 2.0f,
		(min.getZ() + max.getZ()) / 2.0f);
}