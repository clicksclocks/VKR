#pragma once

template <typename T>
struct Vec3;

template <typename T>
struct Vec4;

template <typename T>
struct Mat4
{
	union
	{
		struct
		{
			// This layout is actually transposed
			T m0,  m1,  m2,  m3,
			  m4,  m5,  m6,  m7,
			  m8,  m9,  m10, m11,
			  m12, m13, m14, m15;
		};
		struct
		{
			T colX[4], colY[4], colZ[4], colW[4];
		};
		struct
		{
			T rawData[16];
		};
	};

	Mat4();
	Mat4(T val);
	Mat4(const Mat4& M) = default;


	template <typename U> void scaleX(U val);
	template <typename U> void scaleY(U val);
	template <typename U> void scaleZ(U val);

	Mat4 operator*(const Mat4<T>& v) const;

	template<typename U>
	Vec4<U> operator*(const Vec4<U>& v) const;

	void scale(const Vec3<T>& s);
	void translate(const Vec3<T>& t);
	Mat4 inversed();
	T determinant();

	static Mat4 ConstructTRSMatrix(const Vec3<T>& t, const Quat<T>& r, const Vec3<T>& s);
	static Mat4 ConstructViewMatrix(const Vec3<T>& t, const Quat<T>& r, const Vec3<T>& s);

	static Mat4 ConstructPerspectiveMatrix(float fov, float aspect, float nearZ, float farZ);
	static Mat4 ConstructScaleMatrix(const Vec3<T>& s);
	static Mat4 ConstructRotationMatrix(const Quat<T>& r);
	static Mat4 ConstructTranslationMatrix(const Vec3<T>& t);
};


template <typename T>
inline Mat4<T>::Mat4()
	: m0(1),  m1(0),  m2(0),  m3(0),
	  m4(0),  m5(1),  m6(0),  m7(0),
	  m8(0),  m9(0),  m10(1), m11(0),
	  m12(0), m13(0), m14(0), m15(1)
{}

template <typename T>
inline Mat4<T>::Mat4(T val)
	: m0(val),  m1(val),  m2(val),  m3(val),
	  m4(val),  m5(val),  m6(val),  m7(val),
	  m8(val),  m9(val),  m10(val), m11(val),
	  m12(val), m13(val), m14(val), m15(val)
{}

template <typename T>
template <typename U>
void Mat4<T>::scaleX(U val)
{
	m0 *= val;
	m1 *= val;
	m2 *= val;
}

template <typename T>
template <typename U> 
void Mat4<T>::scaleY(U val)
{
	m4 *= val;
	m5 *= val;
	m6 *= val;
}

template <typename T>
template <typename U> 
void Mat4<T>::scaleZ(U val)
{
	m8  *= val;
	m9  *= val;
	m10 *= val;
}

template<typename T>
inline Mat4<T> Mat4<T>::operator*(const Mat4<T>& M) const
{
	Mat4 I;
	I.m0 = M.m0 * m0 + M.m1 * m4 + M.m2 * m8 + M.m3 * m12;
	I.m1 = M.m0 * m1 + M.m1 * m5 + M.m2 * m9 + M.m3 * m13;
	I.m2 = M.m0 * m2 + M.m1 * m6 + M.m2 * m10 + M.m3 * m14;
	I.m3 = M.m0 * m3 + M.m1 * m7 + M.m2 * m11 + M.m3 * m15;
	I.m4 = M.m4 * m0 + M.m5 * m4 + M.m6 * m8 + M.m7 * m12;
	I.m5 = M.m4 * m1 + M.m5 * m5 + M.m6 * m9 + M.m7 * m13;
	I.m6 = M.m4 * m2 + M.m5 * m6 + M.m6 * m10 + M.m7 * m14;
	I.m7 = M.m4 * m3 + M.m5 * m7 + M.m6 * m11 + M.m7 * m15;
	I.m8 = M.m10 * m8 + M.m11 * m12 + M.m8 * m0 + M.m9 * m4;
	I.m9 = M.m10 * m9 + M.m11 * m13 + M.m8 * m1 + M.m9 * m5;
	I.m10 = M.m10 * m10 + M.m11 * m14 + M.m8 * m2 + M.m9 * m6;
	I.m11 = M.m10 * m11 + M.m11 * m15 + M.m8 * m3 + M.m9 * m7;
	I.m12 = M.m12 * m0 + M.m13 * m4 + M.m14 * m8 + M.m15 * m12;
	I.m13 = M.m12 * m1 + M.m13 * m5 + M.m14 * m9 + M.m15 * m13;
	I.m14 = M.m12 * m2 + M.m13 * m6 + M.m14 * m10 + M.m15 * m14;
	I.m15 = M.m12 * m3 + M.m13 * m7 + M.m14 * m11 + M.m15 * m15;

	return I;
}

