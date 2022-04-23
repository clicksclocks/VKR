#include "pchheader.h"

#include "Vector.h"
#include <memory>
#include <stdexcept>

// DEBUG
#include <iostream>
#include <iomanip>
// ---------

Vector::Vector(size_t dim)
	: m_dim(dim), m_values(new double[dim])
{
}

Vector::Vector(size_t dim, double initValue)
	: m_dim(dim), m_values(new double[dim])
{
	for (size_t i = 0; i < m_dim; ++i) {
		m_values[i] = initValue;
	}
}

Vector::Vector(const Vector& v)
	: m_dim(v.m_dim)
{
	m_values = new double[v.m_dim];
	memcpy(m_values, v.m_values, v.m_dim * sizeof(double));
}

Vector::Vector(Vector&& v) noexcept
	: m_dim(v.m_dim), m_values(v.m_values)
{
	v.m_dim = 0;
	v.m_values = nullptr;
}

Vector::Vector(const std::initializer_list<double>& list)
	: m_dim(list.size())
{
	m_values = new double[m_dim];
	size_t i = 0;
	for (auto val : list) {
		m_values[i] = val;
		++i;
	}
}

Vector::Vector(std::string filename)
{
	std::ifstream in(filename, std::ios::in | std::ios::binary);
	in.read(reinterpret_cast<char*>(&m_dim), sizeof(size_t));

	m_values = new double[m_dim];

	in.read(reinterpret_cast<char*>(m_values), sizeof(double)* m_dim);
	in.close();
}

Vector::~Vector()
{
	delete[] m_values;
}

Vector& Vector::operator=(const Vector& v)
{
	m_dim = v.m_dim;
	m_values = new double[v.m_dim];
	memcpy(m_values, v.m_values, v.m_dim * sizeof(double));
	return *this;
}

Vector& Vector::operator=(Vector&& v) noexcept
{
	if (m_values != nullptr)
		delete m_values;
	m_dim = v.m_dim;
	m_values = v.m_values;
	v.m_dim = 0;
	v.m_values = nullptr;

	return *this;
}

double& Vector::operator[](size_t index) const
{
	return m_values[index];
}

//Vector Vector::operator-(const Vector& v) const
//{
//	Vector result(v.m_dim);
//	for (size_t i = 0; i < v.m_dim; ++i) {
//		result.m_values[i] = -v.m_values[i];
//	}
//	return result;
//}

size_t Vector::getDimension() const
{
	return m_dim;
}

double Vector::euclideanNorm() const
{
	double result = 0.0;
	for (size_t i = 0; i < m_dim; ++i)
		result += m_values[i] * m_values[i];
	result = sqrt(result);
	return result;	
}

void Vector::print()
{
	std::cout << "[";
	for (size_t i = 0; i < m_dim - 1; ++i)
		std::cout << m_values[i] << ", ";
	std::cout << m_values[m_dim-1] << "]";
}

void Vector::printIntoFile(std::string filename, size_t width)
{
	std::ofstream out;
	out.open(filename);
	for (size_t i = 0; i < m_dim; ++i) {
		out << std::setw(width) << m_values[i] << std::endl;
	}
	out.close();
}

void Vector::writeIntoBinaryFile(std::string filename)
{
	std::ofstream out(filename, std::ios::out | std::ios::binary);
	out.write(reinterpret_cast<const char*>(&m_dim), sizeof(size_t));
	out.write(reinterpret_cast<const char*>(m_values), sizeof(double)* m_dim);
	out.close();
}

double dot(const Vector& u, const Vector& w)
{
	if (u.m_dim != w.m_dim) {
		throw std::out_of_range("Index out of range");
	}

	double result = 0;
	for (size_t i = 0; i < u.m_dim; ++i)
		result += u[i] * w[i];

	return result;
}

Vector scale(double scalar, const Vector& u)
{
	Vector result(u);
	for (size_t i = 0; i < u.m_dim; ++i) {
		result.m_values[i] = u.m_values[i] * scalar;
	}
	return result;
}

Vector operator+(const Vector& u, const Vector& w)
{
	Vector result(u.m_dim);
	for (int i = 0; i < u.m_dim; ++i) {
		result.m_values[i] = u.m_values[i] + w.m_values[i];
	}
	return result;
}

Vector operator-(const Vector& u, const Vector& w)
{
	Vector result(u.m_dim);
	for (size_t i = 0; i < u.m_dim; ++i) {
		result.m_values[i] = u.m_values[i] - w.m_values[i];
	}
	return result;
}




