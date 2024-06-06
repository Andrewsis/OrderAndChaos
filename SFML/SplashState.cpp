//#pragma once
//
//#include <sstream>
//#include "SplashState.h"
//#include <iostream>
//#include "DEFINITIONS.h"
//#include "MainMenuState.h"
//
//namespace Engine
//{
//	SplashState::SplashState(GameDataRef data) : _data(data)
//	{
//
//	}
//
//	void SplashState::Init()
//	{
//
//	}
//
//	void SplashState::HandleInput()
//	{
//		sf::Event event;
//
//		while (this->_data->window.pollEvent(event))
//		{
//			if (sf::Event::Closed == event.type)
//			{
//				this->_data->window.close();
//			}
//		}
//	}
//
//	void SplashState::Update(float dt)
//	{
//		if (this->_clock.getElapsedTime().asSeconds() > 0.001)
//		{
//			this->_data->machine.AddState(StateRef(new MainMenuState(_data)), true);
//		}
//	}
//
//	void SplashState::Draw(float dt)
//	{
//	}
//}