#pragma once
#include <math.h>

class _Vector2
{
public:
	float x = 0.f, y = 0.f;
	_Vector2() { }
	_Vector2(float x, float y) { this->x = x; this->y = y; }
protected:
	void set(float x, float y) { this->x = x; this->y = y; }
	_Vector2& get() { return *this; }
};

class _Vector3
{
public:
	float x = 0.f, y = 0.f, z = 0.f;
	_Vector3() { }
	_Vector3(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
protected:
	void set(float x, float y, float z) { this->x = x; this->y = y; this->z = z; }
	_Vector3& get() { return *this; }
};

class _Vector4
{
public:
	float x = 0.f, y = 0.f, z = 0.f, w = 0.f;
	_Vector4() { }
	_Vector4(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; }
protected:
	void set(float x, float y, float z, float w) { this->x = x; this->y = y; this->z = z; this->w = w; }
	_Vector4& get() { return *this; }
};

class Vector2 : public _Vector2
{
public:
	Vector2() {  }
	Vector2(float x) { set(x, x); }
	Vector2(float x, float y) { set(x, y); }

	void operator=(_Vector3 v) { set(v.x, v.y); }
	void operator=(_Vector4 v) { set(v.x, v.y); }
	
	bool operator<(Vector2 v) { return (v.x > x && v.y > y); }
	bool operator>(Vector2 v) { return (v.x < x && v.y < y); }
	bool operator<=(Vector2 v) { return (v.x >= x && v.y >= y); }
	bool operator>=(Vector2 v) { return (v.x <= x && v.y <= y); }
	bool operator!=(Vector2 v) { return (v.x != x || v.y != y); }

	Vector2 operator+(float v) { return Vector2(x + v, y + v); }
	Vector2 operator-(float v) { return Vector2(x - v, y - v); }
	Vector2 operator/(float v) { return Vector2(x / v, y / v); }
	Vector2 operator*(float v) { return Vector2(x * v, y * v); }
	Vector2 operator+(_Vector2 v) { return Vector2(x + v.x, y + v.y); }
	Vector2 operator+(_Vector3 v) { return Vector2(x + v.x, y + v.y); }
	Vector2 operator+(_Vector4 v) { return Vector2(x + v.x, y + v.y); }
	Vector2 operator-(_Vector2 v) { return Vector2(x - v.x, y - v.y); }
	Vector2 operator-(_Vector3 v) { return Vector2(x - v.x, y - v.y); }
	Vector2 operator-(_Vector4 v) { return Vector2(x - v.x, y - v.y); }
	Vector2 operator/(_Vector2 v) { return Vector2(x / v.x, y / v.y); }
	Vector2 operator/(_Vector3 v) { return Vector2(x / v.x, y / v.y); }
	Vector2 operator/(_Vector4 v) { return Vector2(x / v.x, y / v.y); }
	Vector2 operator*(_Vector2 v) { return Vector2(x * v.x, y * v.y); }
	Vector2 operator*(_Vector3 v) { return Vector2(x * v.x, y * v.y); }
	Vector2 operator*(_Vector4 v) { return Vector2(x * v.x, y * v.y); }
	Vector2 operator+=(float v) { set(v + x, v + y); return *this; }
	Vector2 operator-=(float v) { set(v - x, v - y); return *this; }
	Vector2 operator/=(float v) { set(v / x, v / y); return *this; }
	Vector2 operator*=(float v) { set(v * x, v * y); return *this; }
	Vector2 operator+=(_Vector2 v) { set(x + v.x, y + v.y); return *this; }
	Vector2 operator+=(_Vector3 v) { set(x + v.x, y + v.y); return *this; }
	Vector2 operator+=(_Vector4 v) { set(x + v.x, y + v.y); return *this; }
	Vector2 operator-=(_Vector2 v) { set(x - v.x, y - v.y); return *this; }
	Vector2 operator-=(_Vector3 v) { set(x - v.x, y - v.y); return *this; }
	Vector2 operator-=(_Vector4 v) { set(x - v.x, y - v.y); return *this; }
	Vector2 operator/=(_Vector2 v) { set(x / v.x, y / v.y); return *this; }
	Vector2 operator/=(_Vector3 v) { set(x / v.x, y / v.y); return *this; }
	Vector2 operator/=(_Vector4 v) { set(x / v.x, y / v.y); return *this; }
	Vector2 operator*=(_Vector2 v) { set(x * v.x, y * v.y); return *this; }
	Vector2 operator*=(_Vector3 v) { set(x * v.x, y * v.y); return *this; }
	Vector2 operator*=(_Vector4 v) { set(x * v.x, y * v.y); return *this; }

	Vector2 to_Vector2() { return Vector2(x, y); }
	_Vector3 to_Vector3() { return _Vector3(x, y, 0.f); }
	_Vector4 to_Vector4() { return _Vector4(x, y, 0.f, 0.f); }

