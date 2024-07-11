#pragma once
#include <glm/glm.hpp>
#include <SDL/SDL.h>
#include <array>
#include <target.h>

class Missile
{
public:
	Missile(glm::vec2 target, float speed);

	glm::vec2 velocity() const { return m_Vel; }
	void tick(const Target& target, float dt);
	void draw(SDL_Renderer* r);

private:
	glm::vec2 m_Pos;
	glm::vec2 m_InitialLOS;
	glm::vec2 m_CurrLOS;
	glm::vec2 m_Vel;
	float m_Speed;
	float m_prevLOSA;
};