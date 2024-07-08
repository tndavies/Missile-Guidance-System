#pragma once

struct vec2 {

	vec2(float x, float y) : x(x), y(y) {}
	vec2() : x(0.0f), y(0.0f) {}
	~vec2() {}

	vec2 operator+(vec2 const& other) {
		return vec2(x + other.x, y + other.y);
	}

	vec2 operator-(vec2 const& other) {
		return vec2(x - other.x, y - other.y);
	}

	vec2 operator*(float scalar) {
		return vec2(x * scalar, y * scalar);
	}

	static float dot(vec2& v1, vec2& v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	float x, y;
};