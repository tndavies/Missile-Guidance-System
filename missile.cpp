#include <missile.h>
#include <coords.h>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

glm::vec2 calc_trajectory(glm::vec2 from, glm::vec2 to)
{
	return glm::normalize(to - from);
}

Missile::Missile(glm::vec2 target, float speed) :
	m_Pos(0, 0), m_Speed(speed)
{
	m_InitialLOS = calc_trajectory(m_Pos, target);
	m_CurrLOS = m_InitialLOS;
	m_prevLOSA = 0.0f;
}

void Missile::tick(glm::vec2 target_pos, float dt)
{
	//m_Pos = m_Pos + getVelocity() * dt;

	// Guidance algorithm.
	// 1) Calc angle between reference line & target (LOS angle).
	m_CurrLOS = calc_trajectory(m_Pos, target_pos);
	float currLOSA = glm::angle(m_InitialLOS, m_CurrLOS);

	// 2) Calc temporal ROC of LOS angle.
	float rocLOSA = (currLOSA - m_prevLOSA) / dt;
	m_prevLOSA = currLOSA;
	
	// 3) Find missile velocity upon target
	// 4) Use PN formula to find correction acceleration needed.
	// 5) Adjust missile velocity accordingly.


}

void Missile::draw(SDL_Renderer* r)
{
	const auto Size = 5.0f;
	const auto mpos = GStoSDL(m_Pos);
	SDL_FRect visual{ mpos.x - 0.5f * Size, mpos.y - 0.5f * Size, Size, Size };
	SDL_SetRenderDrawColor(r, 0xff, 0, 0, 0xff);
	SDL_RenderFillRectF(r, &visual);

	const auto gspos = GStoSDL(glm::vec2(0, 0));
	const auto velpos = GStoSDL(m_InitialLOS * 60.0f);
	const auto LOS = GStoSDL(m_CurrLOS * 75.0f);

	SDL_SetRenderDrawColor(r, 0xff, 0xff, 0, 0xff);
	SDL_RenderDrawLineF(r, gspos.x, gspos.y, velpos.x, velpos.y);

	SDL_SetRenderDrawColor(r, 255, 255, 255, 0xff);
	SDL_RenderDrawLineF(r, gspos.x, gspos.y, LOS.x, LOS.y);
}