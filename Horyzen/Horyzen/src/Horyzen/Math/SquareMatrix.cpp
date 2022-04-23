#include "pchheader.h"

#include "SquareMatrix.h"
#include "Vector.h"
#include <memory>
#include <limits>
#include <cstring>


// DEBUG
#include <iostream>
#include <iomanip>
// ----------------

SquareMatrix::SquareMatrix(size_t dim)
	: m_dim(dim), m_values(new double[m_dim * m_dim])
{

}

SquareMatrix::SquareMatrix(size_t dim, double initVal)
	: m_dim(dim), m_values(new double[m_dim * m_dim])
{
	for (size_t i = 0; i < m_dim * m_dim; ++i) {
		m_values[i] = initVal;
	}
}

SquareMatrix::SquareMatrix(size_t dim, const std::initializer_list<double>& init)
	: m_dim(dim)
{
	m_values = new double[m_dim * m_dim];
	size_t i = 0;
	for (auto val : init) {
		m_values[i] = val;
		++i;
	}
}

SquareMatrix::SquareMatrix(const SquareMatrix& M)
	: m_dim(M.m_dim)
{
	m_values = new double[m_dim * m_dim];
	memcpy(m_values, M.m_values, m_dim * m_dim * sizeof(double));
}

SquareMatrix::SquareMatrix(size_t dim, std::string filename)
	: m_dim(dim), m_values(new double[m_dim * m_dim])
{
	std::ifstream in;
	in.open(filename);
	for (size_t i = 0; i < m_dim * m_dim; ++i) {
		in >> m_values[i];
	}
	in.close();
}

SquareMatrix::SquareMatrix(std::string filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	in.read(reinterpret_cast<char*>(&m_dim), sizeof(size_t));
	in.read(reinterpret_cast<char*>(&m_dim), sizeof(size_t));

	m_values = new double[m_dim * m_dim];
	
	in.read(reinterpret_cast<char*>(m_values), sizeof(double)* m_dim* m_dim);
	in.close();
}

SquareMatrix::SquareMatrix(SquareMatrix&& M) noexcept
	: m_dim(M.m_dim), m_values(M.m_values)
{
	M.m_dim = 0;
	M.m_values = nullptr;
}

SquareMatrix::~SquareMatrix()
{
	delete[] m_values;
}

SquareMatrix& SquareMatrix::operator=(const SquareMatrix& M)
{
	m_dim = M.m_dim;
	m_values = new double[m_dim * m_dim];
	memcpy(m_values, M.m_values, m_dim * m_dim * sizeof(double));

	return *this;
}

SquareMatrix& SquareMatrix::operator=(const std::initializer_list<double>& init)
{
	// TODO if invalid number of values

	size_t i = 0;
	for (auto val : init) {
		m_values[i] = val;
		++i;
	}
	return *this;
}

SquareMatrix& SquareMatrix::operator=(SquareMatrix&& M) noexcept
{
	m_dim = M.m_dim;
	m_values = M.m_values;
	M.m_dim = 0;
	M.m_values = nullptr;

	return *this;
}

double& SquareMatrix::operator()(size_t i, size_t j) const
{
	return m_values[i * m_dim + j];
}

size_t SquareMatrix::getDimension() const
{
	return m_dim;
}

void SquareMatrix::setToIdentity()
{
	memset(m_values, 0, m_dim * m_dim * sizeof(double));
	for (size_t i = 0; i < m_dim; ++i)
		(*this)(i, i) = 1.0;
}

void SquareMatrix::setToZero()
{
	memset(m_values, 0, m_dim * m_dim * sizeof(double));
}

SquareMatrix SquareMatrix::transposed() const
{
	SquareMatrix T(m_dim);
	const SquareMatrix& M = *this;

	for (size_t i = 0; i < m_dim; ++i) {
		for (size_t j = 0; j < m_dim; ++j) {
			T(i, j) = M(j, i);
		}
	}


	return T;
}

