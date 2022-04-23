#include "pchheader.h"
#include "NURBSCurveProjector.h"

#include "../Curves/NURBSCurve.h"
#include "Tools/ComputationalGeometry/CGAlgorithms.h"

#include "Horyzen/Math/Vector.h"

#include "Horyzen/Math/Mat2.h"

#include "Horyzen/Logger.h"


#undef min
#undef max

namespace Horyzen::Geo {

	NURBSCurveProjector::NURBSCurveProjector(std::shared_ptr<NURBSCurve> p_curve)
		: m_Curve(p_curve)
	{
		auto bezierPatches = m_Curve->GetBezierControlPoints();
		for (auto& patch : bezierPatches) {
			BuildBezoutMatrix(patch);
			BuildConvexHull(patch);
		}
	}

	f64 NURBSCurveProjector::GetAlgebraicLevelSetValue(const Vec2D& p_point)
	{
		//const u64 n = m_BezierEntries[0].MCalculated.getDimension();

		//auto& MCalc = m_BezierEntries[0].MCalculated;
		//auto& M = m_BezierEntries[0].M;

		//for (size_t i = 0; i < n; ++i) {
		//	for (size_t j = 0; j < n; ++j) {
		//		MCalc(i, j) = M(i, j).x * p_x + M(i, j).y * p_y + M(i, j).w;
		//	}
		//}

		//return MCalc.det();
		return f64();
	}

	f64 NURBSCurveProjector::GetDistance2ndOrderApproximation(const Vec2D& p_point)
	{
		f64 res = GetDistanceForPatch2ndOrder(p_point, 0);
		for (size_t i = 1; i < m_BezierEntries.size(); ++i) {
			const f64 v = GetDistanceForPatch2ndOrder(p_point, i);
			//	res = res + v - std::sqrt(res * res + v * v);
			res = res + v - std::pow(res * res * res * res + v * v * v * v, 0.25);
		}
		return res;
	}


	f64 NURBSCurveProjector::GetDistance1stOrderApproximation(const Vec2D& p_point)
	{
		f64 res = GetDistanceForPatch1stOrder(p_point, 0);
		for (size_t i = 1; i < m_BezierEntries.size(); ++i) {
			const f64 v = GetDistanceForPatch1stOrder(p_point, i);
			res = res + v - std::sqrt(res * res + v * v);
		//	res = res + v - std::pow(res * res * res * res + v * v * v * v, 0.25);
		}
		return res;
	}

	f64 NURBSCurveProjector::GetDistanceForPatch1stOrder(const Vec2D& p_point, u64 p_index)
	{
		const u64 n = m_BezierEntries[p_index].M.getDimension();

		auto& M  = m_BezierEntries[p_index].M;
		auto& Mx = m_BezierEntries[p_index].Mx;
		auto& My = m_BezierEntries[p_index].My;
		auto& Mw = m_BezierEntries[p_index].Mw;
		auto& MI = m_BezierEntries[p_index].MI;
		auto& MIMx = m_BezierEntries[p_index].MIMx;
		auto& MIMy = m_BezierEntries[p_index].MIMy;

		for (size_t i = 0; i < n; ++i) {
			for (size_t j = 0; j < n; ++j) {
				M(i, j) = Mx(i, j) * p_point.x + My(i, j) * p_point.y + Mw(i, j);
			}
		}

		//............................................
		// Using preallocated buffers to avoid memory allocations
		SquareMatrix::Inverse(M, MI);
		SquareMatrix::Multiply(MI, Mx, MIMx);
		SquareMatrix::Multiply(MI, My, MIMy);

		const Vec2D g(MIMx.trace(), MIMy.trace()); // g = grad(M) / det(M) 
		const f64 d = 1.0 / g.length();            // Approximated distance to the implicit curve

		const f64 phi = m_BezierEntries[p_index].ConvexHullFunction(p_point).first; // Approximated distance
		                                                                            // to the convex hull

		return std::sqrt(d * d + (std::abs(phi) - phi) * (std::abs(phi) - phi) / 4);
	//	return std::sqrt(d * d + (std::sqrt(phi * phi + d * d * d * d) - phi) * (std::sqrt(phi * phi + d * d * d * d) - phi) / 4);
	//	return -phi;
	//	return d;
	}

