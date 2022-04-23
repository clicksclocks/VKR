#pragma once

#include <cmath>

template <typename T>
struct Quat;

template <typename T>
struct Vec2;

template <typename T>
struct Vec3
{
	union
	{
		struct { T x, y, z; };
		struct { T r, g, b; };
		struct { T rawData[3]; };
	};

	inline Vec3() : x(0), y(0), z(0) {}
	inline Vec3(T p_val) : x(p_val), y(p_val), z(p_val) {}
	inline Vec3(T p_x, T p_y, T p_z) : x(p_x), y(p_y), z(p_z) {}

	inline Vec3 xyz() const { return Vec3(x, y, z); }
	inline Vec3 xzy() const { return Vec3(x, z, y); }
	inline Vec3 yxz() const { return Vec3(y, x, z); }
	inline Vec3 yzx() const { return Vec3(y, z, x); }
	inline Vec3 zxy() const { return Vec3(z, x, y); }
	inline Vec3 zyx() const { return Vec3(z, y, x); }

	inline Vec2<T> xy() const { return Vec2<T>(x, y); }
	inline Vec2<T> xz() const { return Vec2<T>(x, z); }
	inline Vec2<T> yx() const { return Vec2<T>(y, x); }
	inline Vec2<T> yz() const { return Vec2<T>(y, z); }
	inline Vec2<T> zx() const { return Vec2<T>(z, x); }
	inline Vec2<T> zy() const { return Vec2<T>(z, y); }

	
	inline Vec3& operator+=(const Vec3& v)
	{
		x += v.x;
		y += v.y;
		z += v.z;
		return *this;
	}

	inline Vec3& operator-=(const Vec3& v)
	{
		x -= v.x;
		y -= v.y;
		z -= v.z;
		return *this;
	}
	
	inline Vec3& operator*=(T val)
	{
		x *= val;
		y *= val;
		z *= val;
		return *this;
	}

	inline Vec3 operator+() const { return Vec3(x, y, z); }
	inline Vec3 operator-() const { return Vec3(-x, -y, -z); }

	inline Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
	inline Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }

	inline T operator*(const Vec3& v) const { return  x * v.x + y * v.y + z * v.z; }

	inline Vec3 operator*(T val) const { return Vec3(x * val, y * val, z * val); }

	inline void scale(T val) { *this *= val; }
	inline Vec3 scaled(T val) const { return *this * val; }

	inline Vec3 rotated(const Quat<T>& q)
	{
		auto v_p = q * QuatF{ x, y, z, 0 } * q.inversed();
		return Vec3<T>{v_p.x, v_p.y, v_p.z};
	}

	inline T dot(const Vec3& v) const { return  x * v.x + y * v.y + z * v.z; }

	inline Vec3 cross(const Vec3& v) const
	{
		return Vec3(y * v.z - z * v.y,
			        z * v.x - x * v.z,
			        x * v.y - y * v.x);
	}

	inline bool operator==(const Vec3& v) const
	{
		return  x == v.x && y == v.y && z == v.z;
	}

	inline T norm() const { return std::sqrt(x*x + y*y + z*z); }
	inline T length() const { return norm(); }
	inline T normSquared() const { return x*x + y*y + z*z; }
	inline T lengthSquared() const { return normSquared(); }

	inline void normalize()
	{
		T len = length();
		x /= len;
		y /= len;
		z /= len;
	}

	inline Vec3 normalized() const
	{
		T len = length();
		return Vec3(x / len, y / len, z / len);
	}

	inline Vec3 opposite() const { return Vec3(-x, -y, -z); }

	inline static void Scale(Vec3& v, T val) { v.scale(val); }
	inline static T Dot(const Vec3& v, const Vec3& u) { return v.dot(u); }
	inline static Vec3 Cross(const Vec3& v, const Vec3& u) { return v.cross(u); }
	inline static Vec3 Opposite(const Vec3& v) { return v.opposite(); }
	inline static T CosOfAngleBetween(const Vec3& v, const Vec3& u)
	{
		return v * u / (std::sqrt(v.normSquared() * u.normSquared()));
	}
	inline static T AngleBetween(const Vec3& v, const Vec3& u)
	{
		return std::acos(v * u / (std::sqrt(v.normSquared() * u.normSquared())));
	}

};

using Vec3F = Vec3<float>;
using Vec3D = Vec3<double>;