void SquareMatrix::transposeThis()
{
	for (size_t i = 0; i < m_dim; ++i) {
		for (size_t j = 0; j < i; ++j) {
			double tmp = m_values[i * m_dim + j];
			m_values[i * m_dim + j] = m_values[j * m_dim + i];
			m_values[j * m_dim + i] = tmp;
		}
	}
}

SquareMatrix SquareMatrix::inversed() const
{
	const SquareMatrix& M = *this;
	SquareMatrix MI(m_dim);

	if (m_dim == 1) {
		MI(0, 0) = 1.0 / M(0, 0);
	} else if (m_dim == 2) {
		double det = M.det();
		MI(0, 0) = M(1, 1) / det;
		MI(1, 1) = M(0, 0) / det;
		MI(0, 1) = -M(0, 1) / det;
		MI(1, 0) = -M(1, 0) / det;
	} else if (m_dim == 3) {
		double det = M.det();
		MI(0, 0) =  (M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2)) / det;
		MI(0, 1) = -(M(1, 0) * M(2, 2) - M(2, 0) * M(1, 2)) / det;
		MI(0, 2) =  (M(1, 0) * M(2, 1) - M(2, 0) * M(1, 1)) / det;
		MI(1, 0) = -(M(0, 1) * M(2, 2) - M(2, 1) * M(0, 2)) / det;
		MI(1, 1) =  (M(0, 0) * M(2, 2) - M(2, 0) * M(0, 2)) / det;
		MI(1, 2) = -(M(0, 0) * M(2, 1) - M(2, 0) * M(0, 1)) / det;
		MI(2, 0) =  (M(0, 1) * M(1, 2) - M(1, 1) * M(0, 2)) / det;
		MI(2, 1) = -(M(0, 0) * M(1, 2) - M(1, 0) * M(0, 2)) / det;
		MI(2, 2) =  (M(0, 0) * M(1, 1) - M(1, 0) * M(0, 1)) / det;

		MI.transposeThis();
	} else if (m_dim == 4) {
		double det = M.det();
		MI(0, 0) = M(1, 1) * (M(2, 2) * M(3, 3) - M(3, 2) * M(2, 3)) -
			       M(1, 2) * (M(2, 1) * M(3, 3) - M(3, 1) * M(2, 3)) +
			       M(1, 3) * (M(2, 1) * M(3, 2) - M(3, 1) * M(2, 2));

		MI(0, 1) = M(1, 0) * (M(2, 2) * M(3, 3) - M(3, 2) * M(2, 3)) -
			       M(1, 2) * (M(2, 0) * M(3, 3) - M(3, 0) * M(2, 3)) +
			       M(1, 3) * (M(2, 0) * M(3, 2) - M(3, 0) * M(2, 2));

		MI(0, 2) = M(1, 0) * (M(2, 1) * M(3, 3) - M(3, 1) * M(2, 3)) -
			       M(1, 1) * (M(2, 0) * M(3, 3) - M(3, 0) * M(2, 3)) +
			       M(1, 3) * (M(2, 0) * M(3, 1) - M(3, 0) * M(2, 1));

		MI(0, 3) = M(1, 0) * (M(2, 1) * M(3, 2) - M(3, 1) * M(2, 2)) -
			       M(1, 1) * (M(2, 0) * M(3, 2) - M(3, 0) * M(2, 2)) +
			       M(1, 2) * (M(2, 0) * M(3, 1) - M(3, 0) * M(2, 1));


		MI(1, 0) = M(0, 1) * (M(2, 2) * M(3, 3) - M(3, 2) * M(2, 3)) -
			       M(0, 2) * (M(2, 1) * M(3, 3) - M(3, 1) * M(2, 3)) +
			       M(0, 3) * (M(2, 1) * M(3, 2) - M(3, 1) * M(2, 2));

		MI(1, 1) = M(0, 0) * (M(2, 2) * M(3, 3) - M(3, 2) * M(2, 3)) -
			       M(0, 2) * (M(2, 0) * M(3, 3) - M(3, 0) * M(2, 3)) +
			       M(0, 3) * (M(2, 0) * M(3, 2) - M(3, 0) * M(2, 2));

		MI(1, 2) = M(0, 0) * (M(2, 1) * M(3, 3) - M(3, 1) * M(2, 3)) -
			       M(0, 1) * (M(2, 0) * M(3, 3) - M(3, 0) * M(2, 3)) +
			       M(0, 3) * (M(2, 0) * M(3, 1) - M(3, 0) * M(2, 1));

		MI(1, 3) = M(0, 0) * (M(2, 1) * M(3, 2) - M(3, 1) * M(2, 2)) -
			       M(0, 1) * (M(2, 0) * M(3, 2) - M(3, 0) * M(2, 2)) +
			       M(0, 2) * (M(2, 0) * M(3, 1) - M(3, 0) * M(2, 1));


		MI(2, 0) = M(0, 1) * (M(1, 2) * M(3, 3) - M(3, 2) * M(1, 3)) -
			       M(0, 2) * (M(1, 1) * M(3, 3) - M(3, 1) * M(1, 3)) +
			       M(0, 3) * (M(1, 1) * M(3, 2) - M(3, 1) * M(1, 2));

		MI(2, 1) = M(0, 0) * (M(1, 2) * M(3, 3) - M(3, 2) * M(1, 3)) -
			       M(0, 2) * (M(1, 0) * M(3, 3) - M(3, 0) * M(1, 3)) +
			       M(0, 3) * (M(1, 0) * M(3, 2) - M(3, 0) * M(1, 2));

		MI(2, 2) = M(0, 0) * (M(1, 1) * M(3, 3) - M(3, 1) * M(1, 3)) -
			       M(0, 1) * (M(1, 0) * M(3, 3) - M(3, 0) * M(1, 3)) +
			       M(0, 3) * (M(1, 0) * M(3, 1) - M(3, 0) * M(1, 1));

		MI(2, 3) = M(0, 0) * (M(1, 1) * M(3, 2) - M(3, 1) * M(1, 2)) -
			       M(0, 1) * (M(1, 0) * M(3, 2) - M(3, 0) * M(1, 2)) +
			       M(0, 2) * (M(1, 0) * M(3, 1) - M(3, 0) * M(1, 1));




		MI(3, 0) = M(0, 1) * (M(1, 2) * M(2, 3) - M(2, 2) * M(1, 3)) -
			       M(0, 2) * (M(1, 1) * M(2, 3) - M(2, 1) * M(1, 3)) +
			       M(0, 3) * (M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2));

		MI(3, 1) = M(0, 0) * (M(1, 2) * M(2, 3) - M(2, 2) * M(1, 3)) -
			       M(0, 2) * (M(1, 0) * M(2, 3) - M(2, 0) * M(1, 3)) +
			       M(0, 3) * (M(1, 0) * M(2, 2) - M(2, 0) * M(1, 2));

		MI(3, 2) = M(0, 0) * (M(1, 1) * M(2, 3) - M(2, 1) * M(1, 3)) -
			       M(0, 1) * (M(1, 0) * M(2, 3) - M(2, 0) * M(1, 3)) +
			       M(0, 3) * (M(1, 0) * M(2, 1) - M(2, 0) * M(1, 1));

		MI(3, 3) = M(0, 0) * (M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2)) -
			       M(0, 1) * (M(1, 0) * M(2, 2) - M(2, 0) * M(1, 2)) +
			       M(0, 2) * (M(1, 0) * M(2, 1) - M(2, 0) * M(1, 1));

		for (size_t i = 0; i < m_dim; ++i) {
			for (size_t j = 0; j < m_dim; ++j) {
				if ((i + j) % 2 != 0) MI(i, j) = -MI(i, j);
				MI(i, j) /= det;
			}
		}
		MI.transposeThis();
	}

	return MI;
}

