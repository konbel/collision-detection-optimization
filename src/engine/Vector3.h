#ifndef VECTOR2_H
#define VECTOR2_H

struct Vector3 {
    float x;
    float y;
    float z;

    Vector3 operator+(const Vector3 &other) const {
        return { x + other.x, y + other.y, z + other.z };
    }

    Vector3& operator+=(const Vector3 &other) {
        x += other.x;
        y += other.y;
        z += other.z;

        return *this;
    }

    Vector3 operator-(const Vector3 &other) const {
        return { x - other.x, y - other.y, z - other.z };
    }

    Vector3& operator-=(const Vector3 &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;

        return *this;
    }

    Vector3 operator*(const float scalar) const {
        return { x * scalar, y * scalar, z * scalar };
    }
};

#endif //VECTOR2_H