	float dot(_Vector2 v) { return x * v.x + y * v.y; }
	float distance(_Vector2 v) { return sqrtf(powf(x - v.x, 2.f) + powf(y - v.y, 2.f)); }
};

class Vector3 : public _Vector3
{
public:
	Vector3() {  }
	Vector3(float x) { set(x, x, x); }
	Vector3(float x, float y, float z) { set(x, y, z); }

	void operator=(_Vector2 v) { set(v.x, v.y, 0.f); }
	void operator=(_Vector4 v) { set(v.x, v.y, v.z); }

	bool operator<(Vector3 v) { return (v.x > x && v.y > y && v.z > z); }
	bool operator>(Vector3 v) { return (v.x < x && v.y < y && v.z < z); }
	bool operator<=(Vector3 v) { return (v.x >= x && v.y >= y && v.z >= z); }
	bool operator>=(Vector3 v) { return (v.x <= x && v.y <= y && v.z <= z); }
	bool operator!=(Vector3 v) { return ((DWORD)v.x != (DWORD)x || (DWORD)v.y != (DWORD)y || (DWORD)v.z != (DWORD)z); }
	bool operator==(Vector3 v) { return ((DWORD)v.x == (DWORD)x && (DWORD)v.y == (DWORD)y && (DWORD)v.z == (DWORD)z); }

	Vector3 operator+(float v) { return Vector3(x + v, y + v, z + v); }
	Vector3 operator-(float v) { return Vector3(x - v, y - v, z - v); }
	Vector3 operator/(float v) { return Vector3(x / v, y / v, z / v); }
	Vector3 operator*(float v) { return Vector3(x * v, y * v, z * v); }
	Vector3 operator+(_Vector2 v) { return Vector3(x + v.x, y + v.y, z + 0.f); }
	Vector3 operator+(_Vector3 v) { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3 operator+(_Vector4 v) { return Vector3(x + v.x, y + v.y, z + v.z); }
	Vector3 operator-(_Vector2 v) { return Vector3(x - v.x, y - v.y, z - 0.f); }
	Vector3 operator-(_Vector3 v) { return Vector3(x - v.x, y - v.y, z - v.z); }
	Vector3 operator-(_Vector4 v) { return Vector3(x - v.x, y - v.y, z - v.z); }
	Vector3 operator/(_Vector2 v) { return Vector3(x / v.x, y / v.y, z / 0.f); }
	Vector3 operator/(_Vector3 v) { return Vector3(x / v.x, y / v.y, z / v.z); }
	Vector3 operator/(_Vector4 v) { return Vector3(x / v.x, y / v.y, z / v.z); }
	Vector3 operator*(_Vector2 v) { return Vector3(x * v.x, y * v.y, z * 0.f); }
	Vector3 operator*(_Vector3 v) { return Vector3(x * v.x, y * v.y, z * v.z); }
	Vector3 operator*(_Vector4 v) { return Vector3(x * v.x, y * v.y, z * v.z); }
	Vector3 operator+=(float v) { set(x + v, y + v, z + v); return *this; }
	Vector3 operator-=(float v) { set(x - v, y - v, z - v); return *this; }
	Vector3 operator/=(float v) { set(x / v, y / v, z / v); return *this; }
	Vector3 operator*=(float v) { set(x * v, y * v, z * v); return *this; }
	Vector3 operator+=(_Vector2 v) { set(x + v.x, y + v.y, z + 0.f); return *this; }
	Vector3 operator+=(_Vector3 v) { set(x + v.x, y + v.y, z + v.z); return *this; }
	Vector3 operator+=(_Vector4 v) { set(x + v.x, y + v.y, z + v.z); return *this; }
	Vector3 operator-=(_Vector2 v) { set(x - v.x, y - v.y, z - 0.f); return *this; }
	Vector3 operator-=(_Vector3 v) { set(x - v.x, y - v.y, z - v.z); return *this; }
	Vector3 operator-=(_Vector4 v) { set(x - v.x, y - v.y, z - v.z); return *this; }
	Vector3 operator/=(_Vector2 v) { set(x / v.x, y / v.y, z / 0.f); return *this; }
	Vector3 operator/=(_Vector3 v) { set(x / v.x, y / v.y, z / v.z); return *this; }
	Vector3 operator/=(_Vector4 v) { set(x / v.x, y / v.y, z / v.z); return *this; }
	Vector3 operator*=(_Vector2 v) { set(x * v.x, y * v.y, z * 0.f); return *this; }
	Vector3 operator*=(_Vector3 v) { set(x * v.x, y * v.y, z * v.z); return *this; }
	Vector3 operator*=(_Vector4 v) { set(x * v.x, y * v.y, z * v.z); return *this; }

	Vector2 to_Vector2() { return Vector2(x, y); }
	Vector3 to_Vector3() { return Vector3(x, y, z); }
	_Vector4 to_Vector4() { return _Vector4(x, y, z, 0.f); }

	float dot(_Vector3 v) { return x * v.x + y * v.y + z * v.z; }
	float distance(_Vector3 v) { return sqrtf(powf(x - v.x, 2.f) + powf(y - v.y, 2.f) + powf(z - v.z, 2.f)); }
};

class Vector4 : public _Vector4
{
public:
	Vector4() {  }
	Vector4(float x) { set(x, x, x, x); }
	Vector4(float x, float y, float z, float w) { set(x, y, z, w); }

