#pragma once

template <typename T>
struct Vec2;

template <typename T>
struct Mat2
{
	union
	{
		struct
		{
			// This layout is actually transposed
			T m0, m1,
		      m2, m3;
		};
		struct
		{
			// This layout is actually transposed
			T m00, m10,
			  m01, m11;
		};
		struct
		{
			T colX[2], colY[2];
		};
		struct
		{
			T rawData[4];
		};
	};

	inline Mat2() : m0(0), m1(0), m2(0), m3(0) {}
	inline Mat2(T val) : m0(val), m1(0), m2(0), m3(val) {}
	inline Mat2(const Mat2& M) = default;

	Mat2 operator*(const Mat2& M) const
	{
		Mat2 I;
		I.m0 = m0 * M.m0 + m2 * M.m1;
		I.m1 = m1 * M.m0 + m3 * M.m1;
		I.m2 = m0 * M.m2 + m2 * M.m3;
		I.m3 = m1 * M.m2 + m3 * M.m3;

		return I;
	}

	inline Vec2<T> operator*(const Vec2<T>& v) const
	{
		Vec2<T> w;
		w.x = m0 * v.x + m2 * v.y;
		w.y = m1 * v.x + m3 * v.y;

		return w;
	}

	void inverse()
	{
		Mat2 I = inversed();
		*this = I;
	}

	Mat2 inversed() const
	{
		Mat2 I;
		T det = determinant();
		I.m00 = m11 / det;
		I.m11 = m00 / det;
		I.m01 = -m01 / det;
		I.m10 = -m10 / det;
	}

	inline T determinant() const
	{
		return m00 * m11 - m10 * m01;
	}

};

template<typename T>
inline Vec2<T> operator*(const Vec2<T>& v, const Mat2<T>& M)
{
	return Vec2<T>(v.x * M.m00 + v.y * M.m10, v.x * M.m10 + v.y * M.m11);
}


using Mat2F = Mat2<float>;
using Mat2D = Mat2<double>;