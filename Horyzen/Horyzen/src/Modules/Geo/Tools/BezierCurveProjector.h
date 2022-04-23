#pragma once

#include "Horyzen/Core.h"
#include "Horyzen/Math/SquareMatrix.h"
#include "Horyzen/Math/Vec3.h"

namespace Horyzen::Geo {

	class BezierCurve;
	class HORYZEN_OBJECT BezierCurveProjector
	{
	public:

		BezierCurveProjector(std::shared_ptr<BezierCurve> p_curve);

		f64 GetAlgebraicLevelSetValue(f64 p_x, f64 p_y);
		f64 GetDistance(f64 p_x, f64 p_y);

	public:

		struct BezoutEntry
		{
			f64 x, y, w;
		};

		struct Line
		{
			inline Line(const Vec3D& p1, const Vec3D& p2)
				: Nx(p1.y - p2.y),
				  Ny(p2.x - p1.x),
				  b(p1.x* p2.y - p2.x * p1.y),
				  d(std::sqrt((p1.y - p2.y)* (p1.y - p2.y) + (p2.x - p1.x) * (p2.x - p1.x)))
			{}

			f64 Nx;
			f64 Ny;
			f64 b;
			f64 d;

			inline f64 operator()(f64 p_x, f64 p_y)
			{
				return (Nx * p_x + Ny * p_y + b) / d;
			}
		};

		struct ConvexHull
		{
			std::vector<Line> Lines;

			inline f64 operator()(f64 p_x, f64 p_y)
			{
				f64 res = Lines[0](p_x, p_y);
				for (size_t i = 1; i < Lines.size(); ++i) {
					f64 v = Lines[i](p_x, p_y);
					res = res + v - std::sqrt(res * res + v * v);
				}
				return res;
				//f64 v = 0;
				//f64 vv = 0;
				//for (size_t i = 0; i < Lines.size(); ++i) {
				//	const f64 val = Lines[i](p_x, p_y);
				//	v += val;
				//	vv += val * val;
				//}
				//return v - std::sqrt(vv);
			}
		};

		struct BezoutMatrix
		{
			inline BezoutMatrix(size_t n)
				: entries(n*n),
				  dim(n)
			{}

			std::vector<BezoutEntry> entries;
			size_t dim;

			inline BezoutEntry& operator()(size_t i, size_t j)
			{
				return entries[i * dim + j];
			}

		};

		void BuildBezoutMatrix();
		void BuildBezoutMatrixDegree2();
		void BuildBezoutMatrixDegree3();
		void BuildBezoutMatrixGeneral();
		
		void BuildConvexHull();
		void BuildConvexHullDegree2();
		void BuildConvexHullDegree3();
		void BuildConvexHullGeneral();

		// TODO : get rid of this crap
		u64 Factorial(u64 n);

	private:

		std::shared_ptr<BezierCurve> m_Curve;

		BezoutMatrix m_M;
		SquareMatrix m_MCalculated;
		SquareMatrix m_Mx;
		SquareMatrix m_My;
		SquareMatrix m_Mw;
		std::function<f64(f64, f64)> m_ConvexHullFunction;

	};

}

