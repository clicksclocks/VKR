#pragma once

#include "BasicMath.h"

struct Line
{
	inline Line(const Vec3D& p_point,
		        const Vec3D& p_directionalVector)
		: aX(p_directionalVector.x)
		, aY(p_directionalVector.y)
		, aZ(p_directionalVector.z)
		, x0(p_point.x)
		, y0(p_point.y)
		, z0(p_point.z)
	{}

	double aX, aY, aZ;
	double x0, y0, z0;

	inline Vec3D operator()(double p_t)
	{
		return Vec3D(
			x0 + aX*p_t,
			y0 + aY*p_t,
			z0 + aZ*p_t
		);
	}
};

struct Plane
{
	inline Plane()
		: A(0), B(0), C(0), D(0)
	{}

	inline Plane(Vec3D p_point, Vec3D p_normalVector)
		: A(p_normalVector.x)
		, B(p_normalVector.y)
		, C(p_normalVector.z)
		, D(-p_point.x*A - p_point.y*B - p_point.z*C)
	{}
 
	double A, B, C, D;

	inline double FindParameterOfIntersection(const Line& p_line)
	{
		double numerator = A*p_line.x0 + B*p_line.y0 + C*p_line.z0 + D;
		double denominator = A*p_line.aX + B*p_line.aY + C*p_line.aZ;
		return -numerator * denominator;
	}
};

