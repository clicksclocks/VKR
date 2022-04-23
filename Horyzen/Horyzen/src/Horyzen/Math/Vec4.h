#pragma once

#include <cmath>

template <typename T>
struct Vec2;

template <typename T>
struct Vec3;

template <typename T>
struct Vec4
{
	union
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
		struct { T rawData[4]; };
	};

	inline Vec4() : x(0), y(0), z(0), w(1) {}
	inline Vec4(T p_x, T p_y, T p_z, T p_w) : x(p_x), y(p_y), z(p_z), w(p_w) {}
	
	Vec4 wxyz() const { return Vec4(w, x, y, z); }
	Vec4 zxyw() const { return Vec4(z, x, y, w); }
	Vec4 wxzy() const { return Vec4(w, x, z, y); }
	Vec4 yxzw() const { return Vec4(y, x, z, w); }
	Vec4 zxwy() const { return Vec4(z, x, w, y); }
	Vec4 yxwz() const { return Vec4(y, x, w, z); }
	Vec4 wyxz() const { return Vec4(w, y, x, z); }
	Vec4 zyxw() const { return Vec4(z, y, x, w); }
	Vec4 wyzx() const { return Vec4(w, y, z, x); }
	Vec4 xyzw() const { return Vec4(x, y, z, w); }
	Vec4 zywx() const { return Vec4(z, y, w, x); }
	Vec4 xywz() const { return Vec4(x, y, w, z); }
	Vec4 wzxy() const { return Vec4(w, z, x, y); }
	Vec4 yzxw() const { return Vec4(y, z, x, w); }
	Vec4 wzyx() const { return Vec4(w, z, y, x); }
	Vec4 xzyw() const { return Vec4(x, z, y, w); }
	Vec4 yzwx() const { return Vec4(y, z, w, x); }
	Vec4 xzwy() const { return Vec4(x, z, w, y); }
	Vec4 zwxy() const { return Vec4(z, w, x, y); }
	Vec4 ywxz() const { return Vec4(y, w, x, z); }
	Vec4 zwyx() const { return Vec4(z, w, y, x); }
	Vec4 xwyz() const { return Vec4(x, w, y, z); }
	Vec4 ywzx() const { return Vec4(y, w, z, x); }
	Vec4 xwzy() const { return Vec4(x, w, z, y); }

	Vec3<T> xyz() const { return Vec3<T>(x, y, z); }
	Vec3<T> xyw() const { return Vec3<T>(x, y, w); }
	Vec3<T> xzy() const { return Vec3<T>(x, z, y); }
	Vec3<T> xzw() const { return Vec3<T>(x, z, w); }
	Vec3<T> xwy() const { return Vec3<T>(x, w, y); }
	Vec3<T> xwz() const { return Vec3<T>(x, w, z); }
	Vec3<T> yxz() const { return Vec3<T>(y, x, z); }
	Vec3<T> yxw() const { return Vec3<T>(y, x, w); }
	Vec3<T> yzx() const { return Vec3<T>(y, z, x); }
	Vec3<T> yzw() const { return Vec3<T>(y, z, w); }
	Vec3<T> ywx() const { return Vec3<T>(y, w, x); }
	Vec3<T> ywz() const { return Vec3<T>(y, w, z); }
	Vec3<T> zxy() const { return Vec3<T>(z, x, y); }
	Vec3<T> zxw() const { return Vec3<T>(z, x, w); }
	Vec3<T> zyx() const { return Vec3<T>(z, y, x); }
	Vec3<T> zyw() const { return Vec3<T>(z, y, w); }
	Vec3<T> zwx() const { return Vec3<T>(z, w, x); }
	Vec3<T> zwy() const { return Vec3<T>(z, w, y); }
	Vec3<T> wxy() const { return Vec3<T>(w, x, y); }
	Vec3<T> wxz() const { return Vec3<T>(w, x, z); }
	Vec3<T> wyx() const { return Vec3<T>(w, y, x); }
	Vec3<T> wyz() const { return Vec3<T>(w, y, z); }
	Vec3<T> wzx() const { return Vec3<T>(w, z, x); }
	Vec3<T> wzy() const { return Vec3<T>(w, z, y); }

	Vec2<T> xy() const { return Vec2<T>(x, y); }
	Vec2<T> xz() const { return Vec2<T>(x, z); }
	Vec2<T> xw() const { return Vec2<T>(x, w); }
	Vec2<T> yx() const { return Vec2<T>(y, x); }
	Vec2<T> yz() const { return Vec2<T>(y, z); }
	Vec2<T> yw() const { return Vec2<T>(y, w); }
	Vec2<T> zx() const { return Vec2<T>(z, x); }
	Vec2<T> zy() const { return Vec2<T>(z, y); }
	Vec2<T> zw() const { return Vec2<T>(z, w); }
	Vec2<T> wx() const { return Vec2<T>(w, x); }
	Vec2<T> wy() const { return Vec2<T>(w, y); }
	Vec2<T> wz() const { return Vec2<T>(w, z); }

	inline Vec4 operator*(T val) const { return Vec4(x * val, y * val, z * val, w * val); }

	inline Vec4 operator+() const { return Vec4(x, y, z, w); }
	inline Vec4 operator-() const { return Vec4(-x, -y, -z, -w); }

	inline Vec4 operator+(const Vec4& v) const { return Vec4(x + v.x, y + v.y, z + v.z, w + v.w); }
	inline Vec4 operator-(const Vec4& v) const { return Vec4(x - v.x, y - v.y, z - v.z, w - v.w); }


	inline void scale(T val) { *this *= val; }
	inline Vec4 scaled(T val) const { return *this * val; }

};

using Vec4F = Vec4<float>;
using Vec4D = Vec4<double>;
