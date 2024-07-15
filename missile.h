#pragma once

#include <glm/glm.hpp>
#include <SDL/SDL.h>
#include <target.h>
#include <array>

class Missile
{
public:
	Missile(glm::vec2 target, float speed);

	glm::vec2 calcLOS(glm::vec2 target);
	void tick(const Target& target, float dt);
	void draw(SDL_Renderer* r);

	glm::vec2 getVelocity() const { return m_Vel; }

private:
	glm::vec2 m_Pos, m_Vel, m_Acc;
	glm::vec2 m_InitialLOS, m_LOS;
	float m_prevLOSAngle;
};