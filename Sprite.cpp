#include <SDL_image.h>
#include "Sprite.h"
#include "System.h"

using namespace std;

namespace engine
{
	Sprite::Sprite(const char* filepath, int xPos, int yPos, int width, int height) : rect{ xPos, yPos, width, height }
	{
		loadTexture(filepath);
	}

	Sprite::Sprite(const char* filepath, int xPos, int yPos) {
		loadTexture(filepath);
		rect = { xPos, yPos, surface->w, surface->h };
	}

	Sprite::~Sprite() {
		SDL_DestroyTexture(texture);
		SDL_FreeSurface(surface);
	}

	void Sprite::loadTexture(const char* filepath)
	{
		surface = IMG_Load(filepath);
		texture = SDL_CreateTextureFromSurface(system.getRenderer(), surface);
	}
}