#pragma once

#include "Horyzen/Core.h"
#include "Horyzen/Math/SquareMatrix.h"
#include "Horyzen/Math/Vec4.h"
#include "Horyzen/Math/Vec3.h"
#include "Horyzen/Math/Vec2.h"

namespace Horyzen::Geo {

	class NURBSCurve;
	class HORYZEN_OBJECT NURBSCurveProjector
	{
	public:
		
		NURBSCurveProjector(std::shared_ptr<NURBSCurve> p_curve);

		f64 GetAlgebraicLevelSetValue(const Vec2D& p_point);
		f64 GetDistance1stOrderApproximation(const Vec2D& p_point);
		f64 GetDistance2ndOrderApproximation(const Vec2D& p_point);

		Vec2D Project1stOrderApproximation(const Vec2D& p_point);
		Vec2D Project2ndOrderApproximation(const Vec2D& p_point);

	private:

		struct BezoutEntry
		{
			f64 x, y, w;
		};

		struct BezoutMatrix
		{
			inline BezoutMatrix(size_t n)
				: entries(n*n)
				, dim(n)
			{}

			std::vector<BezoutEntry> entries;
			size_t dim;

			inline BezoutEntry& operator()(size_t i, size_t j)
			{
				return entries[i * dim + j];
			}

		};

		struct Line
		{
			inline Line(const Vec2D& p1, const Vec2D& p2)
				: N(p1.y - p2.y, p2.x - p1.x)
				, b(p1.x*p2.y - p2.x*p1.y)
			{
				//.........................................
				// Normalizing the coefficients
				const f64 normFactor = std::sqrt((p1.y - p2.y) * (p1.y - p2.y) +
					                             (p2.x - p1.x) * (p2.x - p1.x));
				N.scale(1.0 / normFactor);
				b /= normFactor;
			}

			Vec2D N; // Normal vector to the line
			f64 b;   // Offset from the origin

			inline f64 operator()(const Vec2D& p_point) const
			{
				return N.x * p_point.x + N.y * p_point.y + b;
			}

			inline f64 dx() const { return N.x; }
			inline f64 dy() const { return N.y; }
			inline Vec2D grad() const { return { N.x, N.y }; }
		};

		struct ConvexHull
		{
			std::vector<Line> Lines;

			inline std::pair<f64, Vec2D> operator()(const Vec2D& p_point)
			{
				const u64 N = Lines.size();
				f64 res = Lines[0](p_point);
				for (u64 i = 1; i < N; ++i) {
					f64 v = Lines[i](p_point);
				//	res = res + v - std::sqrt(res * res + v * v);
					const f64 p = 2;
					res = res + v - std::pow(std::pow(res, p) + std::pow(v, p), 1.0 / p);
				}

				/*f64 gradX = Lines[0].dx(p_point);
				f64 gradY = Lines[0].dy(p_point);
				f64 s1 = Lines[0](p_point);

				for (u64 i = 1; i < Lines.size(); ++i) {
					f64 vX = Lines[i].dx(p_point);
					f64 vY = Lines[i].dy(p_point);
					f64 s2 = Lines[i](p_point);
					gradX = gradX + vX - (s1 * gradX + s2 * vX) / std::sqrt(s1*s1 + s2*s2);
					gradY = gradY + vY - (s1 * gradY + s2 * vY) / std::sqrt(s1*s1 + s2*s2);
					s1 = s2;
				}*/

				Vec2D grad = Lines[0].grad();
				f64 s1 = Lines[0](p_point);
				for (u64 i = 1; i < Lines.size(); ++i) {
					Vec2D g = Lines[i].grad();
					f64 s2 = Lines[i](p_point);
					grad = grad + g - (grad.scaled(s1) + g.scaled(s2)).scaled(1.0 / std::sqrt(s1*s1 + s2*s2));
					s1 = s2;
				}


				return std::make_pair(res, grad);
			}
		};

		struct BezierEntry
		{
			inline BezierEntry(u64 p_dimension)
				: M(p_dimension)
				, Mx(p_dimension)
				, My(p_dimension)
				, Mw(p_dimension)
				, MI(p_dimension)
				, MIMx(p_dimension)
				, MIMy(p_dimension)
				, MatBuf(p_dimension)
			{}

			SquareMatrix M;    // Bezout Matrix M = M_x*x + M_y*y + M_w
			SquareMatrix Mx;   // M_x part
			SquareMatrix My;   // M_y part
			SquareMatrix Mw;   // M_w part

			// Next four matrixes are kept here to act like buffers
			// in order to avoid unnecessary dynamical memory allocations

			SquareMatrix MI;     // M^-1       buffer
			SquareMatrix MIMx;   // M^-1 * M_x buffer
			SquareMatrix MIMy;   // M^-1 * M_y buffer
			SquareMatrix MatBuf; // General buffer for some multiplications 

			std::function<std::pair<f64, Vec2D>(const Vec2D& p_point)> ConvexHullFunction;
		};

		void BuildBezoutMatrix(const std::vector<Vec4D>& p_bezierPatch);
		void BuildBezoutMatrixDegree2(const std::vector<Vec4D>& p_bezierPatch);
		void BuildBezoutMatrixDegree3(const std::vector<Vec4D>& p_bezierPatch);
		void BuildBezoutMatrixGeneral(const std::vector<Vec4D>& p_bezierPatch);

		void BuildConvexHull(const std::vector<Vec4D>& p_bezierPatch);

		f64 GetDistanceForPatch1stOrder(const Vec2D& p_point, u64 p_index);
		f64 GetDistanceForPatch2ndOrder(const Vec2D& p_point, u64 p_index);

		Vec2D ProjectOnPatch1stOrderApproximation(const Vec2D& p_point, u64 p_index);
		Vec2D ProjectOnPatch2ndOrderApproximation(const Vec2D& p_point, u64 p_index);

		// TODO : get rid of this crap
		u64 Factorial(u64 n);

	private:

		std::shared_ptr<NURBSCurve> m_Curve;
		std::vector<BezierEntry> m_BezierEntries;

	};

}