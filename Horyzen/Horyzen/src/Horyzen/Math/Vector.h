#pragma once
#include <initializer_list>
#include <cmath>
#include <string>
#include <fstream>

class Vector
{
public:
	Vector() = delete;
	Vector(size_t dim);
	Vector(size_t dim, double initValue);
	Vector(const Vector& v);
	Vector(Vector&& v) noexcept;
	Vector(const std::initializer_list<double>& list);
	Vector(std::string filename);
	
	~Vector();

	Vector& operator=(const Vector& v);
	Vector& operator=(Vector&& v) noexcept;

	double& operator[](size_t index) const;

//	Vector operator-(const Vector& v) const;
	
	size_t getDimension() const;
	double euclideanNorm() const;

	// DEBUG
	void print();
	void printIntoFile(std::string filename, size_t width = 13);
	void writeIntoBinaryFile(std::string filename);


	friend double dot(const Vector& u, const Vector& w);
	friend Vector scale(double scalar, const Vector& u);
	friend Vector operator+(const Vector& u, const Vector& w);
	friend Vector operator-(const Vector& u, const Vector& w);


private:
	size_t m_dim;
	double* m_values;
};

double dot(const Vector& u, const Vector& w);
Vector scale(double scalar, const Vector& u);

Vector operator+(const Vector& u, const Vector& w);
Vector operator-(const Vector& u, const Vector& w);

