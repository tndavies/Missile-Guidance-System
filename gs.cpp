#include <coords.h>
#include <cmath>
#include <gs.h>

GuidanceSystem::GuidanceSystem(const glm::vec2& pos, float range, float fov) 
	: m_Pos(toSimSpace(pos)), m_Range(range), m_FOV(fov), m_TargetRegistered(false)
{
	// Calculate top-left & top-right points of view frustum, 
	// from specified view range & FOV.
	const float alpha = glm::radians(m_FOV) / 2.0f;
	const float dx = m_Range * std::sinf(alpha);
	const float dy = m_Range * std::cosf(alpha);

	// note: Calculation is done in SDL space!
	m_FrustumTopLeft.x = pos.x - dx;
	m_FrustumTopLeft.y = pos.y - dy;
	m_FrustumTopRight.x = pos.x + dx;
	m_FrustumTopRight.y = pos.y - dy;
}

void GuidanceSystem::draw(SDL_Renderer* r) 
{
	const auto pos = toSDLSpace(m_Pos);
	const auto A = m_FrustumTopLeft;
	const auto B = m_FrustumTopRight;

	// Colour frustum outline red if target is visible,
	// black otherwise.
	const uint8_t RedChannel = m_TargetRegistered ? 0xff : 0x00;
	SDL_SetRenderDrawColor(r, RedChannel, 0, 0, 0xff);

	// Draw frustum outline.
	SDL_RenderDrawLineF(r, pos.x, pos.y, A.x, A.y);
	SDL_RenderDrawLineF(r, pos.x, pos.y, B.x, B.y);
	SDL_RenderDrawLineF(r, A.x, A.y, B.x, B.y);

	// Draw Guidance-system square.
	SDL_Rect visual = { pos.x - 0.5f * m_Size, pos.y - m_Size, m_Size, m_Size };
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
	SDL_RenderFillRect(r, &visual);

	if (m_Missile) {
		m_Missile->draw(r);
	}
}

void GuidanceSystem::tick(Target& target, float dt)
{
	// Detect target when it enters the GS's visible region.
	bool targetVisible = TargetVisible(target.getPos());
	if (targetVisible && !m_TargetRegistered) {
		const auto missileSpeed = 2.5f * target.getSpeed();
		m_Missile = new Missile(target.getPos(), missileSpeed);

		m_TargetRegistered = true;
	}

	if (m_Missile) {
		m_Missile->tick(target, dt);

		// Detect when the missile successfully strikes the target.
		const auto separationVec = m_Missile->calcLOS(target.getPos());
		if (glm::length(separationVec) <= target.getSize()) {
			m_TargetRegistered = false;
			delete m_Missile;
			target.reset();
		}
	}
}

// Performs a point-in-triangle test to check if the target 
// is within the view frustum of the guidance system.
// (perfomed in SDL space)
bool GuidanceSystem::TargetVisible(const glm::vec2& targetPos)
{
	glm::vec2 P = toSDLSpace(targetPos);
	glm::vec2 G = toSDLSpace(m_Pos);
	glm::vec2 A = m_FrustumTopLeft;
	glm::vec2 B = m_FrustumTopRight;

	glm::vec2 v0 = G - A;
	glm::vec2 v1 = B - A;
	glm::vec2 v2 = P - A;

	auto d00 = glm::dot(v0, v0);
	auto d01 = glm::dot(v0, v1);
	auto d02 = glm::dot(v0, v2);
	auto d11 = glm::dot(v1, v1);
	auto d12 = glm::dot(v1, v2);

	float invDenom = 1 / (d00 * d11 - d01 * d01);
	float u = (d11 * d02 - d01 * d12) * invDenom;
	float v = (d00 * d12 - d01 * d02) * invDenom;

	return (u >= 0.0f) && (v >= 0.0f) && (u + v <= 1.0f);
}