	f64 NURBSCurveProjector::GetDistanceForPatch2ndOrder(const Vec2D& p_point, u64 p_index)
	{
		const u64 dim = m_BezierEntries[p_index].M.getDimension();

		auto& M = m_BezierEntries[p_index].M;
		auto& Mx = m_BezierEntries[p_index].Mx;
		auto& My = m_BezierEntries[p_index].My;
		auto& Mw = m_BezierEntries[p_index].Mw;
		auto& MI = m_BezierEntries[p_index].MI;
		auto& MIMx = m_BezierEntries[p_index].MIMx;
		auto& MIMy = m_BezierEntries[p_index].MIMy;
		auto& B = m_BezierEntries[p_index].MatBuf;

		for (size_t i = 0; i < dim; ++i) {
			for (size_t j = 0; j < dim; ++j) {
				M(i, j) = Mx(i, j) * p_point.x + My(i, j) * p_point.y + Mw(i, j);
			}
		}

		//.......................................................
        // Using preallocated buffers to avoid memory allocations
		SquareMatrix::Inverse(M, MI);
		SquareMatrix::Multiply(MI, Mx, MIMx);
		SquareMatrix::Multiply(MI, My, MIMy);

		const Vec2D g(MIMx.trace(), MIMy.trace()); // g = grad(M) / det(M) 
		const f64 d = 1.0 / g.length();            // Approximated distance to the implicit curve

		//.......................................................
		// Filling Hessian matrix
		const f64 trMIMx = MIMx.trace(); // trace(M^-1 * Mx)
		const f64 trMIMy = MIMy.trace(); // trace(M^-1 * My)

		SquareMatrix::Multiply(MIMx, MIMx, B); // Writing to buffer B (M^-1 * Mx)^2
		const f64 trMIMxMIMx = B.trace();      // trace((M^-1 * Mx)^2)
		SquareMatrix::Multiply(MIMy, MIMy, B); // Writing to buffer B (M^-1 * My)^2
		const f64 trMIMyMIMy = B.trace();      // trace((M^-1 * Mx)^2)
		SquareMatrix::Multiply(MIMx, MIMy, B); // Writing to buffer B (M^-1 * Mx)*(M^-1 * My)
		const f64 trMIMxMIMy = B.trace();      // trace((M^-1 * Mx)*(M^-1 * My))

		Mat2D H; // Hessian
		H.m00 = trMIMx * trMIMx - trMIMxMIMx;
		H.m11 = trMIMy * trMIMy - trMIMyMIMy;
		H.m01 = H.m10 = trMIMx * trMIMy - trMIMxMIMy;

		const Vec2D Hg = H * g;
		const Vec2D grad_d = g.scaled(d) - Hg.scaled(d*d*d);  // Gradient of approximated distance field
		const Vec2D n = grad_d.normalized();                  // Approximated normal vector
		
		const f64 nHn = n * (H * n);

		const f64 Mdet = M.det();
		const f64 Gn = Mdet * (g * n);
		
		const f64 dquad1 = (Gn + std::sqrt(Gn*Gn - 2*Mdet*nHn)) / nHn;
		const f64 dquad2 = (Gn - std::sqrt(Gn*Gn - 2*Mdet*nHn)) / nHn;

		f64 dSecondOrder;   // Distance to the curve (second order approximation)

		if (dquad1 > 0) dSecondOrder = dquad1;
		else            dSecondOrder = dquad2;

		if (dquad1 > 0 && dquad2 > 0) {
			dSecondOrder = std::min(dquad1, dquad2);
		}

		const f64 f = dSecondOrder;
		const f64 phi = m_BezierEntries[p_index].ConvexHullFunction(p_point).first;

		return std::sqrt(f * f + (std::abs(phi) - phi) * (std::abs(phi) - phi) / 4);
	//	return std::sqrt(f * f + (std::sqrt(phi * phi + f * f * f * f) - phi) * (std::sqrt(phi * phi + f * f * f * f) - phi) / 4);
	//	return -phi;
	//	return f;
	}

