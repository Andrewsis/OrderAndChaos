#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"

namespace Engine
{
	class MainMenuState : public State
	{
	public:
		MainMenuState(GameDataRef data);

		void Init();
		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		GameDataRef _data;

		sf::Sprite _background;
		sf::Sprite _playButton;
		sf::Sprite _playButtonOuter;
		sf::Sprite _title;
	};
}