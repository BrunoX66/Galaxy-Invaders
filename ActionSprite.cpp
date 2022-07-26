#include "ActionSprite.h"

namespace engine
{

	ActionSprite* ActionSprite::getInstance(int xPos, int yPos, int width, int height, const char* filepath, int xMov, int yMov)
	{
		return new ActionSprite(xPos, yPos, width, height, filepath, xMov, yMov);
	}

	ActionSprite* ActionSprite::getInstance(int xPos, int yPos, const char* filepath, int xMov, int yMov)
	{
		return new ActionSprite(xPos, yPos, filepath, xMov, yMov);
	}

	void ActionSprite::tick(GameEngine&)
	{
		if (xMove != 0)
			rect.x += xMove;

		if (yMove != 0)
			rect.y += yMove;
	}

}