	Vec2D NURBSCurveProjector::Project1stOrderApproximation(const Vec2D& p_point)
	{
		f64 bestDistance = GetDistanceForPatch1stOrder(p_point, 0);
		u64 bestIndex = 0;
		for (size_t i = 1; i < m_BezierEntries.size(); ++i) {
			const f64 d = GetDistanceForPatch1stOrder(p_point, i);
			if (d < bestDistance) {
				bestDistance = d;
				bestIndex = i;
			}
		}
		return ProjectOnPatch1stOrderApproximation(p_point, bestIndex);
	}

	Vec2D NURBSCurveProjector::Project2ndOrderApproximation(const Vec2D& p_point)
	{
		f64 bestDistance = GetDistanceForPatch1stOrder(p_point, 0);
		u64 bestIndex = 0;
		for (size_t i = 1; i < m_BezierEntries.size(); ++i) {
			const f64 d = GetDistanceForPatch1stOrder(p_point, i);
			if (d < bestDistance) {
				bestDistance = d;
				bestIndex = i;
			}
		}
		return ProjectOnPatch2ndOrderApproximation(p_point, bestIndex);
	}

	Vec2D NURBSCurveProjector::ProjectOnPatch1stOrderApproximation(const Vec2D& p_point, u64 p_index)
	{
		const u64 dim = m_BezierEntries[p_index].M.getDimension();

		auto& M = m_BezierEntries[p_index].M;
		auto& Mx = m_BezierEntries[p_index].Mx;
		auto& My = m_BezierEntries[p_index].My;
		auto& Mw = m_BezierEntries[p_index].Mw;
		auto& MI = m_BezierEntries[p_index].MI;
		auto& MIMx = m_BezierEntries[p_index].MIMx;
		auto& MIMy = m_BezierEntries[p_index].MIMy;
		auto& B = m_BezierEntries[p_index].MatBuf;

		//.......................................................
		// Using preallocated buffers to avoid memory allocations
		SquareMatrix::Inverse(M, MI);
		SquareMatrix::Multiply(MI, Mx, MIMx);
		SquareMatrix::Multiply(MI, My, MIMy);

		const Vec2D g(MIMx.trace(), MIMy.trace()); // g = grad(M) / det(M) 
		f64 d = 1.0 / g.length();                  // Approximated distance to the implicit curve

		//.......................................................
		// Filling Hessian matrix
		const f64 trMIMx = MIMx.trace(); // trace(M^-1 * Mx)
		const f64 trMIMy = MIMy.trace(); // trace(M^-1 * My)

		SquareMatrix::Multiply(MIMx, MIMx, B); // Writing to buffer B (M^-1 * Mx)^2
		const f64 trMIMxMIMx = B.trace();      // trace((M^-1 * Mx)^2)
		SquareMatrix::Multiply(MIMy, MIMy, B); // Writing to buffer B (M^-1 * My)^2
		const f64 trMIMyMIMy = B.trace();      // trace((M^-1 * Mx)^2)
		SquareMatrix::Multiply(MIMx, MIMy, B); // Writing to buffer B (M^-1 * Mx)*(M^-1 * My)
		const f64 trMIMxMIMy = B.trace();      // trace((M^-1 * Mx)*(M^-1 * My))

		Mat2D H; // Hessian
		H.m00 = trMIMx * trMIMx - trMIMxMIMx;
		H.m11 = trMIMy * trMIMy - trMIMyMIMy;
		H.m01 = H.m10 = trMIMx * trMIMy - trMIMxMIMy;

		const Vec2D Hg = H * g;
		Vec2D grad_d = g.scaled(d) - Hg.scaled(d * d * d);  // Gradient of approximated distance field
		Vec2D n = grad_d.normalized();                      // Approximated normal vector

		const auto convexHullValues = m_BezierEntries[p_index].ConvexHullFunction(p_point);
		const f64 phi = convexHullValues.first;
		const Vec2D grad_phi = -convexHullValues.second;

		//.......................................................
		// Correction procedure
		//if (phi < 0) {
		//	const Vec2D newGrad_d = (n.scaled(2) + grad_phi.scaled(2)).scaled(1.0 / std::sqrt(d * d + phi * phi));
		//	n = newGrad_d.normalized();

		//	grad_d = grad_d + grad_phi - (grad_d.scaled(d) + grad_phi.scaled(phi)).scaled(1.0 / std::sqrt(d*d + phi*phi));

		//	n = grad_d.normalized();
		//	d = std::sqrt(d * d + phi * phi);
		//}

		Vec2D o_footPoint = p_point - n.scaled(d);
		return o_footPoint;
	}

