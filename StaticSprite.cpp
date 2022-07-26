#include "StaticSprite.h"

namespace engine
{

	StaticSprite* StaticSprite::getInstance(const char* filepath, int xPos, int yPos, int width, int height)
	{
		return new StaticSprite(filepath, xPos, yPos, width, height);
	}
	
	StaticSprite* StaticSprite::getInstance(const char* filepath, int xPos, int yPos)
	{
		return new StaticSprite(filepath, xPos, yPos);
	}

}