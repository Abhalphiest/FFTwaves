#include "vector.h"

// File: vector
// Author: Margaret Dorsey
//
// The vector files provide the basic vector operations,
// for use in FFT.
// probably could have used GLM for this, but wasn't thinking about it
// while implementing
//

vector3::vector3() : x(0.0f), y(0.0f), z(0.0f) { }
vector3::vector3(float x, float y, float z) : x(x), y(y), z(z) { }

//all following functions are really self documenting and
//simple, so I won't bother commenting on them.
// I almost certainly grabbed all of them from one source or another at some point,
// but I can't remember where.

//They're just op overloads and vector operations, nothing mindblowing.

float vector3::operator*(const vector3& v) {
	return this->x*v.x + this->y*v.y + this->z*v.z;
}

vector3 vector3::cross(const vector3& v) {
	return vector3(this->y*v.z - this->z*v.y, this->z*v.x - this->x*v.z, this->x*v.y - this->y*v.z);
}

vector3 vector3::operator+(const vector3& v) {
	return vector3(this->x + v.x, this->y + v.y, this->z + v.z);
}

vector3 vector3::operator-(const vector3& v) {
	return vector3(this->x - v.x, this->y - v.y, this->z - v.z);
}

vector3 vector3::operator*(const float s) {
	return vector3(this->x*s, this->y*s, this->z*s);
}

vector3& vector3::operator=(const vector3& v) {
	this->x = v.x; this->y = v.y; this->z = v.z;
	return *this;
}

float vector3::length() {
	return (float)sqrt(this->x*this->x + this->y*this->y + this->z*this->z);
}

vector3 vector3::unit() {
	float l = this->length();
	return vector3(this->x / l, this->y / l, this->z / l);
}



vector2::vector2() : x(0.0f), y(0.0f) { }
vector2::vector2(float x, float y) : x(x), y(y) { }

float vector2::operator*(const vector2& v) {
	return this->x*v.x + this->y*v.y;
}

vector2 vector2::operator+(const vector2& v) {
	return vector2(this->x + v.x, this->y + v.y);
}

vector2 vector2::operator-(const vector2& v) {
	return vector2(this->x - v.x, this->y - v.y);
}

vector2 vector2::operator*(const float s) {
	return vector2(this->x*s, this->y*s);
}

vector2& vector2::operator=(const vector2& v) {
	this->x = v.x; this->y = v.y;
	return *this;
}

float vector2::length() {
	return (float)sqrt(this->x*this->x + this->y*this->y);
}

vector2 vector2::unit() {
	float l = this->length();
	return vector2(this->x / l, this->y / l);
}