void SquareMatrix::setRowToZero(size_t row)
{
	for (size_t i = 0; i < m_dim; ++i) {
		m_values[row * m_dim + i] = 0;
	}
}

bool SquareMatrix::isSymmetric() const
{
	for (size_t i = 1; i < m_dim; ++i) {
		for (size_t j = 0; j < i; ++j) {
			if (! ((*this)(i, j) == (*this)(j, i)) ) {
				return false;
			}
		}
	}
	return true;
}

bool SquareMatrix::hasSymmetricPortrait() const
{
	for (size_t i = 1; i < m_dim; ++i) {
		for (size_t j = 0; j < i; ++j) {
			double i_e = (*this)(i, j);
			double j_e = (*this)(j, i);
			if ( (i_e == 0.0) ^  (j_e == 0)) {
				return false;
			}
		}
	}
	return true;
}

double SquareMatrix::norm()
{
	double result = 0;
	for (size_t i = 0; i < m_dim; ++i) {
		for (size_t j = 0; j < m_dim; ++j) {
			result += abs(m_values[i * m_dim + j]);
		}
	}
	return result;
}

double SquareMatrix::det() const
{
	const SquareMatrix& M = *this;
	if (m_dim == 1) return M(0, 0);
	if (m_dim == 2) {
		return M(0, 0) * M(1, 1) - M(1, 0) * M(0, 1);
	}
	if (m_dim == 3) {
		return M(0, 0) * ( M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2) ) -
			   M(0, 1) * ( M(1, 0) * M(2, 2) - M(2, 0) * M(1, 2) ) +
			   M(0, 2) * ( M(1, 0) * M(2, 1) - M(2, 0) * M(1, 1) );
	}
	if (m_dim == 4) {
		double first = M(1, 1) * (M(2, 2) * M(3, 3) - M(3, 2) * M(2, 3)) -
			           M(1, 2) * (M(2, 1) * M(3, 3) - M(3, 1) * M(2, 3)) +
			           M(1, 3) * (M(2, 1) * M(3, 2) - M(3, 1) * M(2, 2));

		double second = M(1, 0) * (M(2, 2) * M(3, 3) - M(3, 2) * M(2, 3)) -
			            M(1, 2) * (M(2, 0) * M(3, 3) - M(3, 0) * M(2, 3)) +
			            M(1, 3) * (M(2, 0) * M(3, 2) - M(3, 0) * M(2, 2));

		double third = M(1, 0) * (M(2, 1) * M(3, 3) - M(3, 1) * M(2, 3)) -
			           M(1, 1) * (M(2, 0) * M(3, 3) - M(3, 0) * M(2, 3)) +
			           M(1, 3) * (M(2, 0) * M(3, 1) - M(3, 0) * M(2, 1));

		double fourth = M(1, 0) * (M(2, 1) * M(3, 2) - M(3, 1) * M(2, 2)) -
			            M(1, 1) * (M(2, 0) * M(3, 2) - M(3, 0) * M(2, 2)) +
			            M(1, 2) * (M(2, 0) * M(3, 1) - M(3, 0) * M(2, 1));

		return  M(0, 0) * first - M(0, 1) * second + M(0, 2) * third - M(0, 3) * fourth;
	}
}

