package com.dragonpav.dengine;

public class BoundingBox {
    public Values.Vector3 min;
    public Values.Vector3 max;
    public boolean contains(Values.Vector3 vec) {
        return (vec.x >= min.x && vec.x <= max.x) &&
                (vec.y >= min.y && vec.y <= max.y) &&
                (vec.z >= min.z && vec.z <= max.z);
    };
    public Values.Vector3 getSizes() {
        float sizeX = max.x - min.x;
        float sizeY = max.y - min.y;
        float sizeZ = max.z - min.z;
        return new Values.Vector3(sizeX, sizeY, sizeZ);
    }
    public Values.Vector3 getCenter() {
        return new Values.Vector3((min.x + max.x) / 2.0f,
                (min.y + max.y) / 2.0f,
                (min.z + max.z) / 2.0f);
    }
    public BoundingBox() {
        min = new Values.Vector3();
        max = new Values.Vector3();
    };
    public BoundingBox(Values.Vector3 min, Values.Vector3 max) {
        this.min = min;
        this.max = max;
    }
}
