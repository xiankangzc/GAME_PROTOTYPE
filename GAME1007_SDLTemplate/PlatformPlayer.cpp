#include "PlatformPlayer.h"
#include <algorithm>

void PlatformPlayer::Init(SDL_Renderer* r)
{
	m_rend = r;
	m_rect = { 462,0,50,100 };
	m_grounded = true;

	m_accelerationX = m_accelerationY = m_velX = m_velY = 0.0;
	m_maxVelX = 10.0;
	m_maxVelY = JUMPFORCE;
	m_gravity = GRAVITY;
	m_drag = 0.88;
}

SDL_Rect PlatformPlayer::GetRect()
{
	return m_rect;
}

void PlatformPlayer::Update()
{
	// X axis
	m_velX += m_accelerationX;
	m_velX *= (m_grounded ? m_drag : 1.0);
	m_velX = std::min(std::max(m_velX, -m_maxVelX), m_maxVelX);
	m_rect.x += (int)m_velX;
	// Y axis
	m_velY += m_accelerationY + m_gravity;
	m_velY = std::min(std::max(m_velY, -m_maxVelY), (m_gravity*3.0));
	m_rect.y += (int)m_velY;
	// Reset acceleration.
	m_accelerationX = m_accelerationY = 0.0;
}

void PlatformPlayer::Render()
{
	SDL_SetRenderDrawColor(m_rend, 255, 255, 255, 255);
	SDL_RenderFillRect(m_rend, &GetRect());
}

void PlatformPlayer::Stop()
{
	StopX();
	StopY();
}

void PlatformPlayer::StopX() {	m_velX = 0.0; }

void PlatformPlayer::StopY() {	m_velY = 0.0; }

void PlatformPlayer::SetAccelX(double a) { m_accelerationX = a; }

void PlatformPlayer::SetAccelY(double a) { m_accelerationY = a; }

bool PlatformPlayer::isGrounded() { return m_grounded; }

void PlatformPlayer::SetGrounded(bool g) { m_grounded = g; }

double PlatformPlayer::GetVelX() { return m_velX; }

double PlatformPlayer::GetVelY() { return m_velY; }

void PlatformPlayer::SetX(float x) { m_rect.x = x; }

void PlatformPlayer::SetY(float y) { m_rect.y = y; }

