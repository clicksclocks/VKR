#pragma once

#include <cmath>


template <typename T>
struct Vec2
{
	union
	{
		struct { T x, y; };
		struct { T rawData[2]; };
	};

	inline Vec2() : x(0), y(0) {}
	inline Vec2(T p_val) : x(p_val), y(p_val) {}
	inline Vec2(T p_x, T p_y) : x(p_x), y(p_y) {}

	inline Vec2& operator+=(const Vec2& v)
	{
		x += v.x;
		y += v.y;
		return *this;
	}

	inline Vec2& operator-=(const Vec2& v)
	{
		x -= v.x;
		y -= v.y;
		return *this;
	}

	inline Vec2& operator*=(T val)
	{
		x *= val;
		y *= val;
		return *this;
	}

	inline Vec2 operator+() const { return Vec2(x, y); }
	inline Vec2 operator-() const { return Vec2(-x, -y); }

	inline Vec2 operator+(const Vec2& v) const { return Vec2(x + v.x, y + v.y); }
	inline Vec2 operator-(const Vec2& v) const { return Vec2(x - v.x, y - v.y); }
	inline T operator*(const Vec2& v) const { return  x*v.x + y*v.y; }
	inline Vec2& operator=(const Vec2& v) { x = v.x; y = v.y; return *this; }

	inline Vec2 operator*(T val) const { return Vec2(x*val, y*val); }

	inline void scale(T val) { *this *= val; }
	inline Vec2 scaled(T val) const { return *this * val; }

	inline T dot(const Vec2& v) const { return  x*v.x + y*v.y; }

	inline bool operator==(const Vec2& v) const { return  x == v.x && y == v.y; }

	inline T norm() const { return std::sqrt(x*x + y*y); }
	inline T length() const { return norm(); }
	inline T normSquared() const { return x*x + y*y; }
	inline T lengthSquared() const { return normSquared(); }

	inline void normalize()
	{
		T len = length();
		x /= len;
		y /= len;
	}

	inline Vec2 normalized() const
	{
		T len = length();
		return Vec2(x / len, y / len);
	}

	inline Vec2 opposite() const { return Vec2(-x, -y); }

	inline static void Scale(Vec2& v, T val) { v.scale(val); }
	inline static T Dot(const Vec2& v, const Vec2& u) { return v.dot(u); }
	inline static Vec2 Opposite(const Vec2& v) { return v.opposite(); }

	inline static T CosOfAngleBetween(const Vec2& v, const Vec2& u)
	{
		return v * u / (std::sqrt(v.normSquared() * u.normSquared()));
	}
	inline static T AngleBetween(const Vec2& v, const Vec2& u)
	{
		return std::acos(v * u / (std::sqrt(v.normSquared() * u.normSquared())));
	}

};

using Vec2F = Vec2<float>;
using Vec2D = Vec2<double>;