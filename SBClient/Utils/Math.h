#pragma once
#define PI (3.1415927f)
#include <algorithm>
#include <cmath>
#include <math.h>
#include <stdexcept>
#include <string>
static constexpr float DEG_RAD2 = PI / 360.0f;
static constexpr float DEG_RAD = 180.0f / PI;
static constexpr float RAD_DEG = PI / 180.f;


template<typename T>
struct Vec2 {

	union {
		struct {
			T x, y;
		};
		T arr[2];
	};

	Vec2(T x = 0, T y = 0) {
		this->x = x; this->y = y;
	}

	Vec2<T> add(const Vec2<T>& v2) {
		return Vec2<T>(this->x + v2.x, this->y + v2.y);
	}

	Vec2<T> add(T o) {
		return Vec2<T>(this->x + o, this->y + o);
	}

	Vec2<T> sub(const Vec2<T> v2) {
		return Vec2<T>(this->x - v2.x, this->y - v2.y);
	}
	Vec2<T> mul(float a, float b) {
		return Vec2<T>(x * a, y * b);
	}

	Vec2<T> mul(const Vec2<T>& v3) {
		return Vec2<T>(x * v3.x, y * v3.y);
	}

	Vec2<T> mul(float a) {
		return Vec2<T>(x * a, y * a);
	}
	Vec2<T> sub(const Vec2<int>& o) {
		return Vec2<T>(x - o.x, y - o.y);
	}
	Vec2<T> operator+(const Vec2<T>& v2) const {
		return Vec2<T>(this->x + v2.x, this->y + v2.y);
	}
	Vec2<T> operator-(const Vec2<T>& v2) const {
		return Vec2<T>(this->x - v2.x, this->y - v2.y);
	}

	Vec2<T> operator*(const T& scalar) const {
		return Vec2<T>(this->x * scalar, this->y * scalar);
	}
};

template<typename T>
struct Vec3 {

	union {
		struct {
			T x, y, z;
		};
		T arr[3];
	};

	Vec3<T>(T x = 0, T y = 0, T z = 0) {
		this->x = x; this->y = y; this->z = z;
	}

	Vec3<T>(const Vec3<T>& copy) {
		this->x = copy.x; this->y = copy.y; this->z = copy.z;
	}

	bool operator != (const Vec3<T>& v3) {
		return v3.x != x || v3.y != y || v3.z != z;
	};
	bool operator==(const Vec3<T>& v3) const {
		return v3.x == x && v3.y == y && v3.z == z;
	}
	bool operator*(const Vec3<T>& v3) const {
		return v3.x * x && v3.y * y && v3.z * z;
	}
	bool operator+(const Vec3<T>& v3) const {
		return v3.x + x && v3.y + y && v3.z + z;
	}
	Vec3<T> add(float a) {
		return Vec3<T>(x + a, y + a, z + a);
	}
	Vec3<T> add(float a, float b, float c) {
		return Vec3<T>(x + a, y + b, z + c);
	}
	template<typename R> float distanceTo(const Vec3<R>& other) const { return sqrt(pow(other.x - this->x, 2) + pow(other.y - this->y, 2) + pow(other.z - this->z, 2)); }
	Vec3<T> add(const Vec3<T>& v3) {
		return Vec3<T>(x + v3.x, y + v3.y, z + v3.z);
	}

	Vec3<T> mul(float a, float b, float c) {
		return Vec3<T>(x * a, y * b, z * c);
	}

	Vec3<T> mul(const Vec3<T>& v3) {
		return Vec3<T>(x * v3.x, y * v3.y, z * v3.z);
	}

	Vec3<T> sub(const Vec3<int>& o) {
		return Vec3<T>(x - o.x, y - o.y, z - o.z);
	}

	Vec3<T> sub(const Vec3<float>& o) {
		return Vec3<T>(x - o.x, y - o.y, z - o.z);
	}
	Vec3<T> operator-(const Vec3<int>& o) {
		return Vec3<T>(x - o.x, y - o.y, z - o.z);
	}

	Vec3<T> operator-(const Vec3<float>& o) {
		return Vec3<T>(x - o.x, y - o.y, z - o.z);
	}
	Vec3<T> floor() {
		return Vec3<T>(floorf(x), floorf(y), floorf(z));
	}

	Vec3<int> toInt() {
		return Vec3<int>((int)x, (int)y, (int)z);
	}

	Vec3<float> toFloat() {
		return Vec3<float>((float)x, (float)y, (float)z);
	}

	float dist(const Vec3<int>& e) {
		return sub(e).magnitude();
	}

	float dist(const Vec3<float>& e) {
		return sub(e).magnitude();
	}

	Vec3<T> lerp(const Vec3<float>& o, float val) {
		Vec3<float> ne;
		ne.x = x + val * (o->x - x);
		ne.y = y + val * (o->y - y);
		ne.z = z + val * (o->z - z);
		return ne;
	};

	Vec3<T> lerp(const Vec3<float>& o, float tx, float ty, float tz) {
		Vec3<float> ne;
		ne.x = x + tx * (o.x - x);
		ne.y = y + ty * (o.y - y);
		ne.z = z + tz * (o.z - z);
		return ne;
	};

	float squaredlen() {
		return x * x + y * y + z * z;
	}

	float squaredxzlen() {
		return x * x + z * z;
	}

	T magnitude() const { return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2)); };

	Vec2<float> CalcAngle(Vec3<float> dst) {
		Vec3<float> diff = dst.sub(*this);

		diff.y = diff.y / diff.magnitude();
		Vec2<float> angles;
		angles.x = asinf(diff.y) * -DEG_RAD;
		angles.y = (float)-atan2f(diff.x, diff.z) * DEG_RAD;

		return angles;
	}
	Vec3<T> normalize() const { float mag = this->magnitude(); return Vec3<T>(this->x / mag, this->y / mag, this->z / mag); }
};

struct AABB {
	Vec3<float> lower;
	Vec3<float> upper;
	Vec2<float> size;

	bool intersects(AABB aabb) {
		return aabb.upper.x > lower.x && upper.x > aabb.lower.x &&
			aabb.upper.y > lower.y && upper.y > aabb.lower.y &&
			aabb.upper.z > lower.z && upper.z > aabb.lower.z;
	}

};

template<typename T>
struct Vec4 {

	union {
		struct {
			T x, y, z, w;
		};
		T arr[4];
	};

	Vec4(T x = 0, T y = 0, T z = 0, T w = 0) {
		this->x = x; this->y = y; this->z = z; this->w = w;
	}

	__forceinline bool contains(const Vec2<float>& point) {
		if (point.x <= x || point.y <= y)
			return false;

		if (point.x >= z || point.y >= w)
			return false;
		return true;
	};
};