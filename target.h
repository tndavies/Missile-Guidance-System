#pragma once

#include <SDL/SDL.h>
#include <glm/glm.hpp>

class Target {
public:
	Target(glm::vec2 pos, float size) :
		m_Pos(pos), m_Size(size) {}
	
	void tick(float dt);
	void draw(SDL_Renderer* r);

	glm::vec2 velocity() const { return glm::vec2(65.0f,0.0f); }
	auto getSize() const { return m_Size; }
	auto getPos() const { return m_Pos; }

private:
	glm::vec2 m_Pos;
	float m_Size;
};