template<typename T>
template<typename U>
inline Vec4<U> Mat4<T>::operator*(const Vec4<U>& v) const
{
	Vec4<U> w;
	w.x = static_cast<U>(m0*v.x + m4*v.y + m8 *v.z + m12*v.w);
	w.y = static_cast<U>(m1*v.x + m5*v.y + m9 *v.z + m13*v.w);
	w.z = static_cast<U>(m2*v.x + m6*v.y + m10*v.z + m14*v.w);
	w.w = static_cast<U>(m3*v.x + m7*v.y + m11*v.z + m15*v.w);

	return w;
}

template <typename T>
void Mat4<T>::scale(const Vec3<T>& s)
{
	m0 *= s.x; m4 *= s.y; m8  *= s.z;
	m1 *= s.x; m5 *= s.y; m9  *= s.z;
	m2 *= s.x; m6 *= s.y; m10 *= s.z;
}

template <typename T>
void Mat4<T>::translate(const Vec3<T>& t)
{
	m12 = += t.x;
	m13 = += t.y;
	m14 = += t.z;
}

template<typename T>
inline Mat4<T> Mat4<T>::inversed()
{
	Mat4<T> I;
	T det = determinant();

	I.m0 = ((m14 * m7 - m15 * m6) * m9 + (m13 * m6 - m14 * m5) * m11 + (-m13 * m7 + m15 * m5) * m10) / det;
	I.m1 = (m9 * (-m14 * m3 + m15 * m2) + (m1 * m14 - m13 * m2) * m11 - (m1 * m15 - m13 * m3) * m10) / det;
	I.m2 = ((-m1 * m14 + m13 * m2) * m7 + m14 * m3 * m5 - m13 * m3 * m6 + m15 * (m1 * m6 - m2 * m5)) / det;
	I.m3 = ((-m2 * m7 + m3 * m6) * m9 + (-m1 * m6 + m2 * m5) * m11 + (m1 * m7 - m3 * m5) * m10) / det;
	I.m4 = (m8 * (-m14 * m7 + m15 * m6) + (-m12 * m6 + m14 * m4) * m11 - (-m12 * m7 + m15 * m4) * m10) / det;
	I.m5 = ((m14 * m3 - m15 * m2) * m8 + (-m0 * m14 + m12 * m2) * m11 + (m0 * m15 - m12 * m3) * m10) / det;
	I.m6 = ((m0 * m14 - m12 * m2) * m7 - m14 * m3 * m4 + (-m0 * m6 + m2 * m4) * m15 + m12 * m3 * m6) / det;
	I.m7 = ((m2 * m7 - m3 * m6) * m8 + m11 * (m0 * m6 - m2 * m4) - (m0 * m7 - m3 * m4) * m10) / det;
	I.m8 = ((-m12 * m7 + m15 * m4) * m9 + m11 * m12 * m5 + (-m11 * m4 + m7 * m8) * m13 - m15 * m5 * m8) / det;
	I.m9 = ((-m0 * m15 + m12 * m3) * m9 - m1 * m11 * m12 + (m0 * m11 - m3 * m8) * m13 + m1 * m15 * m8) / det;
	I.m10 = ((-m0 * m13 + m1 * m12) * m7 + m13 * m3 * m4 - m12 * m3 * m5 + m15 * (m0 * m5 - m1 * m4)) / det;
	I.m11 = ((m11 * m4 - m7 * m8) * m1 + m3 * m5 * m8 + (m0 * m7 - m3 * m4) * m9 - m0 * m11 * m5) / det;
	I.m12 = ((m12 * m6 - m14 * m4) * m9 - m10 * m12 * m5 + (m10 * m4 - m6 * m8) * m13 + m14 * m5 * m8) / det;
	I.m13 = ((m0 * m14 - m12 * m2) * m9 + m1 * m10 * m12 + (-m0 * m10 + m2 * m8) * m13 - m1 * m14 * m8) / det;
	I.m14 = ((m0 * m13 - m1 * m12) * m6 - m13 * m2 * m4 + (-m0 * m5 + m1 * m4) * m14 + m12 * m2 * m5) / det;
	I.m15 = ((-m10 * m4 + m6 * m8) * m1 - m2 * m5 * m8 + (-m0 * m6 + m2 * m4) * m9 + m0 * m10 * m5) / det;

	return I;
}

