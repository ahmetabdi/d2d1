#ifndef MATH_H
#define MATH_H

#pragma once

class Vector3 {
public:
	float x, y, z;

	Vector3() {
		x = y = z = 0.0f;
	}

	Vector3(float X, float Y, float Z) {
		x = X; y = Y; z = Z;
	}

	Vector3(float XYZ) {
		x = XYZ; y = XYZ; z = XYZ;
	}

	Vector3(float* v) {
		x = v[0]; y = v[1]; z = v[2];
	}

	Vector3(const float* v) {
		x = v[0]; y = v[1]; z = v[2];
	}

	inline Vector3& operator=(const Vector3& v) {
		x = v.x; y = v.y; z = v.z; return *this;
	}

	inline Vector3& operator=(const float* v) {
		x = v[0]; y = v[1]; z = v[2]; return *this;
	}

	inline float& operator[](int i) {
		return ((float*)this)[i];
	}

	inline float operator[](int i) const {
		return ((float*)this)[i];
	}

	inline Vector3& operator+=(const Vector3& v) {
		x += v.x; y += v.y; z += v.z; return *this;
	}

	inline Vector3& operator-=(const Vector3& v) {
		x -= v.x; y -= v.y; z -= v.z; return *this;
	}

	inline Vector3& operator*=(const Vector3& v) {
		x *= v.x; y *= v.y; z *= v.z; return *this;
	}

	inline Vector3& operator/=(const Vector3& v) {
		x /= v.x; y /= v.y; z /= v.z; return *this;
	}

	inline Vector3& operator+=(float v) {
		x += v; y += v; z += v; return *this;
	}

	inline Vector3& operator-=(float v) {
		x -= v; y -= v; z -= v; return *this;
	}

	inline Vector3& operator*=(float v) {
		x *= v; y *= v; z *= v; return *this;
	}

	inline Vector3& operator/=(float v) {
		x /= v; y /= v; z /= v; return *this;
	}

	inline Vector3 operator-() const {
		return Vector3(-x, -y, -z);
	}

	inline Vector3 operator+(const Vector3& v) const {
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	inline Vector3 operator-(const Vector3& v) const {
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	inline Vector3 operator*(const Vector3& v) const {
		return Vector3(x * v.x, y * v.y, z * v.z);
	}

	inline Vector3 operator/(const Vector3& v) const {
		return Vector3(x / v.x, y / v.y, z / v.z);
	}

	inline Vector3 operator+(float v) const {
		return Vector3(x + v, y + v, z + v);
	}

	inline Vector3 operator-(float v) const {
		return Vector3(x - v, y - v, z - v);
	}

	inline Vector3 operator*(float v) const {
		return Vector3(x * v, y * v, z * v);
	}

	inline Vector3 operator/(float v) const {
		return Vector3(x / v, y / v, z / v);
	}

	inline float Length() const {
		return sqrtf(x * x + y * y + z * z);
	}

	inline float LengthSqr() const {
		return (x * x + y * y + z * z);
	}

	inline float LengthXY() const {
		return sqrtf(x * x + y * y);
	}

	inline float LengthXZ() const {
		return sqrtf(x * x + z * z);
	}

	inline float DistTo(const Vector3& v) const {
		return (*this - v).Length();
	}

	inline float Dot(const Vector3& v) const {
		return (x * v.x + y * v.y + z * v.z);
	}

	inline Vector3 Cross(const Vector3& v) const {
		return Vector3(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}

	inline bool IsZero() const {
		return (x > -0.01f && x < 0.01f
			&&	y > -0.01f && y < 0.01f
			&&	z > -0.01f && z < 0.01f);
	}
};

class Matrix4x4 {
public:
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};

		float m[4][4];
		float mm[16];

		struct {
			__m128 m1, m2, m3, m4;
		};
	};

	inline void Transpose() {
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				m[i][j] = m[j][i];
			}
		}
	}

	inline Vector3& GetAxis(int i) {
		return *(Vector3*)&m[i][0];
	}
};

class Matrix3x4 {
public:
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
		};

		float m[3][4];
		float mm[12];
	};

	inline Vector3& GetAxis(int i) {
		return *(Vector3*)&m[i][0];
	}
};

#endif // MATH_H