	Vec2D NURBSCurveProjector::ProjectOnPatch2ndOrderApproximation(const Vec2D& p_point, u64 p_index)
	{
		const u64 n = m_BezierEntries[p_index].M.getDimension();

		auto& MCalc = m_BezierEntries[p_index].M;
		auto& M = m_BezierEntries[p_index].M;
		auto& Mx = m_BezierEntries[p_index].Mx;
		auto& My = m_BezierEntries[p_index].My;


		SquareMatrix MI = MCalc.inversed();

		f64 g1 = (MI * Mx).trace();
		f64 g2 = (MI * My).trace();

		f64 norm = std::sqrt(g1 * g1 + g2 * g2);

		const f64 d = 1.0 / norm;

		SquareMatrix MIMx = MI * Mx;
		SquareMatrix MIMy = MI * My;

		const f64 H00 = g1 * g1 - (MIMx * MIMx).trace();
		const f64 H11 = g2 * g2 - (MIMy * MIMy).trace();

		const f64 H01 = g1 * g2 - (MIMx * MIMy).trace();
		const f64 H10 = H01;

		const f64 Hg0 = H00 * g1 + H01 * g2;
		const f64 Hg1 = H10 * g1 + H11 * g2;

		const f64 gradD0 = g1 / norm - Hg0 / (norm * norm * norm);
		const f64 gradD1 = g2 / norm - Hg1 / (norm * norm * norm);

		const f64 gradDnorm = std::sqrt(gradD0 * gradD0 + gradD1 * gradD1);

		const f64 phi = m_BezierEntries[p_index].ConvexHullFunction(p_point).first;

		const f64 dCorrected = std::sqrt(d*d + (std::abs(phi) - phi) * (std::abs(phi) - phi) /4);

		Vec2D o_footPoint;
		o_footPoint.x = p_point.x - dCorrected * gradD0 / gradDnorm;
		o_footPoint.y = p_point.y - dCorrected * gradD1 / gradDnorm;

		return o_footPoint;
	}

	void NURBSCurveProjector::BuildBezoutMatrix(const std::vector<Vec4D>& p_bezierPatch)
	{
		switch (p_bezierPatch.size() - 1) {
			case 2:  BuildBezoutMatrixDegree2(p_bezierPatch); return;
			case 3:  BuildBezoutMatrixDegree3(p_bezierPatch); return;
			default: BuildBezoutMatrixGeneral(p_bezierPatch); return;
		}
	}

	void NURBSCurveProjector::BuildBezoutMatrixDegree2(const std::vector<Vec4D>& p_bezierPatch)
	{
		auto& P = p_bezierPatch;
		constexpr u64 n = 2;

		m_BezierEntries.emplace_back(BezierEntry(n));

		auto& bezierEntry = m_BezierEntries[m_BezierEntries.size() - 1];
		auto& Mx = bezierEntry.Mx;
		auto& My = bezierEntry.My;
		auto& Mw = bezierEntry.Mw;

		const f64 a0 = P[0].w * P[0].x;
		const f64 a1 = 2 * (P[1].w * P[1].x - P[0].w * P[0].x);
		const f64 a2 = P[0].w * P[0].x - 2 * P[1].w * P[1].x + P[2].w * P[2].x;

		const f64 b0 = P[0].w * P[0].y;
		const f64 b1 = 2 * (P[1].w * P[1].y - P[0].w * P[0].y);
		const f64 b2 = P[0].w * P[0].y - 2 * P[1].w * P[1].y + P[2].w * P[2].y;

		const f64 d0 = P[0].w;
		const f64 d1 = 2 * (P[1].w - P[0].w);
		const f64 d2 = P[0].w - 2 * P[1].w + P[2].w;

		Mx(0, 0) = b0 * d1 - b1 * d0;
		Mx(1, 1) = b1 * d2 - b2 * d1;
		Mx(0, 1) = Mx(1, 0) = b0 * d2 - b2 * d0;

		My(0, 0) = a1 * d0 - a0 * d1;
		My(1, 1) = a2 * d1 - a1 * d2;
		My(0, 1) = My(1, 0) = a2 * d0 - a0 * d2;

		Mw(0, 0) = a0 * b1 - a1 * b0;
		Mw(1, 1) = a1 * b2 - a2 * b1;
		Mw(0, 1) = Mw(1, 0) = a0 * b2 - a2 * b0;
	}

