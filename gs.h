#pragma once

#include <SDL/SDL.h>
#include <target.h>
#include <glm/glm.hpp>
#include <cmath>
#include <missile.h>

class GuidanceSystem {
public:
	GuidanceSystem(const glm::vec2& pos, float range, float fov);
	~GuidanceSystem() {}

	void tick(Target& target, float dt);

	auto getSize() const { return m_Size; }
	auto getPos() const { return m_Pos; }
	auto getRange() const { return m_Range; }
	auto getFOV() const { return m_FOV; }
	auto getA() const { return m_FrustumTopLeft; }
	auto getB() const { return m_FrustumTopRight; }
	auto getMissile() const { return m_Missile; }

	void draw(SDL_Renderer* r);
	bool TargetVisible(const glm::vec2& targetPos);

private:
	const float m_Size;

	glm::vec2 m_Pos;
	glm::vec2 m_FrustumTopLeft;
	glm::vec2 m_FrustumTopRight;
	float m_Range;
	float m_FOV;

	bool m_TargetRegistered;

	Missile* m_Missile;
};