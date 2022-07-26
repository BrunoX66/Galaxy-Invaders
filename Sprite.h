#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>
#include "System.h"

namespace engine
{
	class GameEngine;

	class Sprite
	{
	public:

		virtual ~Sprite();

		// SDL Events
		virtual void keyDown(const SDL_Event&) {}
		virtual void keyUp(const SDL_Event&) {}
		virtual void mouseDown(const SDL_Event&) {}
		virtual void mouseUp(const SDL_Event&) {}
		virtual void mouseMotion(const SDL_Event&) {}

		// Publika medlemsfunktioner
		virtual void tick(GameEngine&) = 0;
		virtual void draw() const { SDL_RenderCopy(system.getRenderer(), texture, NULL, &rect); }
		virtual void collisionHandler(GameEngine&) {}

		const SDL_Rect& getRect() const { return rect; }
		SDL_Surface* getSurface() const { return surface; }
		SDL_Texture* getTexture() const { return texture; }

	protected:

		SDL_Rect rect;

		Sprite(const char* filepath, int xPos, int yPos, int width, int height);
		Sprite(const char* filepath, int xPos, int yPos);
		
	private:

		SDL_Surface* surface;
		SDL_Texture* texture;
		void loadTexture(const char* filepath);

		Sprite(const Sprite&) = delete;
		Sprite& operator= (const Sprite&) = delete;

	};

}

#endif