#include "stdafx.h"
#include "vector_helper.h" 

/*********************** VECTOR2D ***********************/
Vector2D::Vector2D()
{
	this->X = 0;
	this->Y = 0;
}

Vector2D::Vector2D(double x, double y)
{
	this->X = x;
	this->Y = y;
}

double Vector2D::Length()
{
	return std::sqrt(std::pow(this->X, 2) + std::pow(this->Y, 2));
}

double Vector2D::ToAngle()
{
	return atan(this->X / this->Y) * 180 / M_PI;
}

Vector2D Vector2D::operator+(const Vector2D& a)
{
	Vector2D result;
	result.X = this->X + a.X;
	result.Y = this->Y + a.Y;
	return result;
}

Vector2D Vector2D::operator-(const Vector2D& a)
{
	Vector2D result;
	result.X = this->X - a.X;
	result.Y = this->Y - a.Y;
	return result;
}

/*********************** VECTOR3D ***********************/
Vector3D::Vector3D()
{
	this->X = 0;
	this->Y = 0;
	this->Z = 0;
}

Vector3D::Vector3D(double x, double y, double z)
{
	this->X = x;
	this->Y = y;
	this->Z = z;
}

double Vector3D::Length()
{
	return sqrt(pow(sqrt(pow(this->X, 2) + pow(this->Y, 2)), 2) + pow(this->Z, 2));
}

double Vector3D::DistanceTo(const Vector3D& a)
{
	Vector3D result_vec = *this - a;
	return result_vec.Length();
}

Vector3D Vector3D::operator+(const Vector3D& a)
{
	Vector3D result;
	result.X = this->X + a.X;
	result.Y = this->Y + a.Y;
	result.Z = this->Z + a.Z;
	return result;
}

Vector3D Vector3D::operator-(const Vector3D& a)
{
	Vector3D result;
	result.X = this->X - a.X;
	result.Y = this->Y - a.Y;
	result.Z = this->Z - a.Z;
	return result;
}