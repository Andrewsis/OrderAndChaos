#pragma once

#include <sstream>
#include "GameState.h"
#include "MainMenuState.h"
#include "DEFINITIONS.h"
#include "PauseState.h"
#include "GameOverState.h"

#include <iostream>

namespace Engine
{
	GameState::GameState(GameDataRef data) : _data(data)
	{

	}

	void GameState::Init()
	{
		gameState = STATE_PLAYING;
		turn = ORDER;

		this->_data->assets.LoadTexture("Pause Button", PAUSE_BUTTON);
		this->_data->assets.LoadTexture("Grid Sprite", GRID_SPRITE_FILEPATH);
		this->_data->assets.LoadTexture("X Piece", X_PIECE_FILEPATH);
		this->_data->assets.LoadTexture("O Piece", O_PIECE_FILEPATH);
		this->_data->assets.LoadTexture("X Winning Piece", X_WINNING_PIECE_FILEPATH);
		this->_data->assets.LoadTexture("O Winning Piece", O_WINNING_PIECE_FILEPATH);

		_background.setTexture(this->_data->assets.GetTexture("Background"));
		_pauseButton.setTexture(this->_data->assets.GetTexture("Pause Button"));
		_gridSprite.setTexture(this->_data->assets.GetTexture("Grid Sprite")); 

		_gridSprite.scale(1.5, 1.5);

		_pauseButton.setPosition(this->_data->window.getSize().x - _pauseButton.getLocalBounds().width, _pauseButton.getPosition().y);
		_gridSprite.setPosition((SCREEN_WIDTH/2) - (_gridSprite.getGlobalBounds().width / 2),
			(SCREEN_HEIGHT / 2) - (_gridSprite.getGlobalBounds().height / 2)); 

		InitGridPieces();

		for (int x = 0; x < 6; x++)
		{
			for (int y = 0; y < 6; y++)
			{
				gridArray[x][y] = EMPTY_PIECE;
			}
		}
	}

	void GameState::HandleInput()
	{
		sf::Event event;

		while (this->_data->window.pollEvent(event))
		{
			if (sf::Event::Closed == event.type)
			{
				this->_data->window.close();
			}

			if (this->_data->input.IsSpriteClicked(this->_pauseButton, sf::Mouse::Left, this->_data->window))
			{
				this->_data->machine.AddState(StateRef(new PauseState(_data)), false);
			}
			else if (this->_data->input.IsSpriteClicked(this->_gridSprite, sf::Mouse::Left, this->_data->window))
			{
				if (STATE_PLAYING == gameState)
				{
					bool mouseButton = true;
					this->CheckAndPlacePiece(mouseButton);
				}
			}
			else if (this->_data->input.IsSpriteClicked(this->_gridSprite, sf::Mouse::Right, this->_data->window))
			{
				if (STATE_PLAYING == gameState)
				{
					bool mouseButton = false;
					this->CheckAndPlacePiece(mouseButton);
				}
			}
		}
	}

	void GameState::Update(float dt)
	{
		if (STATE_DRAW == gameState || STATE_LOSE == gameState || STATE_WON == gameState)
		{
			if (this->_clock.getElapsedTime().asSeconds() > TIME_BEFORE_SHOWING_GAME_OVER)
			{
				this->_data->machine.AddState(StateRef(new GameOverState(_data)), true);
			}
		}
	}

	void GameState::Draw(float dt)
	{
		this->_data->window.clear(sf::Color::Red);

		this->_data->window.draw(this->_background);
		this->_data->window.draw(this->_pauseButton);
		this->_data->window.draw(this->_gridSprite);

		for (int x = 0; x < 6; x++)
		{
			for (int y = 0; y < 6; y++)
			{
				this->_data->window.draw(this->_gridPieces[x][y]);
			}
		}
		this->_data->window.display();
	}

	void GameState::InitGridPieces()
	{
		sf::Vector2u tempSpriteSize = this->_data->assets.GetTexture("X Piece").getSize();

		for (int x = 0; x < 6; x++)
		{
			for (int y = 0; y < 6; y++)
			{
				_gridPieces[x][y].scale(0.75, 0.75);
				_gridPieces[x][y].setPosition((127 * x) + 7, (125 * y) + 198);
			}
		}
	}

