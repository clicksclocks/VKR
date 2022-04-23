#include "pchheader.h"
#include "BezierCurveProjector.h"
#include "../Curves/BezierCurve.h"
#include <limits>

#undef min
#undef max

namespace Horyzen::Geo {



	BezierCurveProjector::BezierCurveProjector(std::shared_ptr<BezierCurve> p_curve)
		: m_Curve(p_curve),
		  m_M(p_curve->GetControlPoints().second.size()-1),
		  m_MCalculated(p_curve->GetControlPoints().second.size() - 1),
		  m_Mx(p_curve->GetControlPoints().second.size() - 1),
		  m_My(p_curve->GetControlPoints().second.size() - 1),
		  m_Mw(p_curve->GetControlPoints().second.size() - 1)
	{
		BuildBezoutMatrix();
		u64 n = p_curve->GetControlPoints().second.size() - 1;
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				m_Mx(i, j) = m_M(i, j).x;
				m_My(i, j) = m_M(i, j).y;
				m_Mw(i, j) = m_M(i, j).w;
			}
		}
		BuildConvexHull();
	}

	f64 BezierCurveProjector::GetAlgebraicLevelSetValue(f64 p_x, f64 p_y)
	{
		u64 n = m_Curve->GetControlPoints().second.size() - 1;
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				m_MCalculated(i, j) = m_M(i, j).x * p_x + m_M(i, j).y * p_y + m_M(i, j).w;
			}
		}

		return m_MCalculated.det();
	}

	f64 BezierCurveProjector::GetDistance(f64 p_x, f64 p_y)
	{
		u64 n = m_Curve->GetControlPoints().second.size() - 1;
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				m_MCalculated(i, j) = m_M(i, j).x * p_x + m_M(i, j).y * p_y + m_M(i, j).w;
			}
		}
		f64 Mdet = m_MCalculated.det();

		SquareMatrix MI = m_MCalculated.inversed();

		f64 g1 = (MI * m_Mx).trace();
		f64 g2 = (MI * m_My).trace();

		f64 norm = std::sqrt(g1*g1 + g2*g2);

		f64 sign = Mdet < 0.0 ? -1.0 : 1.0;

	//	return - sign / norm;

	//	const f64 phi = m_ConvexHullFunction(p_x, p_y);
		const f64 f   = 1.0 / norm;

		return f;
	//	return phi;
	//	return std::sqrt(f * f + (std::abs(phi) - phi) * (std::abs(phi) - phi) / 4);
	}

	void BezierCurveProjector::BuildBezoutMatrix()
	{
		switch (m_Curve->GetControlPoints().second.size() - 1) {
			case 2: BuildBezoutMatrixDegree2();  return;
			case 3: BuildBezoutMatrixDegree3();	 return;
			default: BuildBezoutMatrixGeneral(); return;
		}
	}

	void BezierCurveProjector::BuildBezoutMatrixDegree2()
	{
		auto& P = m_Curve->GetControlPoints().second;

		f64 a0 = P[0].w * P[0].x;
		f64 a1 = 2 * (P[1].w * P[1].x - P[0].w * P[0].x);
		f64 a2 = P[0].w * P[0].x - 2 * P[1].w * P[1].x + P[2].w * P[2].x;

		f64 b0 = P[0].w * P[0].y;
		f64 b1 = 2 * (P[1].w * P[1].y - P[0].w * P[0].y);
		f64 b2 = P[0].w * P[0].y - 2 * P[1].w * P[1].y + P[2].w * P[2].y;

		f64 d0 = P[0].w;
		f64 d1 = 2 * (P[1].w - P[0].w);
		f64 d2 = P[0].w - 2 * P[1].w + P[2].w;

		m_M(0, 0).x = b0 * d1 - b1 * d0;
		m_M(1, 1).x = b1 * d2 - b2 * d1;
		m_M(0, 1).x = m_M(1, 0).x = b0 * d2 - b2 * d0;

		m_M(0, 0).y = a1 * d0 - a0 * d1;
		m_M(1, 1).y = a2 * d1 - a1 * d2;
		m_M(0, 1).y = m_M(1, 0).y = a2 * d0 - a0 * d2;

		m_M(0, 0).w = a0 * b1 - a1 * b0;
		m_M(1, 1).w = a1 * b2 - a2 * b1;
		m_M(0, 1).w = m_M(1, 0).w = a0 * b2 - a2 * b0;
	}

	void BezierCurveProjector::BuildBezoutMatrixDegree3()
	{
		auto& P = m_Curve->GetControlPoints().second;

		f64 a0 = P[0].w * P[0].x;
		f64 a1 = 3 * (P[1].w * P[1].x - P[0].w * P[0].x);
		f64 a2 = 3 * (P[0].w * P[0].x - 2 * P[1].w * P[1].x + P[2].w * P[2].x);
		f64 a3 = P[3].w * P[3].x + 3 * P[1].w * P[1].x - 3 * P[2].w * P[2].x - P[0].w * P[0].x;

		f64 b0 = P[0].w * P[0].y;
		f64 b1 = 3 * (P[1].w * P[1].y - P[0].w * P[0].y);
		f64 b2 = 3 * (P[0].w * P[0].y - 2 * P[1].w * P[1].y + P[2].w * P[2].y);
		f64 b3 = P[3].w * P[3].y + 3 * P[1].w * P[1].y - 3 * P[2].w * P[2].y - P[0].w * P[0].y;

		f64 d0 = P[0].w;
		f64 d1 = 3 * (P[1].w - P[0].w);
		f64 d2 = 3 * (P[0].w - 2 * P[1].w + P[2].w);
		f64 d3 = P[3].w + 3 * P[1].w - 3 * P[2].w - P[0].w;

		m_M(0, 0).x = b0 * d1 - b1 * d0;
		m_M(1, 1).x = b1 * d2 - b2 * d1;
		m_M(2, 2).x = b2 * d3 - b3 * d2;
		m_M(0, 1).x = m_M(1, 0).x = b0 * d2 - b2 * d0;
		m_M(0, 2).x = m_M(2, 0).x = b0 * d3 - b3 * d0;
		m_M(1, 2).x = m_M(2, 1).x = b1 * d3 - b3 * d1;

		m_M(0, 0).y = a1 * d0 - a0 * d1;
		m_M(1, 1).y = a2 * d1 - a1 * d2;
		m_M(2, 2).y = a3 * d2 - a2 * d3;
		m_M(0, 1).y = m_M(1, 0).y = a2 * d0 - a0 * d2;
		m_M(0, 2).y = m_M(2, 0).y = a3 * d0 - a0 * d3;
		m_M(1, 2).y = m_M(2, 1).y = a3 * d1 - a1 * d3;

		m_M(0, 0).w = a0 * b1 - a1 * b0;
		m_M(1, 1).w = a1 * b2 - a2 * b1;
		m_M(2, 2).w = a2 * b3 - a3 * b2;
		m_M(0, 1).w = m_M(1, 0).w = a0 * b2 - a2 * b0;
		m_M(0, 2).w = m_M(2, 0).w = a0 * b3 - a3 * b0;
		m_M(1, 2).w = m_M(2, 1).w = a1 * b3 - a3 * b1;

		m_M(1, 1).x += m_M(0, 2).x;
		m_M(1, 1).y += m_M(0, 2).y;
		m_M(1, 1).w += m_M(0, 2).w;
	}

	void BezierCurveProjector::BuildBezoutMatrixGeneral()
	{
		auto& P = m_Curve->GetControlPoints().second;
		u64 n = P.size() - 1;

		std::vector<f64> a(n+1, 0);
		std::vector<f64> b(n+1, 0);
		std::vector<f64> d(n+1, 0);

		//...........................................................
		// Getting polynomial form of Bezier curve
		for (size_t j = 0; j <= n; ++j) {
			f64 factor = f64(Factorial(n)) / Factorial(n - j);
			for (size_t i = 0; i <= j; ++i) {
				f64 onePow = (i + j) % 2 == 0 ? 1.0 : -1.0;
				f64 denom = Factorial(i) * Factorial(j - i);
				a[j] += onePow * P[i].x * P[i].w / denom;
				b[j] += onePow * P[i].y * P[i].w / denom;
				d[j] += onePow * P[i].w / denom;
			}
			a[j] *= factor;
			b[j] *= factor;
			d[j] *= factor;
		}

		//...........................................................
		// Writing initial entries
		for (size_t i = 0; i < n; ++i) {
			for (size_t j = i; j < n; ++j) {
				m_M(i, j).x = b[i] * d[j + 1] - b[j + 1] * d[i];
				m_M(i, j).y = a[j + 1] * d[i] - a[i] * d[j + 1];
				m_M(i, j).w = a[i] * b[j + 1] - a[j + 1] * b[i];
			}
		}

		//...........................................................
		// Applying recursive algorithm for Bezout resultant matrix
		for (size_t i = 1; i <= n - 2; ++i) {
			for (size_t j = i; j <= n - 2; ++j) {
				m_M(i, j).x = m_M(i, j).x + m_M(i - 1, j + 1).x;
				m_M(i, j).y = m_M(i, j).y + m_M(i - 1, j + 1).y;
				m_M(i, j).w = m_M(i, j).w + m_M(i - 1, j + 1).w;
			}
		}

		//...........................................................
		// Filling lower left triangle by symmetry
		for (size_t i = 1; i <= n - 1; ++i) {
			for (size_t j = 0; j < i; ++j) {
				m_M(i, j) = m_M(j, i);
			}
		}

	}

	void BezierCurveProjector::BuildConvexHull()
	{
		switch (m_Curve->GetControlPoints().second.size() - 1) {
			case 2:  BuildConvexHullDegree2(); return;
			case 3:  BuildConvexHullDegree3(); return;
			default: BuildConvexHullGeneral(); return;
		}
	}

	void BezierCurveProjector::BuildConvexHullDegree2()
	{
		auto& P = m_Curve->GetControlPoints().second;

		Vec3D point1 = P[0].xyz();
		Vec3D point2 = P[2].xyz();

		Line line(point1, point2);

		f64 d = line(P[1].x, P[1].y);
		std::array<u64, 3> order;
		const f64 epsilon = 0.1;
		
		if (std::abs(d) / line.d < epsilon) {
			auto f = [point1, point2](f64 x, f64 y) {
				f64 x1 = point1.x;
				f64 x2 = point2.x;
				f64 y1 = point1.y;
				f64 y2 = point2.y;
				f64 xc = (x1 + x2) / 2;
				f64 yc = (y1 + y2) / 2;
				f64 numerator = (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1);
				f64 d = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				f64 fval = numerator / d;
				f64 t = (d * d / 4 - (x - xc) * (x - xc) - (y - yc) * (y - yc)) / d;
				f64 h = std::sqrt(fval * fval + (std::abs(t) - t) * (std::abs(t) - t) / 4);
				return h;
			};
			m_ConvexHullFunction = f;
			return;
		} else {
			if (d < 0) order = { 0, 1, 2 };
			else       order = { 0, 2, 1 };

			ConvexHull convexHull;
			u64 i = 0, j = 0;
			convexHull.Lines.push_back(Line(P[order[0]].xyz(), P[order[1]].xyz()));
			convexHull.Lines.push_back(Line(P[order[1]].xyz(), P[order[2]].xyz()));
			convexHull.Lines.push_back(Line(P[order[2]].xyz(), P[order[0]].xyz()));

			m_ConvexHullFunction = convexHull;
			return;
		}
	}

	void BezierCurveProjector::BuildConvexHullDegree3()
	{

	}

	void BezierCurveProjector::BuildConvexHullGeneral()
	{

	}

	u64 BezierCurveProjector::Factorial(u64 n)
	{
		if (n == 0) return 1;
		if (n == 1) return 1;
		
		u64 res = 1;
		for (size_t i = 2; i <= n; ++i) {
			res *= i;
		}
		return res;
	}

}