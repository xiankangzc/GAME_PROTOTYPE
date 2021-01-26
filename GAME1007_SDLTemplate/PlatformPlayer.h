#pragma once
#ifndef _PLATFORMPLAYER_H_
#define _PLATFORMPLAYER_H_
#define GRAVITY 6.0
#define JUMPFORCE 60
#include "SDL.h"


class PlatformPlayer
{

private:
	bool m_grounded;//is the player in grounded or in the air
	double m_accelerationX,
		m_accelerationY,
		m_velX,
		m_maxVelX,
		m_velY,
		m_maxVelY,
		m_drag,
		m_gravity;
	SDL_Rect m_rect;
	SDL_Renderer* m_rend;
public:
	
	void Init(SDL_Renderer* r);
	SDL_Rect GetRect();
	void Update();
	void Render();
	void Stop();
	void StopX();
	void StopY();
	void SetAccelX(double a);
	void SetAccelY(double a);
	bool isGrounded();
	void SetGrounded(bool g);
	double GetVelX();
	double GetVelY();
	void SetX(float x);
	void SetY(float y);

};

#endif // !_PLATFORMPLAYER_H_
