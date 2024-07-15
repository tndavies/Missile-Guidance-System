#include "target.h"
#include <coords.h>

Target::Target(glm::vec2 pos, float size) 
	: m_Pos(toSimSpace(pos)), m_Size(size)
{
	m_Speed = 75.0f;
	m_Vel = { m_Speed, 0.0f };
	m_Acc = { 15.0f, -9.0f }; // Constant acceleration vector.
	
	m_ResetPos = m_Pos;
}

void Target::tick(float dt)
{
	m_Vel += m_Acc * dt;
	m_Pos += m_Vel * dt;
}

void Target::draw(SDL_Renderer* r)
{
	const auto drawPos = toSDLSpace(m_Pos); // Convert from simulation to SDL coordinates.
	SDL_Rect visual = { drawPos.x - 0.5f * m_Size, drawPos.y - 0.5f * m_Size, m_Size, m_Size };

	SDL_SetRenderDrawColor(r, 100, 100, 150, 0xff);
	SDL_RenderFillRect(r, &visual);
}

void Target::reset() {
	m_Vel = { m_Speed, 0.0f };
	m_Pos = m_ResetPos;
}
