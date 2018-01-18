#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"

enum objectType
	{
	player,
	citizen,
	turret,
	soldier,
	sergeant,
	capitain,
	general,
	commander,
	door,
	console,
	lift
	};

class Object
	{
	protected:
		sf::Vector2f position;
		sf::Vector2f velocity;
		float angle = 0;
		bool onGround = false, trigger = false;

		sf::Vector2f size;
		float mass = 1, hp = 100.f, maxHp = hp;
		sf::Texture texture;
		int type;

		Object ()
			{	
			}

	public:
		Object (sf::Image &image, sf::Vector2f POS, float M)
			{
			position = POS;
			velocity = sf::Vector2f (0, 0);
			angle = 0;
			onGround = false;

			mass = M;
			texture.loadFromImage (image);
			}

		virtual void Control (Level &lvl, sf::Vector2f target, float time) = 0;

		virtual int CheckBorders (Level &level, float time) = 0;

		void Update (Level &level, float time, sf::Vector2f target = sf::Vector2f (0, 0), bool TRG = false)
			{
			trigger = TRG;
			position += velocity*75.0f*time;
			Control (level, target, time);

			if (!onGround) velocity.y += 9.8f*time;
			}

		virtual void Draw (sf::RenderWindow &window, float time, bool DEBUG_VIEW = false) = 0;

		sf::Vector2f getPos  () { return position; }
		sf::Vector2f getVel  () { return velocity; }
		sf::Vector2f getSize () { return size; }
		float        getMass () { return mass; }
		int          getType () { return type; }
		float        getHp ()   { return hp; }
		bool         alive () { return hp > 0; }

		void damage (float dmg, sf::Vector2f kick = sf::Vector2f (0, 0)) { hp -= dmg; velocity += kick;  }
	};