	void GameState::CheckAndPlacePiece(bool mouseButton)
	{
		sf::Vector2i touchPoint = this->_data->input.GetMousePosition(this->_data->window);
		sf::FloatRect gridSize = _gridSprite.getGlobalBounds();
		sf::Vector2f gapOutsideOfGrid = sf::Vector2f((SCREEN_WIDTH - gridSize.width) / 2,
			(SCREEN_HEIGHT - gridSize.height) / 2);

		sf::Vector2f gridLocalTouchPos = sf::Vector2f(touchPoint.x - gapOutsideOfGrid.x, touchPoint.y - gapOutsideOfGrid.y);

		sf::Vector2f gridSectionSize = sf::Vector2f(gridSize.width / 6, gridSize.height / 6);

		int column, row;

		if (gridLocalTouchPos.x < gridSectionSize.x)
		{
			column = 1;
		}
		else if (gridLocalTouchPos.x < gridSectionSize.x * 2)
		{
			column = 2;
		}
		else if (gridLocalTouchPos.x < gridSectionSize.x * 3)
		{
			column = 3;
		}
		else if (gridLocalTouchPos.x < gridSectionSize.x * 4)
		{
			column = 4;
		}
		else if (gridLocalTouchPos.x < gridSectionSize.x * 5)
		{
			column = 5;
		}
		else if (gridLocalTouchPos.x < gridSize.width)
		{
			column = 6;
		}


		if (gridLocalTouchPos.y < gridSectionSize.y)
		{
			row = 1;
		}
		else if (gridLocalTouchPos.y < gridSectionSize.y * 2)
		{
			row = 2;
		}
		else if (gridLocalTouchPos.y < gridSectionSize.y * 3)
		{
			row = 3;
		}
		else if (gridLocalTouchPos.y < gridSectionSize.y * 4)
		{
			row = 4;
		}
		else if (gridLocalTouchPos.y < gridSectionSize.y * 5)
		{
			row = 5;
		}
		else if (gridLocalTouchPos.y < gridSize.height)
		{
			row = 6;
		}


		if (gridArray[column - 1][row - 1] == EMPTY_PIECE)	
		{
			if (CHAOS == turn)
			{
				if (mouseButton)
				{
					gridArray[column - 1][row - 1] = X_BLUE_PIECE;

					_gridPieces[column - 1][row - 1].setTexture(this->_data->assets.GetTexture("X Piece"));
					_gridPieces[column - 1][row - 1].setColor(sf::Color(242, 109, 125, 255));

					this->CheckPlayerHasWon(X_BLUE_PIECE);
				}
				else
				{
					gridArray[column - 1][row - 1] = O_BLUE_PIECE;
					_gridPieces[column - 1][row - 1].setTexture(this->_data->assets.GetTexture("O Piece"));
					_gridPieces[column - 1][row - 1].setColor(sf::Color(242, 109, 125, 255));

					this->CheckPlayerHasWon(O_BLUE_PIECE);
				}
				turn = ORDER;
			}
			else if (ORDER == turn)
			{
				if (mouseButton)
				{
					gridArray[column - 1][row - 1] = X_RED_PIECE;
					_gridPieces[column - 1][row - 1].setTexture(this->_data->assets.GetTexture("X Piece"));
					_gridPieces[column - 1][row - 1].setColor(sf::Color(28, 187, 180, 255));
					this->CheckPlayerHasWon(X_RED_PIECE);
				}
				else
				{
					gridArray[column - 1][row - 1] = O_RED_PIECE;
					_gridPieces[column - 1][row - 1].setTexture(this->_data->assets.GetTexture("O Piece"));
					_gridPieces[column - 1][row - 1].setColor(sf::Color(28, 187, 180, 255));
					this->CheckPlayerHasWon(O_RED_PIECE);
				}
				turn = CHAOS;
			}
		}
	}

	void GameState::CheckPlayerHasWon(int player)
	{
		for (int i = 0; i < 6; ++i)
		{
			Check5PiecesForMatch(0, i, 1, i, 2, i, 3, i, 4, i, player);
			Check5PiecesForMatch(1, i, 2, i, 3, i, 4, i, 5, i, player);
		}
		for (int i = 0; i < 6; i++)
		{
			Check5PiecesForMatch(i, 0, i, 1, i, 2, i, 3, i, 4, player);
			Check5PiecesForMatch(i, 1, i, 2, i, 3, i, 4, i, 5, player);
		}

		Check5PiecesForMatch(0, 5, 1, 4, 2, 3, 3, 2, 4, 1, player); // Global left to right diagonal
		Check5PiecesForMatch(1, 4, 2, 3, 3, 2, 4, 1, 5, 0, player);

		Check5PiecesForMatch(0, 0, 1, 1, 2, 2, 3, 3, 4, 4, player); // Global right to left diagonal
		Check5PiecesForMatch(1, 1, 2, 2, 3, 3, 4, 4, 5, 5, player);

		Check5PiecesForMatch(1, 0, 2, 1, 3, 2, 4, 3, 5, 4, player);
		Check5PiecesForMatch(0, 1, 1, 2, 2, 3, 3, 4, 4, 5, player);

		Check5PiecesForMatch(0, 4, 1, 3, 2, 2, 3, 1, 4, 0, player);
		Check5PiecesForMatch(1, 5, 2, 4, 3, 3, 4, 2, 5, 1, player);

		int emptyNum = 36;

		for (int x = 0; x < 6; x++)
		{
			for (int y = 0; y < 6; y++)
			{
				if (EMPTY_PIECE != gridArray[x][y])
				{
					emptyNum--;
				}
			}
		}

		if (0 == emptyNum && (STATE_WON != gameState) && (STATE_LOSE != gameState))
		{
			gameState = STATE_DRAW;
		}

		if (STATE_DRAW == gameState || STATE_LOSE == gameState || STATE_WON == gameState)
		{
			this->_clock.restart();
		}
	}

