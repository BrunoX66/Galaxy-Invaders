#include <stdexcept>
#include <string>
#include <SDL_mixer.h>
#include "System.h"

using namespace std;

namespace engine
{

	System::System()
	{
		const int WIDTH = 1000;
		const int HEIGHT = 700;

		int initCode = SDL_Init(SDL_INIT_EVERYTHING);
		if (initCode != 0)
			throw runtime_error(string("Initialization of SDL failed: ") + SDL_GetError());

		int creationCode = SDL_CreateWindowAndRenderer(WIDTH, HEIGHT, 0, &window, &renderer);
		if (creationCode == -1)
			throw runtime_error(string("Creation of Window or Renderer failed: ") + SDL_GetError());

		int audioInitCode = Mix_OpenAudio(
			MIX_DEFAULT_FREQUENCY,
			MIX_DEFAULT_FORMAT,
			MIX_DEFAULT_CHANNELS,
			1024
		);
		if (audioInitCode == -1)
			throw runtime_error(string("Initialization of SDL Mixer failed: ") + Mix_GetError());

		Mix_Volume(-1, 64);
	}

	System::~System()
	{
		Mix_CloseAudio();
		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

	SDL_Renderer* System::getRenderer() const
	{
		return renderer;
	}

	System system; // Statiskt objekt
}