	void operator=(_Vector2 v) { set(v.x, v.y, 0.f, 0.f); }
	void operator=(_Vector3 v) { set(v.x, v.y, v.z, 0.f); }

	Vector4 operator+(float v) { return Vector4(x + v, y + v, z + v, w + v); }
	Vector4 operator-(float v) { return Vector4(x - v, y - v, z - v, w - v); }
	Vector4 operator/(float v) { return Vector4(x / v, y / v, z / v, w / v); }
	Vector4 operator*(float v) { return Vector4(x * v, y * v, z * v, w * v); }
	Vector4 operator+(_Vector2 v) { return Vector4(x + v.x, y + v.y, z + 0.f, w + 0.f); }
	Vector4 operator+(_Vector3 v) { return Vector4(x + v.x, y + v.y, z + v.z, w + 0.f); }
	Vector4 operator+(_Vector4 v) { return Vector4(x + v.x, y + v.y, z + v.z, w + v.w); }
	Vector4 operator-(_Vector2 v) { return Vector4(x - v.x, y - v.y, z - 0.f, w - 0.f); }
	Vector4 operator-(_Vector3 v) { return Vector4(x - v.x, y - v.y, z - v.z, w - 0.f); }
	Vector4 operator-(_Vector4 v) { return Vector4(x - v.x, y - v.y, z - v.z, w - v.w); }
	Vector4 operator/(_Vector2 v) { return Vector4(x / v.x, y / v.y, z / 0.f, w / 0.f); }
	Vector4 operator/(_Vector3 v) { return Vector4(x / v.x, y / v.y, z / v.z, w / 0.f); }
	Vector4 operator/(_Vector4 v) { return Vector4(x / v.x, y / v.y, z / v.z, w / v.w); }
	Vector4 operator*(_Vector2 v) { return Vector4(x * v.x, y * v.y, z * 0.f, w * 0.f); }
	Vector4 operator*(_Vector3 v) { return Vector4(x * v.x, y * v.y, z * v.z, w * 0.f); }
	Vector4 operator*(_Vector4 v) { return Vector4(x * v.x, y * v.y, z * v.z, w * v.w); }
	Vector4 operator+=(float v) { set(x + v, y + v, z + v, w + v); return *this; }
	Vector4 operator-=(float v) { set(x - v, y - v, z - v, w - v); return *this; }
	Vector4 operator/=(float v) { set(x / v, y / v, z / v, w / v); return *this; }
	Vector4 operator*=(float v) { set(x * v, y * v, z * v, w * v); return *this; }
	Vector4 operator+=(_Vector2 v) { set(x + v.x, y + v.y, z + 0.f, w + 0.f); return *this; }
	Vector4 operator+=(_Vector3 v) { set(x + v.x, y + v.y, z + v.z, w + 0.f); return *this; }
	Vector4 operator+=(_Vector4 v) { set(x + v.x, y + v.y, z + v.z, w + v.w); return *this; }
	Vector4 operator-=(_Vector2 v) { set(x - v.x, y - v.y, z - 0.f, w - 0.f); return *this; }
	Vector4 operator-=(_Vector3 v) { set(x - v.x, y - v.y, z - v.z, w - 0.f); return *this; }
	Vector4 operator-=(_Vector4 v) { set(x - v.x, y - v.y, z - v.z, w - v.w); return *this; }
	Vector4 operator/=(_Vector2 v) { set(x / v.x, y / v.y, z / 0.f, w / 0.f); return *this; }
	Vector4 operator/=(_Vector3 v) { set(x / v.x, y / v.y, z / v.z, w / 0.f); return *this; }
	Vector4 operator/=(_Vector4 v) { set(x / v.x, y / v.y, z / v.z, w / v.w); return *this; }
	Vector4 operator*=(_Vector2 v) { set(x * v.x, y * v.y, z * 0.f, w * 0.f); return *this; }
	Vector4 operator*=(_Vector3 v) { set(x * v.x, y * v.y, z * v.z, w * 0.f); return *this; }
	Vector4 operator*=(_Vector4 v) { set(x * v.x, y * v.y, z * v.z, w * v.w); return *this; }

	Vector2 to_Vector2() { return Vector2(x, y); }
	Vector3 to_Vector3() { return Vector3(x, y, z); }
	Vector4 to_Vector4() { return Vector4(x, y, z, w); }

	float dot(_Vector4 v) { return x * v.x + y * v.y + z * v.z + w * v.w; }
	float distance(_Vector4 v) { return sqrtf(powf(x - v.x, 2.f) + powf(y - v.y, 2.f) + powf(z - v.z, 2.f) + powf(w - v.w, 2.f)); }
};