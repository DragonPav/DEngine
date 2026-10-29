#pragma once
#include "Vector3.h"
class BoundingBox {
private:
	Vector3 min;
	Vector3 max;
public:
	/** @brief Set min vector of `BoundingBox` */
	void setMin(Vector3 minV);
	/** @brief Set max vector of `BoundingBox` */
	void setMax(Vector3 maxV);
	/** @brief Get min vector of `BoundingBox` */
	Vector3& getMin();
	/** @brief Get max vector of `BoundingBox` */
	Vector3& getMax();
	/** @brief Check if `BoundingBox` contains given vector in ifself */
	bool contains(Vector3 vec);
	/** @brief Get size of `BoundingBox` */
	Vector3 getSizes();
	/** @brief Get center of `BoundingBox` */
	Vector3 getCenter();
	/** @param min Min vector
	 * @param max Max vector
	**/
	BoundingBox(Vector3 min, Vector3 max);
	BoundingBox() = default;
};