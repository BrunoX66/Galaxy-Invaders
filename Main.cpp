#include <iostream>
#include "GameEngine.h"

using namespace engine;

GameEngine gameEngine;

class Projectile : public ActionSprite
{
public:

	static Projectile* getInstance(int xPos, const char* filepath)
	{
		return new Projectile(xPos, filepath);
	}

	void fireSound()
	{
		gameEngine.playSound("Assets/Sound/Fire.wav");
	}

protected:

	Projectile(int xPos, const char* filepath)
		: ActionSprite(xPos, 583, filepath) {}

private:

	void tick(GameEngine& engine)
	{
		if (rect.y < 0)
			engine.remove(this);
		else
			rect.y -= 2;
	}

	void collisionHandler(GameEngine& engine);
};

class Player : public ActionSprite
{
public:

	static Player* getInstance(int xPos, int yPos, int width, int height, const char* filepath)
	{
		return new Player(xPos, yPos, width, height, filepath);
	}
	
	static Player* getInstance(int xPos, int yPos, const char* filepath)
	{
		return new Player(xPos, yPos, filepath);
	}

	void kill()
	{
		playerDeathSound();
		dead = true;
		gameEngine.remove(this);
	}

	const bool& isDead() const { return dead; }

protected:

	Player(int xPos, int yPos, int width, int height, const char* filepath)
		: ActionSprite(xPos, yPos, width, height, filepath) {}
	Player(int xPos, int yPos, const char* filepath)
		: ActionSprite(xPos, yPos, filepath) {}

private:

	bool dead = false;
	const int MOVEMENT_VALUE = 5;
	int xPos = rect.x + (rect.w / 2) - 18;

	void keyDown(const SDL_Event& event)
	{
		if (event.key.keysym.sym == SDLK_SPACE)
		{
			Projectile* p = Projectile::getInstance(xPos, "Assets/Sprite/missile.png");
			gameEngine.add(p);
			p->fireSound();
		}
		else if (event.key.keysym.sym == SDLK_LEFT)
		{
			if (rect.x <= 0)
				rect.x = 0;
			else
				rect.x -= MOVEMENT_VALUE;
		}
		else if (event.key.keysym.sym == SDLK_RIGHT)
		{
			if (rect.x >= 898)
				rect.x = 898;
			else
				rect.x += MOVEMENT_VALUE;
		}
	}

	void tick(GameEngine& engine)
	{
		xPos = rect.x + (rect.w / 2) - 18;
	}

	void collisionHandler(GameEngine& engine);

	void playerDeathSound()
	{
		gameEngine.playSound("Assets/Sound/Dead.wav");
		SDL_Delay(1000);
	}
};

class Enemy : public ActionSprite
{
public:

	static Enemy* getInstance(Uint32 speed, int xPos, const char* filepath)
	{
		return new Enemy(speed, xPos, filepath);
	}

	void explosionSound()
	{
		gameEngine.playSound("Assets/Sound/Explosion.wav");
	}

protected:

	Enemy(Uint32 speed, int xPos, const char* filepath)
		: SPEED(speed), ActionSprite(xPos, -100, filepath) {}

private:

	const Uint32 SPEED;
	int counter = 0;

	void tick(GameEngine& engine)
	{
		counter++;
		if (rect.y > 637)
		{
			engine.remove(this);

			for (Sprite* sprite : engine.getSprites())
			{
				if (Player* p = dynamic_cast<Player*>(sprite))
				{
					if (!(p->isDead()))
					{
						p->kill();
						break;
					}
				}
			}
		}
		else if (counter % 10 == 0)
			rect.y += SPEED;
	}
};

void generateEnemies()
{
	int count;
	int speed = 2;
	int xPos = 0;

	for (count = 0; count != 10; ++count)
	{
		Enemy* enemy = Enemy::getInstance(speed, xPos, "Assets/Sprite/enemy.png");
		gameEngine.add(enemy);
		xPos += (enemy->getRect().w + 4);
	}
}

// For Projectile class
void Projectile::collisionHandler(GameEngine& engine)
{
	Enemy* enemy = engine.collisionDetection<Enemy>(this);
	if (enemy)
	{
		enemy->explosionSound();
		engine.remove(this);
		engine.remove(enemy);
	}
}

// For Player class
void Player::collisionHandler(GameEngine& engine)
{
	Enemy* enemy = engine.collisionDetection<Enemy>(this);
	if (enemy)
		kill();
}

void installGameSounds()
{
	bool installed;

	installed = gameEngine.installSound("Assets/Sound/Dead.wav");
	if (!installed)
		std::cerr << "Loading of file Dead.wav failed." << std::endl;

	installed = gameEngine.installSound("Assets/Sound/Explosion.wav");
	if (!installed)
		std::cerr << "Loading of file Explosion.wav failed." << std::endl;

	installed = gameEngine.installSound("Assets/Sound/Fire.wav");
	if (!installed)
		std::cerr << "Loading of file Fire.wav failed." << std::endl;
}

int main(int argc, char** argv)
{
	Player* player = Player::getInstance(449, 580, "Assets/Sprite/player.png");
	gameEngine.add(player);
	installGameSounds();
	gameEngine.addGeneratorFunc(generateEnemies);
	gameEngine.start(60);

	return 0;
}