template<typename T>
inline T Mat4<T>::determinant()
{
	T det = ((m1 * m7 - m3 * m5) * m12
		+ (-m0 * m7 + m3 * m4) * m13
		+ m15 * (m0 * m5 - m1 * m4)) * m10
		+ ((-m1 * m6 + m2 * m5) * m12 + (m0 * m6 - m2 * m4) * m13 - m14 * (m0 * m5 - m1 * m4)) * m11 - m9 * (m2 * m7 - m3 * m6) 
		* m12 + m8 * (m2 * m7 - m3 * m6) * m13 + (-m7 * m8 * m1 + (-m4 * m9 + m5 * m8) * m3 + m0 * m9 * m7) * m14 - m15 
		* (-m6 * m8 * m1 + (-m4 * m9 + m5 * m8) * m2 + m0 * m9 * m6);
	return det;
}

template <typename T>
Mat4<T> Mat4<T>::ConstructTRSMatrix(const Vec3<T>& t, const Quat<T>& r, const Vec3<T>& s)
{
	Mat4<T> TRS;

	T xx2 = 2 * r.x * r.x;
	T yy2 = 2 * r.y * r.y;
	T zz2 = 2 * r.z * r.z;
	T ww2 = 2 * r.w * r.w;

	T xy2 = 2 * r.x * r.y;
	T xz2 = 2 * r.x * r.z;
	T xw2 = 2 * r.x * r.w;

	T yz2 = 2 * r.y * r.z;
	T yw2 = 2 * r.y * r.w;

	T zw2 = 2 * r.z * r.w;

	TRS.m0 = (1 - yy2 - zz2) * s.x;
	TRS.m1 = (xy2 + zw2) * s.x;
	TRS.m2 = (xz2 - yw2) * s.x;

	TRS.m4 = (xy2 - zw2) * s.y;
	TRS.m5 = (1 - xx2 - zz2) * s.y;
	TRS.m6 = (yz2 + xw2) * s.y;

	TRS.m8 = (xz2 + yw2) * s.z;
	TRS.m9 = (yz2 - xw2) * s.z;
	TRS.m10 = (1 - xx2 - yy2) * s.z;

	TRS.m12 = t.x;
	TRS.m13 = t.y;
	TRS.m14 = t.z;
	return TRS;
}

