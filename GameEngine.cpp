#include <SDL.h>
#include <stdexcept>
#include <iostream>
#include "System.h"
#include "GameEngine.h"

using namespace std;

namespace engine
{
	GameEngine::~GameEngine()
	{
		generators.clear();
		deleteVecPointers(removed);
		deleteVecPointers(added);
		deleteVecPointers(sprites);
		freeAudioChunk();
	}

	void GameEngine::add(Sprite* sprite)
	{
		added.push_back(sprite);
	}

	void GameEngine::remove(Sprite* sprite)
	{
		removed.push_back(sprite);
	}

	void GameEngine::start(const Uint16& FPS, const Uint16& generatorInterval)
	{
		bool run = true;

		const Uint32 TICK_INTERVAL = 1000 / FPS;

		while (run)
		{
			Uint32 nextTick = SDL_GetTicks() + TICK_INTERVAL;

			SDL_Event event;

			while (SDL_PollEvent(&event))
			{
				switch (event.type)
				{
				case SDL_KEYDOWN:
					for (Sprite* sprite : sprites)
						sprite->keyDown(event);
					break;
				case SDL_KEYUP:
					for (Sprite* sprite : sprites)
						sprite->keyUp(event);
					break;
				case SDL_MOUSEBUTTONDOWN:
					for (Sprite* sprite : sprites)
						sprite->mouseDown(event);
					break;
				case SDL_MOUSEBUTTONUP:
					for (Sprite* sprite : sprites)
						sprite->mouseUp(event);
					break;
				case SDL_MOUSEMOTION:
					for (Sprite* sprite : sprites)
						sprite->mouseMotion(event);
					break;
				case SDL_QUIT:
					run = false; break;
				}
			}

			generatorHandler(generatorInterval);
			updateSprites();
			checkCollision();
			addNewSprites();
			removeSprites();
			updateScreen();
			fpsDelayer(nextTick);
			addLoopCount();
		}
	}

	void GameEngine::generatorHandler(const Uint16& intervalValue)
	{
		if (loopCounter % intervalValue == 0)
			for (auto generator : generators)
				generator();
	}

	void GameEngine::updateSprites()
	{
		for (Sprite* sprite : sprites)
		{
			sprite->tick(*this);
		}
	}

	void GameEngine::checkCollision()
	{
		for (Sprite* sprite : sprites)
		{
			sprite->collisionHandler(*this);
		}
	}

	void GameEngine::addNewSprites()
	{
		for (Sprite* sprite : added)
			sprites.push_back(sprite);
		added.clear();
	}

	void GameEngine::removeSprites()
	{
		for (Sprite* sprite : removed)
		{
			for (vector<Sprite*>::iterator iter = sprites.begin(); iter != sprites.end();)
			{
				if (*iter == sprite)
				{
					delete sprite;
					iter = sprites.erase(iter);
					break;
				}
				else
					++iter;
			}
		}

		removed.clear();
	}

	void GameEngine::updateScreen() const
	{
		SDL_SetRenderDrawColor(system.getRenderer(), 0, 0, 0, 0);
		SDL_RenderClear(system.getRenderer());
		drawSprites();
		SDL_RenderPresent(system.getRenderer());
	}

	void GameEngine::drawSprites() const
	{
		for (Sprite* sprite : sprites)
		{
			sprite->draw();
		}
	}

	void GameEngine::fpsDelayer(const Uint32& nextTick) const
	{
		int delay = nextTick - SDL_GetTicks();
		if (delay > 0)
			SDL_Delay(delay);
	}

	void GameEngine::addGeneratorFunc(void (*func)())
	{
		generators.push_back(func);
	}

	void GameEngine::addLoopCount()
	{
		++loopCounter;
	}

	bool GameEngine::installSound(const char* filepath)
	{
		try
		{
			Mix_Chunk* audio = Mix_LoadWAV(filepath);

			if (!audio)
				throw runtime_error(string("Audio file failed to load: ") + Mix_GetError());

			if (sounds.count(string(filepath)) == 0)
				sounds.insert(make_pair(string(filepath), audio));

			return true;
		}
		catch (std::runtime_error& e)
		{
			std::cerr << e.what() << std::endl;
			return false;
		}
	}

	bool GameEngine::playSound(const string& filepath) const
	{
		if (Mix_Chunk* sound = searchSound(filepath))
		{
			try
			{
				Mix_HaltChannel(-1);

				int channel = Mix_PlayChannel(-1, sound, 0);

				if (channel == -1)
					throw runtime_error(string("Audio file failed to play: ") + Mix_GetError());

				return true;
			}
			catch (std::runtime_error& e)
			{
				std::cerr << e.what() << std::endl;
				return false;
			}
		}
		else
			return false;
	}

	Mix_Chunk* GameEngine::searchSound(const string& filepath) const
	{
		for (const pair<const string, Mix_Chunk*>& p : sounds)
		{
			if (p.first == filepath)
				return p.second;
		}

		return nullptr;
	}

	void GameEngine::freeAudioChunk()
	{
		for (const pair<const string, Mix_Chunk*>& p : sounds)
		{
			Mix_FreeChunk(p.second);
		}
		sounds.clear();
	}
}