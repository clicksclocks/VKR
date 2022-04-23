#pragma once
#include "Horyzen/Core.h"

#include <initializer_list>
#include <string>
#include <fstream>


class Vector;

class HORYZEN_OBJECT SquareMatrix
{
public:
	SquareMatrix() = delete;
	SquareMatrix(size_t dim);
	SquareMatrix(size_t dim, double initVal);
	SquareMatrix(size_t dim, const std::initializer_list<double>& init);
	SquareMatrix(const SquareMatrix& M);
	SquareMatrix(size_t dim, std::string filename);
	SquareMatrix(std::string filename);

	SquareMatrix(SquareMatrix&& M) noexcept;

	~SquareMatrix();

	SquareMatrix& operator=(const SquareMatrix& M);
	SquareMatrix& operator=(const std::initializer_list<double>& init);
	SquareMatrix& operator=(SquareMatrix&& M) noexcept;
	double& operator()(size_t i, size_t j) const;

	size_t getDimension() const;
	void setToIdentity();
	void setToZero();

	SquareMatrix transposed() const;
	void transposeThis();
	SquareMatrix inversed() const;

	void setRowToZero(size_t row);

	bool isSymmetric() const;
	bool hasSymmetricPortrait() const;

	double norm();
	double det() const;
	double trace();
	double maxAbsValue();
	double symmetryError();

	static SquareMatrix sum(const SquareMatrix& A, const SquareMatrix& B);
	static SquareMatrix multiply(const SquareMatrix& A, const SquareMatrix& B);
	static SquareMatrix KroneckerProduct(const SquareMatrix& A, const SquareMatrix& B);

	friend SquareMatrix operator+(const SquareMatrix& A, const SquareMatrix& B);
	friend SquareMatrix operator*(const SquareMatrix& A, const SquareMatrix& B);
	friend SquareMatrix operator-(const SquareMatrix& A, const SquareMatrix& B);
	friend Vector operator*(const SquareMatrix& M, const Vector& v);

	friend SquareMatrix operator*(double s, const SquareMatrix& B);

	static void Multiply(const SquareMatrix& p_leftOperand,
		                 const SquareMatrix& p_rightOperand,
		                 SquareMatrix&       o_destination);

	static void Inverse(const SquareMatrix& p_matrixToInverse,
		                SquareMatrix&       o_destination);

	// DEBUG
	void print() const;
	void printIntoFile(std::string filename, size_t width = 60);
	void writeIntoBinaryFile(std::string filename);


private:
	size_t m_dim;
	double* m_values;
};

SquareMatrix operator+(const SquareMatrix& A, const SquareMatrix& B);
SquareMatrix operator-(const SquareMatrix& A, const SquareMatrix& B);
SquareMatrix operator*(const SquareMatrix& A, const SquareMatrix& B);
SquareMatrix operator*(double s, const SquareMatrix& B);
Vector operator*(const SquareMatrix& M, const Vector& v);