	void NURBSCurveProjector::BuildBezoutMatrixDegree3(const std::vector<Vec4D>& p_bezierPatch)
	{
		auto& P = p_bezierPatch;
		constexpr u64 n = 3;

		m_BezierEntries.emplace_back(BezierEntry(n));

		auto& bezierEntry = m_BezierEntries[m_BezierEntries.size() - 1];
		auto& Mx = bezierEntry.Mx;
		auto& My = bezierEntry.My;
		auto& Mw = bezierEntry.Mw;

		const f64 a0 = P[0].w * P[0].x;
		const f64 a1 = 3 * (P[1].w * P[1].x - P[0].w * P[0].x);
		const f64 a2 = 3 * (P[0].w * P[0].x - 2 * P[1].w * P[1].x + P[2].w * P[2].x);
		const f64 a3 = P[3].w * P[3].x + 3 * P[1].w * P[1].x - 3 * P[2].w * P[2].x - P[0].w * P[0].x;

		const f64 b0 = P[0].w * P[0].y;
		const f64 b1 = 3 * (P[1].w * P[1].y - P[0].w * P[0].y);
		const f64 b2 = 3 * (P[0].w * P[0].y - 2 * P[1].w * P[1].y + P[2].w * P[2].y);
		const f64 b3 = P[3].w * P[3].y + 3 * P[1].w * P[1].y - 3 * P[2].w * P[2].y - P[0].w * P[0].y;

		const f64 d0 = P[0].w;
		const f64 d1 = 3 * (P[1].w - P[0].w);
		const f64 d2 = 3 * (P[0].w - 2 * P[1].w + P[2].w);
		const f64 d3 = P[3].w + 3 * P[1].w - 3 * P[2].w - P[0].w;

		Mx(0, 0) = b0 * d1 - b1 * d0;
		Mx(1, 1) = b1 * d2 - b2 * d1;
		Mx(2, 2) = b2 * d3 - b3 * d2;
		Mx(0, 1) = Mx(1, 0) = b0 * d2 - b2 * d0;
		Mx(0, 2) = Mx(2, 0) = b0 * d3 - b3 * d0;
		Mx(1, 2) = Mx(2, 1) = b1 * d3 - b3 * d1;

		My(0, 0) = a1 * d0 - a0 * d1;
		My(1, 1) = a2 * d1 - a1 * d2;
		My(2, 2) = a3 * d2 - a2 * d3;
		My(0, 1) = My(1, 0) = a2 * d0 - a0 * d2;
		My(0, 2) = My(2, 0) = a3 * d0 - a0 * d3;
		My(1, 2) = My(2, 1) = a3 * d1 - a1 * d3;

		Mw(0, 0) = a0 * b1 - a1 * b0;
		Mw(1, 1) = a1 * b2 - a2 * b1;
		Mw(2, 2) = a2 * b3 - a3 * b2;
		Mw(0, 1) = Mw(1, 0) = a0 * b2 - a2 * b0;
		Mw(0, 2) = Mw(2, 0) = a0 * b3 - a3 * b0;
		Mw(1, 2) = Mw(2, 1) = a1 * b3 - a3 * b1;

		Mx(1, 1) += Mx(0, 2);
		My(1, 1) += My(0, 2);
		Mw(1, 1) += Mw(0, 2);
	}

