#pragma once

#include <SDL/SDL.h>
#include <target.h>
#include <glm/glm.hpp>
#include <cmath>
#include <missile.h>

class GuidanceSystem {
public:
	GuidanceSystem(float x, float y, float range, float fov);
	~GuidanceSystem() {}

	void tick(Target& target, float dt);

	auto getSize() const { return m_Size; }
	auto getX() const { return m_X; }
	auto getY() const { return m_Y; }
	auto getRange() const { return m_Range; }
	auto getFOV() const { return m_FOV; }
	auto getA() const { return m_FrustumA; }
	auto getB() const { return m_FrustumB; }
	auto getMissile() const { return m_ActiveMissile; }

	void draw(SDL_Renderer* r);
	bool TargetVisible(float tx, float ty);

private:
	const float m_Size = 15.0f;

	// in SDL coords.
	float m_X, m_Y;
	glm::vec2 m_FrustumA;
	glm::vec2 m_FrustumB;
	
	float m_Range;
	float m_FOV;


	bool m_ThreatDetected;

	Missile* m_ActiveMissile = nullptr;
};