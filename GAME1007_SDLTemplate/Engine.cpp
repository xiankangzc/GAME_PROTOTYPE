#include "Engine.h"

int Engine::Init(const char* title, int xPos, int yPos, int width, int height, int flags)
{
	cout << "Initializing engine..." << endl;
	if (SDL_Init(SDL_INIT_EVERYTHING) == 0) // If initialization is okay...
	{
		// Create the SDL window...
		cout << "First pass." << endl;
		m_pWindow = SDL_CreateWindow(title, xPos, yPos, width, height, flags);
		if (m_pWindow != nullptr)
		{
			// Create the SDL renderer...(back buffer)
			cout << "Second pass." << endl;
			m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, NULL);
			if (m_pRenderer != nullptr)
			{
				// Initialize subsystems later...
				cout << "Third pass." << endl;
				if (Mix_Init(MIX_INIT_MP3) != 0) // Mixer init success.
				{
					Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 2048); // Good for most games.
					Mix_AllocateChannels(16);
					m_pMusic = Mix_LoadMUS("Aud/game.mp3"); // Load the music track.
					// Load the chunks into the Mix_Chunk vector.
					/*m_vSounds.reserve(3); // Optional but good practice.
					m_vSounds.push_back(Mix_LoadWAV("Aud/enemy.wav"));
					m_vSounds.push_back(Mix_LoadWAV("Aud/laser.wav"));
					m_vSounds.push_back(Mix_LoadWAV("Aud/explode.wav"));
					/* By the way, you should check to see if any of these loads are failing and
					   you can use Mix_GetError() to print out the error message. Wavs can be finicky.*/
				}
				else return false;
			}
			else return false; // Renderer creation failed.
		}
		else return false; // Window creation failed.
	}
	else return false; // initalization failed.
	m_fps = (Uint32)round(1.0 / (double)FPS * 1000); // Converts FPS into milliseconds, e.g. 16.67
	m_keystates = SDL_GetKeyboardState(nullptr);
	m_player.Init(m_pRenderer);
	// m_player = new PlatformPlayer(m_pRenderer);
	cout << "Initialization successful!" << endl;
	Mix_PlayMusic(m_pMusic, -1); // Play. -1 = looping.
	Mix_VolumeMusic(16); // 0-MIX_MAX_VOLUME (128).
	m_running = true;
	return true;
}

void Engine::Wake()
{
	m_start = SDL_GetTicks();
}

void Engine::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT:
			m_running = false;
			break;

		case SDL_KEYDOWN:
			if (event.key.keysym.sym == ' ' && m_player.isGrounded())
			{
				m_player.SetAccelY(-JUMPFORCE);
				m_player.SetGrounded(false);
			}
		}
	}
}

bool Engine::KeyDown(const SDL_Scancode c)
{
	if (m_keystates[c] == 1)
		return true;
	else return false;
}

void Engine::CheckCollision()
{
	for (int i = 0; i < 5; i++)
	{
		if (SDL_HasIntersection(&m_player.GetRect(), &m_Platforms[i]))
		{
			if ((m_player.GetRect().y + m_player.GetRect().h) - (float)m_player.GetVelY() <= m_Platforms[i].y)
			{
				//colliding with the top side of platforms.
				m_player.SetGrounded(true);
				m_player.StopY();
				m_player.SetY(m_Platforms[i].y - m_player.GetRect().h);
			}
			else if (m_player.GetRect().y - (float)m_player.GetVelY() >= (m_Platforms[i].y + m_Platforms[i].h))
			{
				//colliding with the bottom side of platforms.
				m_player.StopY();
				m_player.SetY(m_Platforms[i].y + m_Platforms[i].h);
			}
			else if ((m_player.GetRect().x + m_player.GetRect().w) - (float)m_player.GetVelX() <= m_Platforms[i].x)
			{
				//colliding with the left side of platforms.
				m_player.StopX();
				m_player.SetX(m_Platforms[i].x - m_Platforms[i].w);
			}
			else if (m_player.GetRect().x - (float)m_player.GetVelX() >= (m_Platforms[i].x + m_Platforms[i].w))
			{
				//colliding with the right side of platforms.
				m_player.StopX();
				m_player.SetX(m_Platforms[i].x + m_Platforms[i].w);
			}

		}

	}
}

void Engine::Update()
{
	//move right and left
	if (KeyDown(SDL_SCANCODE_A))
		m_player.SetAccelX(-1.0);
	else if (KeyDown(SDL_SCANCODE_D))
		m_player.SetAccelX(1.0);
	//wrap the player
	if (m_player.GetRect().x < -51.0) m_player.SetX(1024.0);
	else if (m_player.GetRect().x > 1024.0) m_player.SetX(-50.0);
	//Update the player
	m_player.Update();
	CheckCollision();
}

void Engine::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 64, 128, 255, 255);
	SDL_RenderClear(m_pRenderer);
	//Render Platforms
	SDL_SetRenderDrawColor(m_pRenderer, 192, 64, 0, 255);
	for (int i = 0; i < 5; i++)
		SDL_RenderFillRect(m_pRenderer, &m_Platforms[i]);
	m_player.Render();
	SDL_RenderPresent(m_pRenderer); // Flip buffers - send data to window.
	// Any drawing here...


}

void Engine::Sleep()
{
	m_end = SDL_GetTicks();
	m_delta = m_end - m_start; // 1055 - 1050 = 5ms
	if (m_delta < m_fps)
		SDL_Delay(m_fps - m_delta);

}

int Engine::Run()
{
	if (m_running == true)
	{
		return 1;
	}
	// Start and run the "engine"
	if (Init("GAME1007 M1", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, NULL) == false)
	{
		return 2;
	}
	// We passed our initial checks, start the loop!
	while (m_running == true)
	{
		Wake();
		HandleEvents(); // Input
		Update();       // Processing
		Render();       // Output
		if (m_running == true)
			Sleep();
	}
	Clean();
	return 0;
}


void Engine::Clean()
{
	cout << "Cleaning engine..." << endl;
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	for (int i = 0; i < (int)m_vSounds.size(); i++)
		Mix_FreeChunk(m_vSounds[i]);
	m_vSounds.clear();
	Mix_FreeMusic(m_pMusic);
	Mix_CloseAudio();
	Mix_Quit();
	SDL_Quit();
}