	void NURBSCurveProjector::BuildBezoutMatrixGeneral(const std::vector<Vec4D>& p_bezierPatch)
	{
		auto& P = p_bezierPatch;
		const u64 n = P.size() - 1;

		m_BezierEntries.emplace_back(BezierEntry(n));

		auto& bezierEntry = m_BezierEntries[m_BezierEntries.size() - 1];
		auto& Mx = bezierEntry.Mx;
		auto& My = bezierEntry.My;
		auto& Mw = bezierEntry.Mw;

		std::vector<f64> a(n + 1, 0);
		std::vector<f64> b(n + 1, 0);
		std::vector<f64> d(n + 1, 0);

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
				Mx(i, j) = b[i] * d[j + 1] - b[j + 1] * d[i];
				My(i, j) = a[j + 1] * d[i] - a[i] * d[j + 1];
				Mw(i, j) = a[i] * b[j + 1] - a[j + 1] * b[i];
			}
		}

		//...........................................................
		// Applying recursive algorithm for Bezout resultant matrix
		for (size_t i = 1; i <= n - 2; ++i) {
			for (size_t j = i; j <= n - 2; ++j) {
				Mx(i, j) = Mx(i, j) + Mx(i - 1, j + 1);
				My(i, j) = My(i, j) + My(i - 1, j + 1);
				Mw(i, j) = Mw(i, j) + Mw(i - 1, j + 1);
			}
		}

		//...........................................................
		// Filling lower left triangle by symmetry
		for (size_t i = 1; i <= n - 1; ++i) {
			for (size_t j = 0; j < i; ++j) {
				Mx(i, j) = Mx(j, i);
				My(i, j) = My(j, i);
				Mw(i, j) = Mw(j, i);
			}
		}
	}

	void NURBSCurveProjector::BuildConvexHull(const std::vector<Vec4D>& p_bezierPatch)
	{
		auto& P = p_bezierPatch;

		const u64 N = P.size();

		const Vec2D firstPoint = P[0].xy();
		const Vec2D lastPoint = P[N - 1].xy();
		const f64 len = (lastPoint - firstPoint).length();

		const Line line(firstPoint, lastPoint);
		const f64 epsilon = 0.1;

		// Check if almost line
		bool almostLine = true;
		for (size_t i = 1; i < N - 1; ++i) {
			const f64 di = line(P[i].xy());
			if (std::abs(di) / len > epsilon) {
				almostLine = false;
			}
		}

		if (almostLine) {
			auto f = [firstPoint, lastPoint](const Vec2D& p) {
				f64 x1 = firstPoint.x;
				f64 x2 = lastPoint.x;
				f64 y1 = firstPoint.y;
				f64 y2 = lastPoint.y;
				f64 xc = (x1 + x2) / 2;
				f64 yc = (y1 + y2) / 2;
				f64 numerator = (p.x - x1) * (y2 - y1) - (p.y - y1) * (x2 - x1);
				f64 d = std::sqrt((x2 - x1) * (x2 - x1) + (y2 - y1) * (y2 - y1));
				f64 fval = numerator / d;
				f64 t = (d * d / 4 - (p.x - xc) * (p.x - xc) - (p.y - yc) * (p.y - yc)) / d;

				// TODO: Add gradient
				return std::make_pair(t, Vec2D());
			};
			m_BezierEntries[m_BezierEntries.size() - 1].ConvexHullFunction = f;
			return;
		}
		else {

			std::vector<Vec2D> points;
			for (size_t i = 0; i < P.size(); ++i) {
				points.push_back(P[i].xy());
			}

			ConvexHull convexHull;
			auto convexHullPoints = CGAlgorithms::FindConvexHull2DJarvisMarch(points);
			const u64 n = convexHullPoints.size();
			for (size_t i = 0; i < convexHullPoints.size(); ++i) {
				convexHull.Lines.push_back(Line(convexHullPoints[i % n], convexHullPoints[(i + 1) % n]));
			}

			m_BezierEntries[m_BezierEntries.size() - 1].ConvexHullFunction = convexHull;
			return;
		}
	}

	u64 NURBSCurveProjector::Factorial(u64 n)
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
