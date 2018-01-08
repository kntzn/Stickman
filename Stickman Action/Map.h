#pragma once
#include <SFML/Graphics.hpp>
#include <sstream>
#include "Objects.h"
#include "FileIO.h"
#include "Mission.h" 
#include "Blocks.h"

#define MAP_W 500
#define MAP_H 100

sf::Color mapColors [] =
	{
	sf::Color (10, 10, 10)
	};

class Level
	{
	public:
		sf::Vector2i startPos;
		sf::Vector2i finishPos;

		int TileMap     [MAP_H][MAP_W] = {};
		int PhysicalMap [MAP_H][MAP_W] = {};
		int BlockMap    [MAP_H/5] [MAP_W/5] = {};

		void FillBlock (sf::Vector2i pos, int block)
			{
			for (int x = 0; x < 5; x++)
				for (int y = 0; y < 5; y++)
					{
					TileMap [pos.y*5 + y] [pos.x*5 + x] = Blocks [block] [y] [x];
					}

			BlockMap [pos.y] [pos.x] = block;
			}

		void DrawBlock (sf::RenderWindow &window, sf::Sprite &sprite, sf::Vector2i pos, int block)
			{
			for (int x = 0; x < 5; x++)
				for (int y = 0; y < 5; y++)
					{
					sprite.setTextureRect (sf::IntRect (Blocks [block][y][x]*100-100, 0, 100, 100));
					sprite.setScale (1.f, 1.f);
					sprite.setPosition (x*100.f+pos.x*500.f, y*100.f+pos.y*500.f);

					window.draw (sprite);
					}
			}

		void RefreshPhysicalMap ()
			{
			for (int x = 0; x < MAP_W; x++)
				for (int y = 0; y < MAP_H; y++)
					{
					if (TileMap[y][x] == 0)        PhysicalMap [y][x] = 0;
					else if (TileMap[y][x] == 1)   PhysicalMap [y][x] = 1;
					else if (TileMap [y] [x] < 7)  PhysicalMap [y] [x] = 0;
					else if (TileMap [y] [x] < 9)  PhysicalMap [y] [x] = 1;
					else if (TileMap [y] [x] < 11) PhysicalMap [y] [x] = 0;
					else if (TileMap [y] [x] < 13) PhysicalMap [y] [x] = 1;
					}
			}

		void load (const char filename [])
			{
			char* buf = nullptr;
			size_t bufSize = 0;
			loadFromFile (filename, buf, bufSize, true);

			size_t cursor = 0;
			startPos.x = atoi (buf+cursor);
			cursor += std::to_string (atoi (buf+cursor)).size ()+1;
			startPos.y = atoi (buf+cursor);
			cursor += std::to_string (atoi (buf+cursor)).size ()+1;
			finishPos.x = atoi (buf+cursor);
			cursor += std::to_string (atoi (buf+cursor)).size ()+1;
			finishPos.y = atoi (buf+cursor);
			cursor += std::to_string (atoi (buf+cursor)).size ()+1;

			for (int y = 0; y < MAP_H/5; y++)
				for (int x = 0; x < MAP_W/5 && cursor < bufSize; x++)
					{
					FillBlock (sf::Vector2i (x, y), atoi (buf+cursor));
					cursor += std::to_string (atoi (buf+cursor)).size ()+1;
					}

			free (buf);

			RefreshPhysicalMap ();
			}
	
		void save (const char filename [])
			{
			char* buf;
			size_t cursor = 0, bufSize = MAP_H*MAP_W/25*2;
			buf = (char*) calloc (bufSize*2, sizeof (char));

			std::string s = std::to_string (startPos.x) + " " +
				            std::to_string (startPos.y) + " " +
							std::to_string (finishPos.x) + " " +
							std::to_string (finishPos.y) + " ";

			for (size_t i = 0; i < s.size (); i++)
				*(buf+cursor+i) = s [i];
			cursor += s.size ();

			for (int y = 0; y < MAP_H/5; y++)
				for (int x = 0; x < MAP_W/5; x++)
					{
					std::string s = std::to_string (BlockMap [y] [x]) + " ";
					for (size_t i = 0; i < s.size(); i++)
						*(buf+cursor+i) = s[i];

					cursor += s.size();
					}

			saveToFile (filename, buf, bufSize);

			free (buf);
			}

		Level (unsigned int LVL, int FRACTION)
			{
			std::string filename;
			filename += "Data/map/";
			filename += std::to_string (LVL);
			filename += ".txt";
			
			load (filename.c_str ());
			}

		void Draw (sf::RenderWindow &window, sf::Sprite map, sf::Vector2f center, float factor = 1, bool debug = false)
			{
			for (int x = int (center.x)/100 - int (11.f/factor); x < int (center.x)/100 + int (12.f/factor); x++)
				for (int y = int (center.y)/100 - int (8.f/factor); y < int (center.y)/100 + int (5.f/factor); y++)
					if (0 <= x && x < MAP_W)
						if (0 <= y && y < MAP_H)
							if (TileMap [y] [x] != 0)
								{
								if (factor >= 1)
									{
									map.setTextureRect (sf::IntRect (TileMap [y] [x]*100 - 100, 0, 100, 100));
									map.setScale (1.f, 1.f);
									}
								else if (factor >= 0.5f)
									{
									map.setTextureRect (sf::IntRect (TileMap [y] [x]*50 - 50, 100, 50, 50));
									map.setScale (2.f, 2.f);
									}
								else
									{
									map.setTextureRect (sf::IntRect (TileMap [y] [x]*25 - 25, 150, 25, 25));
									map.setScale (4.f, 4.f);
									}

								map.setPosition (x*100.f, y*100.f);
								
								if (debug)
									{
									if (PhysicalMap [y] [x])
										map.setColor (sf::Color::Green);
									else
										map.setColor (sf::Color::White);
									}

								window.draw (map);
								}
			}
	};

