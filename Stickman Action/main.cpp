#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include <iostream>
#include <list>

#include "Display.h"
#include "Draw.h"
#include "Objects.h"

void setup ();
void loop  (WindowParameters wp);

void SinglePlayer (sf::RenderWindow &window);
short int MainMenu (sf::RenderWindow &window);

int main ()
	{
	loop (LoadWindowParameters ());

	return 0;
	}

void setup ()
	{
	
	}
void loop  (WindowParameters wp)
	{
	sf::RenderWindow window (sf::VideoMode (wp.vm.width, wp.vm.height), "", wp.Mode, wp.cs);
	//window.setFramerateLimit (wp.fpsLim);
	window.setFramerateLimit (70);

	short int mode = 1;
	while (mode != 0)
		{
		SinglePlayer (window);
		}
	}

void SinglePlayer (sf::RenderWindow &window)
	{
	//---------Loading img---------//
	sf::Image stickman = LoadImage ("stickman.png");
	
	// Map guns
	sf::Image guns; guns.loadFromFile ("Data/img/guns.png");
	guns.createMaskFromColor (sf::Color (0, 0, 255));
	initGuns (guns);

	// Trees
	sf::Image door; 
	door.loadFromFile ("Data/img/mapObjects.png");
	door.createMaskFromColor (sf::Color (0, 0, 255));

	// Map sprites
	sf::Image map_img; map_img.loadFromFile ("Data/img/tileset.png");
	map_img.createMaskFromColor (sf::Color (0, 0, 255));
	sf::Texture tx; tx.loadFromImage (map_img);
	sf::Sprite map_sprite; map_sprite.setTexture (tx);

	// Background
	sf::Texture backgr;
	backgr.loadFromFile ("Data/img/BGR.png");
	sf::Sprite background;
	background.setTexture (backgr);

	//--------Creating level--------//
	Level level (0, 0);
	mapEditor (window, level, map_sprite, "Data/map/0.txt");

	//---------Objects list---------//
	float snow [100] = {};
	float snowSpeed [100] = {};
	for (int i = 0; i < 100; i++)
		snowSpeed [i] = (rand ()%100+50)*5.f;

	std::vector <Stickman*> stickmans;
	Bullet bullets [1000];
	int nBullets = 0;
	std::vector <Object*> mapObjects;

	stickmans.push_back (new Player (stickman, guns, sf::Vector2f (900, 800), 80));
	stickmans.push_back (new NPC (stickman, guns, sf::Vector2f (1100, 800), 80, objectType::solder, 0, 1, 0));
	mapObjects.push_back (new Door (door, sf::Vector2f (850, 500), 10));

	Camera camera (sf::FloatRect (0, 0, float (window.getSize().x), float (window.getSize().y)));
	
	//---------Variables---------//
	// Timers
	sf::Clock delayTimer;
	unsigned long int tickTimer = 0;
	float avgDelay = 0;
	sf::Vector2f thisPlayerPos;

	bool lMousePrsd = false, rMousePrsd = false;
	bool windowFocus = true;

	// speedtests variables
	clock_t physics = 0, graphics = 0, end = 0;

	while (window.isOpen ())
		{
		//Time Block
		float time = delayTimer.getElapsedTime ().asSeconds ();
		delayTimer.restart ();
		// Debug output
		//std::cout << "FPS: " << 1.f/time << std::endl;
		//std::cout << "Graphics: " << end - graphics << std::endl;
		//std::cout << "Physics: " << graphics - physics << std::endl << std::endl;

		if (tickTimer = 0) avgDelay = time;
		
		// adapt average delay if current delay is not too big
		if (time < avgDelay*10.0f)
			avgDelay += (time - avgDelay)/100.0f;

		//Events
		sf::Event windowEvent;
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

			}

		if (sf::Keyboard::isKeyPressed (sf::Keyboard::F2))
			{
			sf::Vector2u windowSize = window.getSize();
			sf::Texture texture;
			texture.create(windowSize.x, windowSize.y);
			texture.update(window);
			sf::Image screenshot = texture.copyToImage();

			std::string filename;
			filename += "Screenshots/"; 
			filename += std::to_string (tickTimer);
			filename += ".png";
			screenshot.saveToFile (filename.c_str ());
			}

		// Physics
		physics = clock ();
		if (windowFocus)
			{
			// Stickmans
			for (auto i = stickmans.begin (); i != stickmans.end ();)
				{
				Stickman *a = *i;

				// Stickmans <--> Bullets
				for (int i = 0; i < nBullets; i++)
					if ((bullets [i].getOwnerType () == objectType::player && a->getType () >= objectType::citizen) ||
						(bullets [i].getOwnerType () >= objectType::turret && a->getType () <= objectType::citizen))
						if (bullets [i].getPos ().x < a->getPos ().x + a->getSize ().x/2.f &&
							bullets [i].getPos ().x > a->getPos ().x - a->getSize ().x/2.f &&
							bullets [i].getPos ().y < a->getPos ().y &&
							bullets [i].getPos ().y > a->getPos ().y - a->getSize ().y)
							{
							a->damage (bullets [i].getDmg (),
								(bullets [i].getVel ()*bullets [i].getMass ()));

							bullets [i].decreaseDmg (bullets [i].getDmg ());
							}

				if (a->getType () == objectType::player)
					{
					thisPlayerPos = a->getBulletStart ();

					a->Update (level, time, sf::Vector2f (sf::Mouse::getPosition (window))*1920.f/float (window.getSize ().x), lMousePrsd);
					}
				else
					a->Update (level, time, thisPlayerPos);

				if (a->isShoot ())
					CreateBulletsFromGun (bullets+nBullets, nBullets, a->getBulletStart (), a->getHandAngle (), a->getDisp (), a->getGun (), a->getVel (), a->getType());

				// Stickmans <--> MapObjects
				for (auto b: mapObjects)
					{
					sf::Vector2f closestPos = sf::Vector2f (INFINITY, INFINITY);

					for (auto c: stickmans)
						if (vecL (c->getBulletStart () - b->getPos ()) < vecL (closestPos))
							closestPos = c->getBulletStart ();
					
					b->Update (level, time, closestPos);
					}

				if (!a->alive ())
					{
					i = stickmans.erase (i);
					delete a;
					}
				else
					i++;
				}

			// Bullets
			for (int i = 0; i < nBullets; i++)
				{
				bullets [i].Update (level, time);

				if (!bullets [i].alive ())
					{
					bullets [i] = bullets [nBullets-1];
					nBullets--;
					}
				}

			for (auto a: mapObjects)
				a->Update (level, time);
			}
		
		// Snowflakes
		for (int i = 0; i < 100; i++)
			{
			snow [i] += snowSpeed [i]*time;
			
			if (snow [i] > camera.cam.getCenter ().y+window.getSize ().y/2) snow [i] -= window.getSize ().y;
			if (snow [i] < camera.cam.getCenter ().y-window.getSize ().y/2) snow [i] += window.getSize ().y;
			}
		
		graphics = clock ();
		window.setView (camera.PlayerCam (sf::Vector2f (thisPlayerPos.x, thisPlayerPos.y-200)));

		// Graphics
		window.clear (sf::Color (32, 32, 32));
		background.setPosition (camera.cam.getCenter () - sf::Vector2f (background.getLocalBounds().width, background.getLocalBounds ().height)/2.f);
		window.draw (background);

		// Drawing Tile Map
		level.Draw (window, map_sprite, thisPlayerPos);
		// Other objects
		for (auto a: mapObjects)
			a->Draw (window, time);

		// Drawing stickmans
		for (auto a: stickmans)
			a->Draw (window, time);
		// Bullets
		for (int i = 0; i < nBullets; i++)
			if (onScreen (bullets [i].getPos (), window, camera))
				bullets [i].Draw (window, time);
			
		for (int i = int (camera.cam.getCenter ().x-window.getSize().x/2)/20; i < int (camera.cam.getCenter ().x+window.getSize ().x/2)/20; i++)
			{
			sf::CircleShape snowflake;
			snowflake.setPosition (i*20.f, snow [(i+100)%100]);
			snowflake.setRadius (2);
			snowflake.setFillColor (sf::Color::White);
			window.draw (snowflake);
			}
			
		end = clock ();
		window.display ();	
		tickTimer++;
		}
	}