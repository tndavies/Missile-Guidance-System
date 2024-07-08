#pragma once

#include <target.h>
#include <vec2.h>

class Missile {
public:
	Missile(float x, float y, float radLaunchAngle, float speed);
	~Missile() {}

	auto getPos() const { return m_Pos; }

	void tick();

private:
	vec2 m_Pos;
	vec2 m_InitialVel;
};

class GuidanceSystem {
public:
	GuidanceSystem(float x, float y, float size, float range, float fov);
	~GuidanceSystem() {}

	void tick(const Target& target);

	auto getSize() const { return m_Size; }
	auto getX() const { return m_X; }
	auto getY() const { return m_Y; }
	auto getRange() const { return m_Range; }
	auto getFOV() const { return m_FOV; }
	auto getA() const { return m_FrustumA; }
	auto getB() const { return m_FrustumB; }
	auto getMissile() const { return m_ActiveMissile; }

	bool TargetVisible(float tx, float ty);

private:
	float m_X, m_Y;
	float m_Size;

	float m_Range;
	float m_FOV;

	vec2 m_FrustumA;
	vec2 m_FrustumB;

	bool m_ThreatDetected;

	Missile* m_ActiveMissile = nullptr;
};