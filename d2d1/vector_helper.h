#pragma once 

#include <iostream> 

#define M_PI        3.14159265358979323846 

class Vector2D
{
public:
	Vector2D();
	Vector2D(double, double);
	double Length();
	double ToAngle();
	Vector2D operator+(const Vector2D& a);
	Vector2D operator-(const Vector2D& a);
	double X, Y;
};

class Vector3D
{
public:
	Vector3D();
	Vector3D(double, double, double);
	double Length();
	double DistanceTo(const Vector3D&);
	Vector3D operator+(const Vector3D& a);
	Vector3D operator-(const Vector3D& a);
	double X, Y, Z;
};