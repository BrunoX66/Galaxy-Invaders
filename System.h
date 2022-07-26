#ifndef SYSTEM_H
#define SYSTEM_H

#include <SDL.h>

namespace engine 
{
	class System
	{
	public:

		System();
		~System();

		SDL_Renderer* getRenderer() const;

	private:

		SDL_Window* window;
		SDL_Renderer* renderer;

		System(const System&) = delete;
		System& operator= (const System&) = delete;

	};

	extern System system;
}
#endif