	void GameState::Check5PiecesForMatch(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int x5, int y5, int pieceToCheck)
	{
		if ((pieceToCheck == O_BLUE_PIECE) || (pieceToCheck == O_RED_PIECE))
		{
			if (((gridArray[x1][y1] == O_BLUE_PIECE) || (gridArray[x1][y1] == O_RED_PIECE)) &&
				((gridArray[x2][y2] == O_BLUE_PIECE) || (gridArray[x2][y2] == O_RED_PIECE)) &&
				((gridArray[x3][y3] == O_BLUE_PIECE) || (gridArray[x3][y3] == O_RED_PIECE)) &&
				((gridArray[x4][y4] == O_BLUE_PIECE) || (gridArray[x4][y4] == O_RED_PIECE)) &&
				((gridArray[x5][y5] == O_BLUE_PIECE) || (gridArray[x5][y5] == O_RED_PIECE)))
			{
				_gridPieces[x1][y1].setTexture(this->_data->assets.GetTexture("O Winning Piece"));
				_gridPieces[x1][y1].setColor(sf::Color(255, 255, 255, 255));

				_gridPieces[x2][y2].setTexture(this->_data->assets.GetTexture("O Winning Piece"));
				_gridPieces[x2][y2].setColor(sf::Color(255, 255, 255, 255));

				_gridPieces[x3][y3].setTexture(this->_data->assets.GetTexture("O Winning Piece"));
				_gridPieces[x3][y3].setColor(sf::Color(255, 255, 255, 255));

				_gridPieces[x4][y4].setTexture(this->_data->assets.GetTexture("O Winning Piece"));
				_gridPieces[x4][y4].setColor(sf::Color(255, 255, 255, 255));

				_gridPieces[x5][y5].setTexture(this->_data->assets.GetTexture("O Winning Piece"));
				_gridPieces[x5][y5].setColor(sf::Color(255, 255, 255, 255));

				gameState = STATE_WON;
			}
		}
		else if ((pieceToCheck == X_BLUE_PIECE) || (pieceToCheck == X_RED_PIECE))
		{
			if (((gridArray[x1][y1] == X_BLUE_PIECE) || (gridArray[x1][y1] == X_RED_PIECE)) &&
				((gridArray[x2][y2] == X_BLUE_PIECE) || (gridArray[x2][y2] == X_RED_PIECE)) &&
				((gridArray[x3][y3] == X_BLUE_PIECE) || (gridArray[x3][y3] == X_RED_PIECE)) &&
				((gridArray[x4][y4] == X_BLUE_PIECE) || (gridArray[x4][y4] == X_RED_PIECE)) &&
				((gridArray[x5][y5] == X_BLUE_PIECE) || (gridArray[x5][y5] == X_RED_PIECE)))
			{
				_gridPieces[x1][y1].setTexture(this->_data->assets.GetTexture("X Winning Piece"));
				_gridPieces[x1][y1].setColor(sf::Color(255, 255, 255, 255));

				_gridPieces[x2][y2].setTexture(this->_data->assets.GetTexture("X Winning Piece"));
				_gridPieces[x2][y2].setColor(sf::Color(255, 255, 255, 255));

				_gridPieces[x3][y3].setTexture(this->_data->assets.GetTexture("X Winning Piece"));
				_gridPieces[x3][y3].setColor(sf::Color(255, 255, 255, 255));

				_gridPieces[x4][y4].setTexture(this->_data->assets.GetTexture("X Winning Piece"));
				_gridPieces[x4][y4].setColor(sf::Color(255, 255, 255, 255));

				_gridPieces[x5][y5].setTexture(this->_data->assets.GetTexture("X Winning Piece"));
				_gridPieces[x5][y5].setColor(sf::Color(255, 255, 255, 255));

				gameState = STATE_WON;
			}
		}
	}
}