template <typename T>
Mat4<T> Mat4<T>::ConstructViewMatrix(const Vec3<T>& t, const Quat<T>& r, const Vec3<T>& s)
{
	Mat4<T> view;

	auto ri = r.inversed();

	T xx2 = 2 * ri.x * ri.x;
	T yy2 = 2 * ri.y * ri.y;
	T zz2 = 2 * ri.z * ri.z;
	T ww2 = 2 * ri.w * ri.w;

	T xy2 = 2 * ri.x * ri.y;
	T xz2 = 2 * ri.x * ri.z;
	T xw2 = 2 * ri.x * ri.w;

	T yz2 = 2 * ri.y * ri.z;
	T yw2 = 2 * ri.y * ri.w;

	T zw2 = 2 * ri.z * ri.w;

	T rim00 = 1 - yy2 - zz2;
	T rim01 = xy2 + zw2;
	T rim02 = xz2 - yw2;

	T rim10 = xy2 - zw2;
	T rim11 = 1 - xx2 - zz2;
	T rim12 = yz2 + xw2;

	T rim20 =  xz2 + yw2;
	T rim21 =  yz2 - xw2;
	T rim22 = 1 - xx2 - yy2;

	T siX = 1 / s.x;
	T siY = 1 / s.y;
	T siZ = 1 / s.z;

    view.m0 = rim00 * siX;
	view.m1 = rim10 * siY;
	view.m2 = rim20 * siZ;

	view.m4 = rim01 * siX;
	view.m5 = rim11 * siY;
	view.m6 = rim21 * siZ;

	view.m8  = rim02 * siX;
	view.m9  = rim12 * siY;
	view.m10 = rim22 * siZ;

	view.m12 = -(rim00*t.x + rim01*t.y + rim02*t.z) * siX;
	view.m13 = -(rim10*t.x + rim11*t.y + rim12*t.z) * siY;
	view.m14 = -(rim20*t.x + rim21*t.y + rim22*t.z) * siZ;

	return view;
}

template<typename T>
inline Mat4<T> Mat4<T>::ConstructPerspectiveMatrix(float fov, float aspect, float nearZ, float farZ)
{
	Mat4<T> perspective;
	perspective.m0 = 1 / (aspect * std::tan(fov / 2));
	perspective.m5 = 1 / std::tan(fov / 2);
	perspective.m10 = -(farZ + nearZ) / (farZ - nearZ);
	perspective.m11 = -1;
	perspective.m14 = -2 * farZ * nearZ / (farZ - nearZ);
	perspective.m15 = 0;
	return perspective;
}

template<typename T>
inline Mat4<T> Mat4<T>::ConstructScaleMatrix(const Vec3<T>& s)
{
	Mat4<T> M;
	M.m0  = s.x;
	M.m5  = s.y;
	M.m10 = s.z;

	return M;
}

template<typename T>
inline Mat4<T> Mat4<T>::ConstructRotationMatrix(const Quat<T>& r)
{
	Mat4<T> M;

	T xx2 = 2 * r.x * r.x;
	T yy2 = 2 * r.y * r.y;
	T zz2 = 2 * r.z * r.z;
	T ww2 = 2 * r.w * r.w;

	T xy2 = 2 * r.x * r.y;
	T xz2 = 2 * r.x * r.z;
	T xw2 = 2 * r.x * r.w;

	T yz2 = 2 * r.y * r.z;
	T yw2 = 2 * r.y * r.w;

	T zw2 = 2 * r.z * r.w;

	M.m0 = 1 - yy2 - zz2;
	M.m1 = xy2 + zw2;
	M.m2 = xz2 - yw2;
	M
	M.m4 = xy2 - zw2;
	M.m5 = 1 - xx2 - zz2;
	M.m6 = yz2 + xw2;
	M
	M.m8 = xz2 + yw2;
	M.m9 = yz2 - xw2;
	M.m10 = 1 - xx2 - yy2;

	return M;
}

template<typename T>
inline Mat4<T> Mat4<T>::ConstructTranslationMatrix(const Vec3<T>& t)
{
	Mat4<T> M;
	M.m12 = t.x;
	M.m13 = t.y;
	M.m14 = t.z;

	return M;
}


using Mat4F = Mat4<float>;
using Mat4D = Mat4<double>;