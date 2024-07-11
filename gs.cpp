#include "gs.h"
#include <cmath>
#include <coords.h>
#include <iostream>

#define Radians(x) (0.01745329251994329576924 * x) 

GuidanceSystem::GuidanceSystem(float x, float y, float range, float fov) 
	: m_X(x), m_Y(y), m_Range(range), m_FOV(fov), m_ThreatDetected(false)
{
	const float alpha = 0.5f * Radians(m_FOV);
	const float dx = m_Range * std::sinf(alpha);
	const float dy = m_Range * std::cosf(alpha);

	m_FrustumA.x = m_X - dx;
	m_FrustumA.y = m_Y - dy;

	m_FrustumB.x = m_X + dx;
	m_FrustumB.y = m_Y - dy;
}

void GuidanceSystem::draw(SDL_Renderer* r) 
{
	SDL_Rect visual = { m_X - 0.5f * m_Size, m_Y - m_Size, m_Size, m_Size };
	SDL_SetRenderDrawColor(r, 100, 100, 100, 0xff);
	SDL_RenderDrawRect(r, &visual);

	const auto A = m_FrustumA;
	const auto B = m_FrustumB;
	
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
	SDL_RenderDrawLineF(r, m_X, m_Y, A.x, A.y);
	SDL_RenderDrawLineF(r, m_X, m_Y, B.x, B.y);
	SDL_RenderDrawLineF(r, A.x, A.y, B.x, B.y);

	if (m_ActiveMissile) m_ActiveMissile->draw(r);
}

void GuidanceSystem::tick(Target& target, float dt)
{
	const auto tpos = target.getPos();
	bool visible = TargetVisible(tpos.x, tpos.y);
	if (visible && !m_ThreatDetected) {
		m_ThreatDetected = true;
		
		const auto tpos = SDLtoGS(target.getPos());
		m_ActiveMissile = new Missile(tpos, target.getSpeed() * 2.5f);
	}

	if (m_ActiveMissile) {
		m_ActiveMissile->tick(target, dt);

		//const auto tpos = SDLtoGS(target.getPos());
		//const auto missileLOS = m_ActiveMissile->calcLOS(tpos);
		//std::cout << glm::length(missileLOS) << std::endl;
		//if (glm::length(missileLOS) <= target.getSize()) { // missile hit!
		//	delete m_ActiveMissile;
		//	m_ThreatDetected = false;
		//	target.reset();
		//}
	}
}

// Uses barycentric coordinates to test if the target is
// within the view frustum of the guidance system's sensors.
bool GuidanceSystem::TargetVisible(float tx, float ty)
{
	glm::vec2 P(tx, ty);
	glm::vec2 G(m_X, m_Y);
	glm::vec2 A = m_FrustumA;
	glm::vec2 B = m_FrustumB;

	glm::vec2 v0 = G - A;
	glm::vec2 v1 = B - A;
	glm::vec2 v2 = P - A;

	auto d00 = glm::dot(v0, v0);
	auto d01 = glm::dot(v0, v1);
	auto d02 = glm::dot(v0, v2);
	auto d11 = glm::dot(v1, v1);
	auto d12 = glm::dot(v1, v2);

	float invDenom = 1 / (d00 * d11 - d01 * d01);
	float u = (d11 * d02 - d01 * d12) * invDenom;
	float v = (d00 * d12 - d01 * d02) * invDenom;

	return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}