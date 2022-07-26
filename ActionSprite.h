#ifndef ACTIONSPRITE_H
#define ACTIONSPRITE_H

#include "Sprite.h"

namespace engine
{

	class ActionSprite : public Sprite
	{
	public:

		virtual ~ActionSprite() {}

		static ActionSprite* getInstance(int xPos, int yPos, int width, int height, const char* filepath, int xMov = 0, int yMov = 0);
		static ActionSprite* getInstance(int xPos, int yPos, const char* filepath, int xMov = 0, int yMov = 0);

		virtual void tick(GameEngine&);

	protected:

		ActionSprite(int xPos, int yPos, int width, int height, const char* filepath, int xMov = 0, int yMov = 0)
			: xMove(xMov), yMove(yMov), Sprite(filepath, xPos, yPos, width, height) {}
		ActionSprite(int xPos, int yPos, const char* filepath, int xMov = 0, int yMov = 0)
			: xMove(xMov), yMove(yMov), Sprite(filepath, xPos, yPos) {}

	private:

		int xMove;
		int yMove;

	};

}

#endif