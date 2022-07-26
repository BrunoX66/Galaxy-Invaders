#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <SDL_mixer.h>
#include <vector>
#include <map>
#include <string>
#include "Sprite.h"
#include "StaticSprite.h"
#include "ActionSprite.h"

namespace engine
{

	class GameEngine
	{
	public:

		GameEngine() : loopCounter(0) {}
		~GameEngine();

		void add(Sprite* sprite);
		void remove(Sprite* sprite);
		void start(const Uint16& FPS = 30, const Uint16& generatorInterval = 1000);
		void addGeneratorFunc(void (*func)());
		bool installSound(const char* filepath);
		bool playSound(const std::string& filepath) const;
		const std::vector<Sprite*>& getSprites() const { return sprites; }

		template<typename T>
		T* collisionDetection(Sprite* sprite);

	private:

		Uint64 loopCounter;
		std::vector<Sprite*> sprites;
		std::vector<Sprite*> added;
		std::vector<Sprite*> removed;
		std::vector<void (*)()> generators;

		std::map<const std::string, Mix_Chunk*> sounds;
		Mix_Chunk* searchSound(const std::string& filepath) const;
		void freeAudioChunk();

		template<typename T>
		void deleteVecPointers(std::vector<T>& vec);

		void generatorHandler(const Uint16& intervalValue);
		void updateSprites();
		void checkCollision();

		void addNewSprites();
		void removeSprites();

		void updateScreen() const;
		void drawSprites() const;

		void fpsDelayer(const Uint32& nextTick) const;
		void addLoopCount();

		GameEngine(const GameEngine&) = delete;
		GameEngine& operator= (const GameEngine&) = delete;

	};

	template<typename T>
	void GameEngine::deleteVecPointers(std::vector<T>& vec)
	{
		for (auto pointer : vec)
		{
			delete pointer;
		}
		vec.clear();
	}

	template<typename T>
	T* GameEngine::collisionDetection(Sprite* sprite)
	{
		const SDL_Rect rect = sprite->getRect();
		for (Sprite* otherSprite : sprites)
		{
			if (sprite != otherSprite)
			{
				if (T* other = dynamic_cast<T*>(otherSprite))
				{
					const SDL_Rect otherRect = other->getRect();
					if (SDL_HasIntersection(&rect, &otherRect))
						return other;
				}
			}
		}
		return nullptr;
	}
}

#endif