double SquareMatrix::trace()
{
	SquareMatrix& M = *this;
	double res = 0;
	for (size_t i = 0; i < m_dim; ++i) {
		res += M(i, i);
	}
	return res;
}

double SquareMatrix::maxAbsValue()
{
	double result = 0;
	for (size_t i = 0; i < m_dim; ++i) {
		for (size_t j = 0; j < m_dim; ++j) {
			if (abs(m_values[i * m_dim + j]) > result)
			result = abs(m_values[i * m_dim + j]);
		}
	}
	return result;
}

double SquareMatrix::symmetryError()
{
	double result = 0;
	for (size_t i = 0; i < m_dim; ++i) {
		for (size_t j = 0; j < i; ++j) {
			if (abs((*this)(i, j) - (*this)(j, i)) > result) {
				result = abs((*this)(i, j) - (*this)(j, i));
			}
		}
	}
	return result;
}

void SquareMatrix::Multiply(const SquareMatrix& p_leftOperand,
	                        const SquareMatrix& p_rightOperand,
	                        SquareMatrix&       o_destination)
{
	for (size_t i = 0; i < p_leftOperand.m_dim; ++i) {
		for (size_t j = 0; j < p_rightOperand.m_dim; ++j) {
			double sum = 0;
			for (size_t k = 0; k < p_leftOperand.m_dim; ++k) {
				sum += p_leftOperand(i, k) * p_rightOperand(k, j);
			}
			o_destination(i, j) = sum;
		}
	}
}

