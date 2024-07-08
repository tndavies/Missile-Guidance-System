#include "gs.h"
#include <cmath>
#include <iostream>

#define Radians(x) (0.01745329251994329576924 * x) 

GuidanceSystem::GuidanceSystem(float x, float y, float size, float range, float fov) 
	: m_X(x), m_Y(y), m_Size(size), m_Range(range), m_FOV(fov), m_ThreatDetected(false)
{
	const float alpha = 0.5f * Radians(m_FOV);
	const float dx = m_Range * std::sinf(alpha);
	const float dy = m_Range * std::cosf(alpha);

	m_FrustumA.x = m_X - dx;
	m_FrustumA.y = m_Y - dy;

	m_FrustumB.x = m_X + dx;
	m_FrustumB.y = m_Y - dy;
}

void GuidanceSystem::tick(const Target& target)
{
	bool visible = TargetVisible(target.getX(), target.getY());
	if (visible && !m_ThreatDetected) {
		m_ThreatDetected = true;

		const auto coordRatio = (target.getX() - m_X) / (target.getY() - m_Y);
		const float launchAngle = std::atanf(coordRatio);

		std::cout << launchAngle * 57.3f << std::endl;

		const float launchSpeed = 0.05f;

		m_ActiveMissile = new Missile(m_X, m_Y, launchAngle, launchSpeed);
	}
	else if (m_ThreatDetected && !visible) {
		m_ThreatDetected = false;
	}

	if (m_ActiveMissile) m_ActiveMissile->tick();
}

// Uses barycentric coordinates to test if the target is
// within the view frustum of the guidance system's sensors.
bool GuidanceSystem::TargetVisible(float tx, float ty) 
{
	vec2 P (tx, ty);
	vec2 G (m_X, m_Y);
	vec2 A = m_FrustumA;
	vec2 B = m_FrustumB;

	vec2 v0 = G - A;
	vec2 v1 = B - A;
	vec2 v2 = P - A;

	auto d00 = vec2::dot(v0, v0);
	auto d01 = vec2::dot(v0, v1);
	auto d02 = vec2::dot(v0, v2);
	auto d11 = vec2::dot(v1, v1);
	auto d12 = vec2::dot(v1, v2);

	float invDenom = 1 / (d00 * d11 - d01 * d01);
	float u = (d11 * d02 - d01 * d12) * invDenom;
	float v = (d00 * d12 - d01 * d02) * invDenom;

	return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}

Missile::Missile(float x, float y, float radLaunchAngle, float speed)
	: m_Pos(x, y)
{
	const auto radTheta = radLaunchAngle;
	m_InitialVel.x = speed * std::cosf(radTheta);
	m_InitialVel.y = -speed * std::sinf(radTheta);
}

void Missile::tick()
{
	m_Pos = m_Pos + m_InitialVel;
}
