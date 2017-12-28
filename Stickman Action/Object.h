#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"

class Object
	{
	protected:
		sf::Vector2f position;
		sf::Vector2f velocity;
		float angle = 0;
		bool onGround = false, life = true, trigger = false;

		sf::Vector2f size;
		float mass = 1, hp = 100;
		sf::Texture texture;
		sf::String type;

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

		virtual void Control (sf::Vector2f target, float time) = 0;

		virtual void CheckBorders (Level &level, float time) = 0;

		void Update (Level level, float time, sf::Vector2f target = sf::Vector2f (0, 0), bool TRG = false)
			{
			CheckBorders (level, time);

			Control (target, time);
			if (!onGround) velocity.y += 9.8f*time;
			trigger = TRG;
			position += velocity*75.0f*time;
			}

		virtual void Draw (sf::RenderWindow &window, float time) = 0;

		sf::Vector2f getPos () { return position; }
		sf::Vector2f getVel () { return velocity; }
		float        getMass () { return mass; }
		sf::String   getType () { return type; }
		bool         getLife () { return life;  }

	};