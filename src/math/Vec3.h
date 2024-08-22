#ifndef MATH_VEC3_H
#define MATH_VEC3_H

template<typename T>
class Vector3 {
public:
	Vector3() = default;
	Vector3(T x, T y, T z) : v{ x, y, z } {}
private:
	T v[3];
};

typedef Vector3<float> Vec3;

#endif // !MATH_VEC3_H
