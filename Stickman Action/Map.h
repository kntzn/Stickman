#pragma once
#include <SFML/Graphics.hpp>
#include "FileIO.h"
#include "Mission.h" 

#define MAP_W 500
#define MAP_H 100

sf::Color mapColors [] =
	{
	sf::Color (10, 10, 10)
	};

int Blocks  [][5][5] = 
	{
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,

	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 1, 1, 1, 1,

	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 0, 0, 0,
	1, 1, 1, 1, 1,

	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	0, 0, 0, 0, 0,
	1, 0, 1, 0, 1,
	1, 1, 1, 1, 1,
	};

class Level
	{
	public:
		int TileMap     [MAP_H][MAP_W] = {};
		int PhysicalMap [MAP_H][MAP_W] = {};

		void FillBlock (sf::Vector2i pos, int block)
			{
			for (int x = 0; x < 5; x++)
				for (int y = 0; y < 5; y++)
					{
					TileMap [pos.y*5 + y] [pos.x*5 + x] = Blocks [block] [y] [x];
					}
			}

		void RefreshPhysicalMap ()
			{
			for (int x = 0; x < MAP_W; x++)
				for (int y = 0; y < MAP_H; y++)
					{
					if (TileMap[y][x] == 0) PhysicalMap [y][x] = 0;
					if (TileMap[y][x] == 1) PhysicalMap [y][x] = 1;
					}
			}

		void load (const char filename [])
			{		
			char* buf = nullptr;
			size_t bufSize = 0;
			loadFromFile (filename, buf, bufSize, true);

			size_t cursor = 0;
			for (int y = 0; y < MAP_H; y++)
				for (int x = 0; x < MAP_W && cursor < bufSize; x++)
					{
					FillBlock (sf::Vector2i (x, y), atoi (buf+cursor));
					cursor += std::to_string (atoi (buf+cursor)).size ()+1;
					}

			free (buf);

			RefreshPhysicalMap ();
			}
	
		Level (unsigned int LVL, int FRACTION)
			{
			std::string filename;
			filename += "Data/map/";
			filename += std::to_string (LVL);
			filename += ".ini";
			
			load (filename.c_str ());
			}

	void Draw (sf::RenderWindow &window, sf::Sprite map, sf::Vector2f center)
		{
		for (int x = int (center.x)/100 - 11; x < int (center.x)/100 + 12; x++)
			for (int y = int (center.y)/100 - 6; y < int (center.y)/100 + 6; y++)
				{
				if (0 <= x && x < MAP_W)
					if (0 <= y && y < MAP_H)
						if (TileMap [y][x] != 0) 
							{
							map.setTextureRect (sf::IntRect (TileMap [y][x]*100 - 100, 0, 100, 100)); 
							map.setPosition (x*100.f, y*100.f);
							window.draw (map); 
							}
				}
		}
	};

void mapEditor (sf::RenderWindow &window, Level level, sf::Sprite mapSprite)
	{
	float zoom = 1; //Множитель скорости

	bool lMousePrsd = false, rMousePrsd = false;
	float MouseWheelPos = 0;

	bool windowFocus = true;
	Camera cam (sf::FloatRect (0, 0, window.getSize().x, window.getSize().y));

	sf::Clock delayTimer;
	float avgDelay = 0;

	while (window.isOpen () && sf::Keyboard::isKeyPressed (sf::Keyboard::Escape))
		{
		//Time Block
		float time = delayTimer.getElapsedTime ().asSeconds ();
		delayTimer.restart ();
		
		// adapting average delay if current delay is not too big
		if (time < avgDelay*10.0f)
			avgDelay += (time - avgDelay)/100.0f;

		//Events
		sf::Event windowEvent;

		float initMouseWheelPos = MouseWheelPos;
		while (window.pollEvent (windowEvent))
			{
			if (windowEvent.type == sf::Event::Closed)
				window.close ();

			if (windowEvent.type == sf::Event::GainedFocus)
				windowFocus = true;
			if (windowEvent.type == sf::Event::LostFocus)
				windowFocus = false;

			if (windowEvent.type == sf::Event::MouseButtonPressed)
				{
				if (windowEvent.key.code == sf::Mouse::Left)
					lMousePrsd = true;
				if (windowEvent.key.code == sf::Mouse::Right)
					rMousePrsd = true;
				}
			if (windowEvent.type == sf::Event::MouseButtonReleased)
				{
				if (windowEvent.key.code == sf::Mouse::Left)
					lMousePrsd = false;
				if (windowEvent.key.code == sf::Mouse::Right)
					rMousePrsd = false;
				}

			if (windowEvent.type == sf::Event::MouseWheelMoved)
				MouseWheelPos += windowEvent.mouseWheel.delta;
			}

		//Координаты мышки
		sf::Vector2i MousePos = sf::Mouse::getPosition (window);   //вектор, содержащий координаты позиции мыши, относительно окна
		sf::Vector2f Pos = window.mapPixelToCoords (MousePos); //вектор, содержащий координаты позиции мыши, относительно карты

 	    //скроллинг карты
		if (MousePos.x < 30)								    cam.cam.move (-1/zoom*time, 0);
		if (MousePos.x > signed int (window.getSize ().x - 30)) cam.cam.move (1/zoom*time, 0);
		if (MousePos.y < 30)								    cam.cam.move (0, -1/zoom*time);
		if (MousePos.y > signed int (window.getSize ().y - 30)) cam.cam.move (0, 1/zoom*time);

		//зумирование карты
		window.setView (cam.update (window, sf::Mouse::getPosition (window), MouseWheelPos - initMouseWheelPos, time));
		level.Draw (window, mapSprite, cam.cam.getCenter ());

		window.display ();
		}
	}