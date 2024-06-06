#pragma once
#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"

namespace Engine
{
	class GameState : public State
	{
	public:
		GameState(GameDataRef data);

		void Init();
		void HandleInput();
		void Update(float dt);
		void Draw(float dt);

	private:
		void InitGridPieces();

		void CheckAndPlacePiece(bool mouseButton);

		void CheckPlayerHasWon(int turn);
		void Check5PiecesForMatch(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int pieceToCheck);

		GameDataRef _data;

		sf::Sprite _background;
		sf::Sprite _pauseButton;
		sf::Sprite _gridSprite;

		sf::Sprite _gridPieces[6][6];
		int gridArray[6][6];

		int turn;
		int gameState;

		sf::Clock _clock;
	};
}