void SquareMatrix::Inverse(const SquareMatrix& p_matrixToInverse,
	                       SquareMatrix&       o_destination)
{
	const SquareMatrix& M = p_matrixToInverse;
	SquareMatrix& MI = o_destination;
	const size_t dim = p_matrixToInverse.m_dim;

	if (dim == 1) {
		MI(0, 0) = 1.0 / M(0, 0);
	}
	else if (dim == 2) {
		double det = M.det();
		MI(0, 0) = M(1, 1) / det;
		MI(1, 1) = M(0, 0) / det;
		MI(0, 1) = -M(0, 1) / det;
		MI(1, 0) = -M(1, 0) / det;
	}
	else if (dim == 3) {
		double det = M.det();
		MI(0, 0) =  (M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2)) / det;
		MI(0, 1) = -(M(1, 0) * M(2, 2) - M(2, 0) * M(1, 2)) / det;
		MI(0, 2) =  (M(1, 0) * M(2, 1) - M(2, 0) * M(1, 1)) / det;
		MI(1, 0) = -(M(0, 1) * M(2, 2) - M(2, 1) * M(0, 2)) / det;
		MI(1, 1) =  (M(0, 0) * M(2, 2) - M(2, 0) * M(0, 2)) / det;
		MI(1, 2) = -(M(0, 0) * M(2, 1) - M(2, 0) * M(0, 1)) / det;
		MI(2, 0) =  (M(0, 1) * M(1, 2) - M(1, 1) * M(0, 2)) / det;
		MI(2, 1) = -(M(0, 0) * M(1, 2) - M(1, 0) * M(0, 2)) / det;
		MI(2, 2) =  (M(0, 0) * M(1, 1) - M(1, 0) * M(0, 1)) / det;

		MI.transposeThis();
	}
	else if (dim == 4) {
		double det = M.det();
		MI(0, 0) = M(1, 1) * (M(2, 2) * M(3, 3) - M(3, 2) * M(2, 3)) -
			       M(1, 2) * (M(2, 1) * M(3, 3) - M(3, 1) * M(2, 3)) +
			       M(1, 3) * (M(2, 1) * M(3, 2) - M(3, 1) * M(2, 2));

		MI(0, 1) = M(1, 0) * (M(2, 2) * M(3, 3) - M(3, 2) * M(2, 3)) -
			       M(1, 2) * (M(2, 0) * M(3, 3) - M(3, 0) * M(2, 3)) +
			       M(1, 3) * (M(2, 0) * M(3, 2) - M(3, 0) * M(2, 2));

		MI(0, 2) = M(1, 0) * (M(2, 1) * M(3, 3) - M(3, 1) * M(2, 3)) -
			       M(1, 1) * (M(2, 0) * M(3, 3) - M(3, 0) * M(2, 3)) +
			       M(1, 3) * (M(2, 0) * M(3, 1) - M(3, 0) * M(2, 1));

		MI(0, 3) = M(1, 0) * (M(2, 1) * M(3, 2) - M(3, 1) * M(2, 2)) -
			       M(1, 1) * (M(2, 0) * M(3, 2) - M(3, 0) * M(2, 2)) +
			       M(1, 2) * (M(2, 0) * M(3, 1) - M(3, 0) * M(2, 1));


		MI(1, 0) = M(0, 1) * (M(2, 2) * M(3, 3) - M(3, 2) * M(2, 3)) -
			       M(0, 2) * (M(2, 1) * M(3, 3) - M(3, 1) * M(2, 3)) +
			       M(0, 3) * (M(2, 1) * M(3, 2) - M(3, 1) * M(2, 2));

		MI(1, 1) = M(0, 0) * (M(2, 2) * M(3, 3) - M(3, 2) * M(2, 3)) -
			       M(0, 2) * (M(2, 0) * M(3, 3) - M(3, 0) * M(2, 3)) +
			       M(0, 3) * (M(2, 0) * M(3, 2) - M(3, 0) * M(2, 2));

		MI(1, 2) = M(0, 0) * (M(2, 1) * M(3, 3) - M(3, 1) * M(2, 3)) -
			       M(0, 1) * (M(2, 0) * M(3, 3) - M(3, 0) * M(2, 3)) +
			       M(0, 3) * (M(2, 0) * M(3, 1) - M(3, 0) * M(2, 1));

		MI(1, 3) = M(0, 0) * (M(2, 1) * M(3, 2) - M(3, 1) * M(2, 2)) -
			       M(0, 1) * (M(2, 0) * M(3, 2) - M(3, 0) * M(2, 2)) +
			       M(0, 2) * (M(2, 0) * M(3, 1) - M(3, 0) * M(2, 1));


		MI(2, 0) = M(0, 1) * (M(1, 2) * M(3, 3) - M(3, 2) * M(1, 3)) -
			       M(0, 2) * (M(1, 1) * M(3, 3) - M(3, 1) * M(1, 3)) +
			       M(0, 3) * (M(1, 1) * M(3, 2) - M(3, 1) * M(1, 2));

		MI(2, 1) = M(0, 0) * (M(1, 2) * M(3, 3) - M(3, 2) * M(1, 3)) -
			       M(0, 2) * (M(1, 0) * M(3, 3) - M(3, 0) * M(1, 3)) +
			       M(0, 3) * (M(1, 0) * M(3, 2) - M(3, 0) * M(1, 2));

		MI(2, 2) = M(0, 0) * (M(1, 1) * M(3, 3) - M(3, 1) * M(1, 3)) -
			       M(0, 1) * (M(1, 0) * M(3, 3) - M(3, 0) * M(1, 3)) +
			       M(0, 3) * (M(1, 0) * M(3, 1) - M(3, 0) * M(1, 1));

		MI(2, 3) = M(0, 0) * (M(1, 1) * M(3, 2) - M(3, 1) * M(1, 2)) -
			       M(0, 1) * (M(1, 0) * M(3, 2) - M(3, 0) * M(1, 2)) +
			       M(0, 2) * (M(1, 0) * M(3, 1) - M(3, 0) * M(1, 1));


		MI(3, 0) = M(0, 1) * (M(1, 2) * M(2, 3) - M(2, 2) * M(1, 3)) -
			       M(0, 2) * (M(1, 1) * M(2, 3) - M(2, 1) * M(1, 3)) +
			       M(0, 3) * (M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2));

		MI(3, 1) = M(0, 0) * (M(1, 2) * M(2, 3) - M(2, 2) * M(1, 3)) -
			       M(0, 2) * (M(1, 0) * M(2, 3) - M(2, 0) * M(1, 3)) +
			       M(0, 3) * (M(1, 0) * M(2, 2) - M(2, 0) * M(1, 2));

		MI(3, 2) = M(0, 0) * (M(1, 1) * M(2, 3) - M(2, 1) * M(1, 3)) -
			       M(0, 1) * (M(1, 0) * M(2, 3) - M(2, 0) * M(1, 3)) +
			       M(0, 3) * (M(1, 0) * M(2, 1) - M(2, 0) * M(1, 1));

		MI(3, 3) = M(0, 0) * (M(1, 1) * M(2, 2) - M(2, 1) * M(1, 2)) -
			       M(0, 1) * (M(1, 0) * M(2, 2) - M(2, 0) * M(1, 2)) +
			       M(0, 2) * (M(1, 0) * M(2, 1) - M(2, 0) * M(1, 1));

		for (size_t i = 0; i < dim; ++i) {
			for (size_t j = 0; j < dim; ++j) {
				if ((i + j) % 2 != 0) MI(i, j) = -MI(i, j);
				MI(i, j) /= det;
			}
		}
		MI.transposeThis();
	}
}

