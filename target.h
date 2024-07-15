#pragma once

#include <SDL/SDL.h>
#include <glm/glm.hpp>

class Target {
public:
	Target(glm::vec2 pos, float size);
	
	void reset();
	void tick(float dt);
	void draw(SDL_Renderer* r);

	glm::vec2 getVelocity() const { return m_Vel; }
	auto getSpeed() const { return m_Speed; }
	auto getPos() const { return m_Pos; }
	auto getSize() const { return m_Size; }

private:
	glm::vec2 m_Pos, m_ResetPos, m_Vel, m_Acc;
	float m_Speed;
	float m_Size;
};