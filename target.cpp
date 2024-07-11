#include "target.h"

void Target::tick(float dt)
{
	m_Vel += glm::vec2(15.0f, 9.0f) * dt;
	m_Pos += m_Vel * dt;
}

void Target::draw(SDL_Renderer* r)
{
	SDL_Rect visual = { m_Pos.x - 0.5f * m_Size, m_Pos.y - 0.5f * m_Size, m_Size, m_Size };
	SDL_SetRenderDrawColor(r, 100, 100, 150, 0xff);
	SDL_RenderFillRect(r, &visual);
}