void SquareMatrix::print() const
{
	for (size_t i = 0; i < m_dim; ++i) {
		for (size_t j = 0; j < m_dim; ++j) {
			std::cout << std::setw(10) << (*this)(i, j);
		}
		std::cout << std::endl;
	}
}

void SquareMatrix::printIntoFile(std::string filename, size_t width)
{
	std::ofstream out;
	out.open(filename);
	out.precision(std::numeric_limits< double >::digits);
	for (size_t i = 0; i < m_dim; ++i) {
		for (size_t j = 0; j < m_dim; ++j) {
			out << std::setw(width) << m_values[i * m_dim + j];
		}
		out << std::endl;
	}
	out.close();
}

void SquareMatrix::writeIntoBinaryFile(std::string filename)
{
	std::ofstream out(filename, std::ios::out | std::ios::binary);
	out.write(reinterpret_cast<const char*>(&m_dim), sizeof(size_t));
	out.write(reinterpret_cast<const char*>(&m_dim), sizeof(size_t));
	out.write(reinterpret_cast<const char*>(m_values), sizeof(double) * m_dim * m_dim);
	out.close();
}

SquareMatrix SquareMatrix::sum(const SquareMatrix& A, const SquareMatrix& B)
{
	SquareMatrix result(A.m_dim);
	for (size_t ind = 0; ind < A.m_dim * A.m_dim; ++ind)
		result.m_values[ind] = A.m_values[ind] + B.m_values[ind];

	return result;
}

