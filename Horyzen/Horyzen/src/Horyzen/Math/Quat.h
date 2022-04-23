#pragma once

#include <cmath>

template <typename T>
struct Vec3;

template <typename T>
struct Quat
{
	T x, y, z, w;

	Quat();
	Quat(T p_x, T p_y, T p_z, T p_w);

	template <typename U>
	Quat& operator+=(const Quat<U>& q);

	template <typename U>
	Quat& operator-=(const Quat<U>& q);

	template <typename U>
	Quat& operator*=(U val);

	Quat operator+() const;
	Quat operator-() const;

	template <typename U>
	Quat operator+(const Quat<U>& q) const;

	template <typename U>
	Quat operator-(const Quat<U>& q) const;

	template <typename U>
	Quat operator*(const Quat<U>& q) const;

	template <typename U>
	Quat operator*(U val) const;

	Quat conjugated() const;
	void conjugate();

	template <typename U>
	Quat scaled(U val) const;
	template <typename U>
	void scale(U val);

	T norm() const;
	T normSquared() const;

	Quat normalized() const;
	void normalize();

	Quat inversed() const;
	void inverse();

	template <typename U>
	T dot(const Quat<U>& q) const;
	static T dot(const Quat<T>& q1, const Quat<T>& q2);

	static T cosOfAngleBetween(const Quat<T>& q1, const Quat<T>& q2);
	static T angleBetween(const Quat<T>& q1, const Quat<T>& q2);

	template <typename V, typename U>
	static Quat fromAxisAndAngle(const Vec3<V>& axis, U angle);

};


template <typename T>
Quat<T>::Quat(T p_x, T p_y, T p_z, T p_w)
	: x(p_x), y(p_y), z(p_z), w(p_w)
{}

template <typename T>
Quat<T>::Quat()
	: x(0), y(0), z(0), w(1)
{}


template <typename T>
template <typename U>
Quat<T>& Quat<T>::operator+=(const Quat<U>& q)
{
	x += static_cast<T>(q.x);
	y += static_cast<T>(q.y);
	z += static_cast<T>(q.z);
	w += static_cast<T>(q.w);
	return *this;
}

template <typename T>
template <typename U>
Quat<T>& Quat<T>::operator-=(const Quat<U>& q)
{
	x -= static_cast<T>(q.x);
	y -= static_cast<T>(q.y);
	z -= static_cast<T>(q.z);
	w -= static_cast<T>(q.w);
	return *this;
}

template <typename T>
template <typename U>
Quat<T>& Quat<T>::operator*=(U val)
{
	x *= static_cast<T>(val);
	y *= static_cast<T>(val);
	z *= static_cast<T>(val);
	w *= static_cast<T>(val);
	return *this;
}

template <typename T>
Quat<T> Quat<T>::operator+() const
{
	return Quat(x, y, z, w);
}

template <typename T>
Quat<T> Quat<T>::operator-() const
{
	return Quat(-x, -y, -z, -w);
}

template <typename T>
template <typename U>
Quat<T> Quat<T>::operator+(const Quat<U>& q) const
{
	return Quat(
		x + static_cast<T>(q.x),
		y + static_cast<T>(q.y),
		z + static_cast<T>(q.z),
		w + static_cast<T>(q.w)
	);
}

template <typename T>
template <typename U>
Quat<T> Quat<T>::operator-(const Quat<U>& q) const
{
	return Quat(
		x - static_cast<T>(q.x),
		y - static_cast<T>(q.y),
		z - static_cast<T>(q.z),
		w - static_cast<T>(q.w)
	);
}

template <typename T>
template <typename U>
Quat<T> Quat<T>::operator*(const Quat<U>& q) const
{
	return Quat(
		w * static_cast<T>(q.x) + x * static_cast<T>(q.w) + y * static_cast<T>(q.z) - z * static_cast<T>(q.y),
		w * static_cast<T>(q.y) + y * static_cast<T>(q.w) + z * static_cast<T>(q.x) - x * static_cast<T>(q.z),
		w * static_cast<T>(q.z) + z * static_cast<T>(q.w) + x * static_cast<T>(q.y) - y * static_cast<T>(q.x),
		w * static_cast<T>(q.w) - x * static_cast<T>(q.x) - y * static_cast<T>(q.y) - z * static_cast<T>(q.z)
	);
}

template <typename T>
template <typename U>
Quat<T> Quat<T>::operator*(U val) const
{
	return Quat(
		x * static_cast<T>(val),
		y * static_cast<T>(val),
		z * static_cast<T>(val),
		w * static_cast<T>(val)
	);
}

template <typename T>
Quat<T> Quat<T>::conjugated() const
{
	return Quat(-x, -y, -z, w);
}

template <typename T>
template <typename U>
Quat<T> Quat<T>::scaled(U val) const
{
	return *this * val;
}

template <typename T>
template <typename U>
void Quat<T>::scale(U val)
{
	*this *= val;
}

template <typename T>
void Quat<T>::conjugate()
{
	x *= -1;
	y *= -1;
	z *= -1;
}

template <typename T>
T Quat<T>::norm() const
{
	return std::sqrt(
		w * w +
		x * x +
		y * y +
		z * z
	);
}

template <typename T>
T Quat<T>::normSquared() const
{
	return w * w + 
		   x * x + 
		   y * y + 
		   z * z;
}

template <typename T>
Quat<T> Quat<T>::normalized() const
{
	T n = norm();
	return Quat(
		x / n,
		y / n,
		z / n,
		w / n
	);
}

template <typename T>
void Quat<T>::normalize()
{
	T n = norm();
	x /= n;
	y /= n;
	z /= n;
	w /= n;
}

template <typename T>
Quat<T> Quat<T>::inversed() const
{
	T nsqr = normSquared();
	return Quat(
		-x / nsqr,
		-y / nsqr,
		-z / nsqr,
		 w / nsqr
	);
}

template <typename T>
void Quat<T>::inverse()
{
	T nsqr = normSquared();
	-x /= nsqr;
	-y /= nsqr;
	-z /= nsqr;
	 w /= nsqr;
}

template <typename T>
template <typename U>
T Quat<T>::dot(const Quat<U>& q) const
{
	return x * q.x + 
		   y * q.y + 
		   z * q.z + 
		   w * q.w;
}

template <typename T>
T Quat<T>::dot(const Quat<T>& q1, const Quat<T>& q2)
{
	return q1.x * q2.x + 
		   q1.y * q2.y + 
		   q1.z * q2.z + 
		   q1.w * q2.w;
}

template <typename T>
T Quat<T>::cosOfAngleBetween(const Quat<T>& q1, const Quat<T>& q2)
{
	return q1.dot(q2) / (std::sqrt(q1.normSquared() * q2.normSquared()));
}

template <typename T>
T Quat<T>::angleBetween(const Quat<T>& q1, const Quat<T>& q2)
{
	return std::acos(q1.dot(q2) / (std::sqrt(q1.normSquared() * q2.normSquared())));
}

template <typename T>
template <typename V, typename U>
Quat<T> Quat<T>::fromAxisAndAngle(const Vec3<V>& axis, U angle)
{
	T sinHalfTheta = std::sin(static_cast<T>(angle)/2);
	T cosHalfTheta = std::cos(static_cast<T>(angle)/2);
	return Quat(
		sinHalfTheta * static_cast<T>(axis.x),
		sinHalfTheta * static_cast<T>(axis.y),
		sinHalfTheta * static_cast<T>(axis.z),
		cosHalfTheta
	);
}

using QuatF = Quat<float>;
using QuatD = Quat<double>;