void mapEditor (sf::RenderWindow &window, Level &level, sf::Sprite mapSprite, char filename [])
	{
	sf::Font font;
	font.loadFromFile ("Data/fnt/Bulgaria_Glorious_Cyr.ttf");
	sf::Text text;
	text.setFont (font);
	text.setFillColor (sf::Color (31, 63, 191));

	float tileList = 0;
	int tile = 0;
	const int nTileLists = 2;

	bool lMousePrsd = false, rMousePrsd = false;
	float MouseWheelPos = 0;

	bool windowFocus = true;
	Camera cam (sf::FloatRect (0, 0, float (window.getSize().x), float (window.getSize().y)));

	sf::Clock delayTimer;
	float avgDelay = 0;

	while (window.isOpen () && !sf::Keyboard::isKeyPressed (sf::Keyboard::Escape))
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

		// Mouse coords relative to window
		sf::Vector2i MousePos = sf::Mouse::getPosition (window);
		// Mouse coords relative to map
		sf::Vector2f cursor = window.mapPixelToCoords (MousePos);

 	    // Map scrolling
		if (MousePos.x < 30)								    cam.cam.move (-500/cam.getZoom ()*time, 0);
		if (MousePos.x > signed int (window.getSize ().x - 30)) cam.cam.move ( 500/cam.getZoom ()*time, 0);
		if (MousePos.y < 30)								    cam.cam.move (0, -500/cam.getZoom ()*time);
		if (MousePos.y > signed int (window.getSize ().y - 30)) cam.cam.move (0,  500/cam.getZoom ()*time);

		// Choosing tile
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num1)) tile = 1 + int (tileList)*9;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num2)) tile = 2 + int (tileList)*9;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num3)) tile = 3 + int (tileList)*9;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num4)) tile = 4 + int (tileList)*9;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num5)) tile = 5 + int (tileList)*9;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num6)) tile = 6 + int (tileList)*9;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num7)) tile = 7 + int (tileList)*9;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num8)) tile = 8 + int (tileList)*9;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num9)) tile = 9 + int (tileList)*9;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num0)) tile = 0;

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Comma)  && tileList > 1.f*time)              tileList -= 1.f*time;
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::Period) && tileList < nTileLists - 1.f*time) tileList += 1.f*time;

		// Start & finish position
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::S))
			level.startPos = sf::Vector2i (cursor/500.f);
		if (sf::Keyboard::isKeyPressed (sf::Keyboard::F))
			level.finishPos = sf::Vector2i (cursor/500.f);

		// Drawing
		window.setView (cam.update (window, sf::Mouse::getPosition (window), MouseWheelPos - initMouseWheelPos, time));
		window.clear ();

		mapSprite.setColor (sf::Color::White);
		level.Draw (window, mapSprite, cam.cam.getCenter (), cam.getZoom ());

		if (cursor.x < 0 || cursor.x >= MAP_W*100.f ||
			cursor.y < 0 || cursor.y >= MAP_H*100.f)
			mapSprite.setColor (sf::Color::Red);
		else
			{
			mapSprite.setColor (sf::Color::Green);
			if (lMousePrsd)
				level.FillBlock (sf::Vector2i (cursor/500.f), tile);
			}

		level.DrawBlock (window, mapSprite, sf::Vector2i (cursor/500.f), tile);

		std::ostringstream str;
		str << "Editing: " << filename << "\n\n[" << int (cursor.x/500.f);
		str << "; " << int (cursor.y/500.f) << "]\n\n" << blocksNames [tile];

		text.setString (str.str());
		text.setPosition (cam.cam.getCenter () - sf::Vector2f (window.getSize ())/2.f/cam.getZoom());
		text.setScale (sf::Vector2f (1.f, 1.f)/cam.getZoom());
		
		window.draw (text);
		window.display ();
		}

	level.save (filename);
	level.RefreshPhysicalMap ();
	}