SquareMatrix SquareMatrix::multiply(const SquareMatrix& A, const SquareMatrix& B)
{
	SquareMatrix result(A.m_dim);
	SquareMatrix BT = B.transposed();

//	#pragma omp parallel for
	for (int i = 0; i < A.m_dim; ++i) {
		for (int j = 0; j < B.m_dim; ++j) {
			double sum = 0;
//			#pragma omp simd
			for (int k = 0; k < A.m_dim; ++k) {
				sum += A(i, k) * BT(j, k);
			}
			result(i, j) = sum;
		}
	}
	
	return result;
}

SquareMatrix SquareMatrix::KroneckerProduct(const SquareMatrix& A, const SquareMatrix& B)
{
	size_t Adim = A.getDimension();
	size_t Bdim = B.getDimension();

	SquareMatrix result(Adim * Bdim);
	for (size_t i = 0; i < Adim; ++i) {
		for (size_t j = 0; j < Adim; ++j) {
			size_t ist = i * Bdim;
			size_t jst = j * Bdim;
			for (size_t k = 0; k < Bdim; ++k) {
				for (size_t m = 0; m < Bdim; ++m) {
					result(ist + k, jst + m) = A(i, j) * B(k, m);
				}
			}
		}
	}

	return result;
}

SquareMatrix operator+(const SquareMatrix& A, const SquareMatrix& B)
{
	return SquareMatrix::sum(A, B);
}

SquareMatrix operator-(const SquareMatrix& A, const SquareMatrix& B)
{
	SquareMatrix result(A.m_dim);
	for (size_t i = 0; i < A.m_dim; ++i) {
		for (size_t j = 0; j < A.m_dim; ++j) {
			result.m_values[i * A.m_dim + j] = A.m_values[i * A.m_dim + j] - B.m_values[i * A.m_dim + j];
		}
	}
	return result;
}

SquareMatrix operator*(const SquareMatrix& A, const SquareMatrix& B)
{
	return SquareMatrix::multiply(A, B);
}

SquareMatrix operator*(double s, const SquareMatrix& B)
{
	SquareMatrix result(B);
	for (size_t i = 0; i < B.m_dim * B.m_dim; ++i) {
		result.m_values[i] *= s;
	}
	return result;
}

Vector operator*(const SquareMatrix& M, const Vector& v)
{
	Vector result(M.m_dim);
	for (size_t i = 0; i < M.m_dim; ++i) {
		result[i] = 0.0;
		for (size_t k = 0; k < M.m_dim; ++k) {
			result[i] += M(i, k) * v[k];
		}
	}
	return result;
}




