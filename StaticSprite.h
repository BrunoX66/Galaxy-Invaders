#ifndef STATICSPRITE_H
#define STATICSPRITE_H

#include "Sprite.h"

namespace engine
{

	class StaticSprite : public Sprite
	{
	public:

		virtual ~StaticSprite() {}

		static StaticSprite* getInstance(const char* filepath, int xPos, int yPos, int width, int height);
		static StaticSprite* getInstance(const char* filepath, int xPos, int yPos);

		virtual void tick(GameEngine&) {}

	protected:

		StaticSprite(const char* filepath, int xPos, int yPos, int width, int height)
			: Sprite(filepath, xPos, yPos, width, height) {}
		StaticSprite(const char* filepath, int xPos, int yPos)
			: Sprite(filepath, xPos, yPos) {}

	};

}

#endif