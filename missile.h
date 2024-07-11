#pragma once
#include <glm/glm.hpp>
#include <SDL/SDL.h>
#include <array>

class Missile
{
public:
	Missile(glm::vec2 target, float speed);

	void tick(glm::vec2 target_pos, float dt);
	void draw(SDL_Renderer* r);

private:
	glm::vec2 m_Pos;
	glm::vec2 m_InitialLOS;
	glm::vec2 m_CurrLOS;
	float m_Speed;
	float m_prevLOSA;
};