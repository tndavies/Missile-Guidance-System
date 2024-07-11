#pragma once

#include <SDL/SDL.h>
#include <glm/glm.hpp>

class Target {
public:
	Target(glm::vec2 pos, float size) :
		m_Pos(pos), m_ResetPos(pos), m_Size(size)
	{
		m_Vel = { m_Speed, 0.0f };
	}
	
	void tick(float dt);
	void draw(SDL_Renderer* r);

	glm::vec2 velocity() const { return m_Vel; }
	auto getSpeed() const { return m_Speed; }
	auto getSize() const { return m_Size; }
	auto getPos() const { return m_Pos; }

	void reset() { 
		m_Pos = m_ResetPos; 
		m_Vel = { m_Speed, 0.0f };
	}

private:
	glm::vec2 m_Pos, m_ResetPos, m_Vel;
	float m_Size;
	float m_Speed = 75.0f;
};