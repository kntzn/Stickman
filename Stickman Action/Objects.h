#pragma once
#include <SFML/Graphics.hpp>
#include "Map.h"
#include "Mission.h"
#include "Guns.h"

namespace npcAction
	{
	enum
		{
		Stay,
		Walk,
		Run,
		Back,
		Shoot,
		Rush
		};
	}
float getNpcRange (unsigned int level)
	{
	return float (pow (level, 1.1f))*2000.f;
	}

class Stickman: public Object
	{
	protected:
		// ?
		unsigned int action = Action::Stay;
		bool way = 0;
		unsigned int fraction = -1;

		// Appearance
		Appearance app;

		// Guns
		bool shooting = false;
		int currentGun = 0, slot = 0;
		int gunSwitchFlag = 0;
		float handAngle = 0, dispersion = 0;
		Gun guns [3] = { PSR400, F12, hands };
		sf::Texture gunsTexture;
		
		// Constructor
		Stickman (sf::Image &image, sf::Image &gunImage, sf::Vector2f POS, float M): Object (image, POS, M)
			{
			size = sf::Vector2f (180, 240);
			gunsTexture.loadFromImage (gunImage);
			}

		virtual void Control (Level &lvl, sf::Vector2f target, float time) = 0;
		int CheckBorders (Level &level, float time)
			{
			if (0 <= (position - size/2.f).x && 0 <= (position - size).y &&
				(position + size/2.f).x < MAP_W*100.f && (position).y < MAP_H*100.f)
				{
				// floor
				if (level.PhysicalMap [int (position.y/100)] [int ((position.x - 30)/100)] == 1 || level.PhysicalMap [int (position.y/100)] [int ((position.x + 30)/100)] == 1)
					{
					position.y = int (position.y/100)*100.f;
					velocity.y = 0;
					onGround = true;
					}
				else
					onGround = false;

				// ceilling
				if (level.PhysicalMap [int ((position.y - size.y)/100)] [int (position.x/100)] == 1)
					{
					position.y = int ((position.y-size.y)/100)*100.f + int ((size.y)/100+2)*100;
					velocity *= -0.8f;
					return 1;
					}

				// side borders
				for (int i = 0; i < int (size.y + 99)/100-1; i++)
					{
					if (level.PhysicalMap [int (position.y)/100 - i - 1] [int ((position.x + size.x/2)/100)] == 1)
						{
						velocity.x *= -0.3f;
						position.x = int ((position.x + size.x/2)/100)*100 - size.x/2;
						return 2;
						}
					if (level.PhysicalMap [int (position.y)/100 - i - 1] [int ((position.x - size.x/2)/100)] == 1)
						{
						velocity.x *= -0.3f;
						position.x = int ((position.x - size.x/2)/100)*100 + 100 + size.x/2;
						return 3;
						}
					}
				}
			else
				onGround = false;

			return 1;
			}

	public:
		void Draw (sf::RenderWindow &window, float time)
			{
			sf::Sprite hands;
			sf::Sprite gunCharge;

			hands.setOrigin (90.f + 90.f*way, 10.f);
			gunCharge.setOrigin (90.f + 90.f*way, -20.f+int (1.f + 30*guns [currentGun].rechargePercentage ()));
			hands.setPosition (position - sf::Vector2f (0, 180));
			gunCharge.setPosition (hands.getPosition ());
			hands.rotate (handAngle*toDeg+180*way+90);
			gunCharge.rotate (hands.getRotation ());

			hands.setTexture (gunsTexture);
			gunCharge.setTexture (gunsTexture);

			switch (action)
				{
				case Action::Stay:
					hands.setTextureRect (sf::IntRect (270*(way), 60*guns [currentGun].getID (), 270*(1 - 2*int (way)), 60));
					gunCharge.setTextureRect (sf::IntRect (270*(way) + 270, 60*guns [currentGun].getID () + int (30 - 30*guns [currentGun].rechargePercentage ()), 270*(1 - 2*int (way)), int (60*guns [currentGun].rechargePercentage ())));
					break;
				case Action::Walk:
					hands.setTextureRect (sf::IntRect (270*(way), 60*guns [currentGun].getID (), 270*(1 - 2*int (way)), 60));
					gunCharge.setTextureRect (sf::IntRect (270*(way) +270, 60*guns [currentGun].getID () + int (30 - 30*guns [currentGun].rechargePercentage ()), 270*(1 - 2*int (way)), int (60*guns [currentGun].rechargePercentage ())));
					break;
				case Action::Run:
					hands.setTextureRect (sf::IntRect (270*(way), 60*guns [currentGun].getID (), 270*(1 - 2*int (way)), 60));
					gunCharge.setTextureRect (sf::IntRect (270*(way) +270, 60*guns [currentGun].getID () + int (30 - 30*guns [currentGun].rechargePercentage ()), 270*(1 - 2*int (way)), int (60*guns [currentGun].rechargePercentage ())));
					break;
				case Action::Sprint:
					hands.setTextureRect (sf::IntRect (270*(way), 60*guns [currentGun].getID (), 270*(1 - 2*int (way)), 60));
					gunCharge.setTextureRect (sf::IntRect (270*(way) +270, 60*guns [currentGun].getID () + int (30 - 30*guns [currentGun].rechargePercentage ()), 270*(1 - 2*int (way)), int (60*guns [currentGun].rechargePercentage ())));
					break;
				case Action::Jump:
					hands.setTextureRect (sf::IntRect (270*(way), 60*guns [currentGun].getID (), 270*(1 - 2*int (way)), 60));
					gunCharge.setTextureRect (sf::IntRect (270*(way) +270, 60*guns [currentGun].getID () + int (30 - 30*guns [currentGun].rechargePercentage ()), 270*(1 - 2*int (way)), int (60*guns [currentGun].rechargePercentage ())));
					break;
				case Action::Fly:
					hands.setTextureRect (sf::IntRect (270*(way), 60*guns [currentGun].getID (), 270*(1 - 2*int (way)), 60));
					gunCharge.setTextureRect (sf::IntRect (270*(way) +270, 60*guns [currentGun].getID () + int (30 - 30*guns [currentGun].rechargePercentage ()), 270*(1 - 2*int (way)), int (60*guns [currentGun].rechargePercentage ())));
					break;
				default:
					break;
				}

			Draw::Stickman (window, texture, position, false, velocity, time, way, angle, action, app);
			window.draw (hands);
			if (guns[currentGun].animationAvailable ())
				window.draw (gunCharge);
			}

		bool isShoot ()
			{ return shooting; }
		float getHandAngle ()
			{ return handAngle; }
		float getDisp ()
			{ return dispersion; }
		Gun getGun ()
			{ return guns [currentGun]; }
		sf::Vector2f getBulletStart ()
			{
			return position + sf::Vector2f (0, -160.f);
			}
	};

class Player: public Stickman
	{
	private:
		bool lastTriggerState = false;

		void Control (Level &lvl, sf::Vector2f target, float time)
			{
			// Walls interactions controls (jumping & sliding)
			int checkBordersResult = CheckBorders (lvl, time);
			if (currentGun == gunSlot::Melee)
				{
				if (checkBordersResult == 2)
					{
					if (way == 0)
						{
						if (sf::Keyboard::isKeyPressed (sf::Keyboard::Space))
							{
							velocity.y = -7.5f;
							velocity.x = -7.5f;
							}
						else if (sf::Keyboard::isKeyPressed (sf::Keyboard::W))
							{
							velocity.x = 0.1f;
							if (velocity.y > 9.8f/8)
								velocity.y += (9.8f/8 - velocity.y)*time;
							}
						}
					}
				if (checkBordersResult == 3)
					{
					if (way == 1)
						{
						if (sf::Keyboard::isKeyPressed (sf::Keyboard::Space))
							{
							velocity.y = -7.5f;
							velocity.x = 7.5f;
							}
						else if (sf::Keyboard::isKeyPressed (sf::Keyboard::W))
							{
							velocity.x = -0.1f;
							if (velocity.y > 9.8f/8)
								velocity.y += (9.8f/8 - velocity.y)*time;
							}
						}
					}
				}

			// Toolbar switch
			if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num1))
				{
				slot = 0;
				gunSwitchFlag = 1;
				}
			else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num2))
				{
				slot = 1;
				gunSwitchFlag = 1;
				}
			else if (sf::Keyboard::isKeyPressed (sf::Keyboard::Num3))
				{
				slot = 2;
				gunSwitchFlag = 1;
				}

			float mouseAngle = atan2 (1920/2-target.x, target.y-1080/2);
			dispersion -= (dispersion*20 - fabs (velocity.x) - fabs (velocity.y))/1000;
			size.x = (guns[currentGun].getLength()-30)*2;

			// Guns switch animation
			if (gunSwitchFlag == 1)
				{
				if (handAngle > Pi*0.0625f)
					handAngle -= 4*Pi*time;
				else if (handAngle < -Pi*0.0625f)
					handAngle += 4*Pi*time;
				else
					{
					gunSwitchFlag = 2;
					currentGun = slot;
					}
				}
			else if (gunSwitchFlag == 2)
				{
				if (mouseAngle > handAngle && handAngle > 0)
					handAngle += 4*Pi*time;
				else if (mouseAngle < handAngle && handAngle < 0)
					handAngle -= 4*Pi*time;
				else
					gunSwitchFlag = 0;
				}
			else
				handAngle = mouseAngle;

			shooting = trigger;
			if (!guns [currentGun].isReady () ||
				(guns [currentGun].getTriggerType () == triggerType::SemiAuto && lastTriggerState == true && trigger == true))
				shooting = false;
			
			if (shooting)
				guns [currentGun].fire ();

			guns [currentGun].update (time);

			// Way switching
			if (mouseAngle > 0) way = 1;
			else                way = 0;

			// Run & Walk controls
			if (fabs (velocity.x) < 0.05f) velocity.x = 0;
			bool sprint = sf::Keyboard::isKeyPressed (sf::Keyboard::LShift);
			if (sf::Keyboard::isKeyPressed (sf::Keyboard::D) && onGround && velocity.x <  15) velocity.x += (15.f)*time;
			if (sf::Keyboard::isKeyPressed (sf::Keyboard::A) && onGround && velocity.x > -15) velocity.x -= (15.f)*time;
			if (onGround)
				{
				if (sf::Keyboard::isKeyPressed (sf::Keyboard::Space)) velocity.y = -5;
				if (!sf::Keyboard::isKeyPressed (sf::Keyboard::A) && !sf::Keyboard::isKeyPressed (sf::Keyboard::D))
					{
					action = Action::Stay;
					if      (velocity.x >  30.f*time) velocity.x -= 30.f*time;
					else if (velocity.x < -30.f*time) velocity.x += 30.f*time;
					else                              velocity.x = 0;
					}
				else if (fabs (velocity.x) > 11)
					action = Action::Sprint;		
				else if (fabs (velocity.x) > 5)	
					action = Action::Run;
				
				else if (fabs (velocity.x) <= 5)
					action = Action::Walk;
				
				}
			else
				{
				if (velocity.y < -2.5f) { action = Action::Jump; }
				else                    { action = Action::Fly;  }
				}

			if (velocity.x >  10 + 10*sprint) velocity.x -= 7.5f*time;
			if (velocity.x < -10 - 10*sprint) velocity.x += 7.5f*time;

			lastTriggerState = trigger;
			}
	public:
		Player (sf::Image &image, sf::Image &gunImage, sf::Vector2f POS, float M):Stickman (image, gunImage, POS, M)
			{
			type = objectType::player;
			fraction = 0;
			}
	};
class NPC: public Stickman
	{
	private:
		int fightAction = npcAction::Stay;
		unsigned int npcLevel = 0;

		void walk (float speed, float time)
			{
			if (way == 0)
				{
				if (velocity.x < speed - 7.5f*time)
					velocity.x += 7.5f*time;
				handAngle = -Pi/2.f;
				}
			else
				{
				if (velocity.x > -speed + 7.5f*time)
					velocity.x -= 7.5f*time;
				handAngle = Pi/2.f;
				}
			}

		void Control (Level &lvl, sf::Vector2f target, float time)
			{
			sf::Vector2f trgDiff = target - getBulletStart();
			float trgAngle = atan2 (-trgDiff.x, trgDiff.y);
			float dist = vecL (trgDiff);

			// Logic
			size.x = (guns [currentGun].getLength ()-30)*2;
			int checkResult = CheckBorders (lvl, time);

			switch (type)
				{
				case objectType::citizen:
					{
					if (fightAction == npcAction::Walk)
						{
						if (checkResult > 1)
							{
							if (way == 1) way = 0;
							else          way = 1;
							}
						walk (2.5f, time);

						if (hp != 100)
							fightAction = npcAction::Run;
						}
					else if (fightAction == npcAction::Run)
						{
						if (CheckBorders (lvl, time) > 1)
							way = !way;

						walk (10.f, time);
						}

					break;
					}
				case objectType::solder:
					{
					if (hp != maxHp || trigger)
						{
						if (hp < maxHp/4.f)
							fightAction = npcAction::Back;
						else if (hp < maxHp/2.f)
							fightAction = npcAction::Shoot;
						else
							fightAction = npcAction::Rush;
						}

					if (fightAction == npcAction::Walk)
						{
						if (checkResult > 1)
							{
							if (way == 1) way = 0;
							else          way = 1;
							}
						walk (2.5f, time);
						}
					else
						{
						if (dist < 300)
							currentGun = gunSlot::Melee;
						else
							currentGun = gunSlot::Primary;

						if (fightAction == npcAction::Rush)
							{
							float angleBeforeUpdate = handAngle;

							// Choosing the way
							if (handAngle > 0) way = 1;
							else               way = 0;

							if (trgDiff.x > 0)
								way = 0;
							else
								way = 1;
							walk (10.f, time);

							shooting = false;
							if (dist < getNpcRange (npcLevel))
								{
								handAngle = angleBeforeUpdate;

								// Aiming & shooting
								if (trgAngle > handAngle+2.f*Pi*time)
									handAngle += 2.f*Pi*time;
								if (trgAngle < handAngle-2.f*Pi*time)
									handAngle -= 2.f*Pi*time;
								else if (guns [currentGun].isReady ())
									{
									guns [currentGun].fire ();
									//shooting = true;
									}
								}
							}
						else if (fightAction == npcAction::Shoot)
							{
							// Choosing the way
							if (handAngle > 0) way = 1;
							else               way = 0;

							// Stopping 
							if (velocity.x >  7.5f*time) velocity.x -= 7.5f*time;
							else if (velocity.x < -7.5f*time) velocity.x += 7.5f*time;
							else velocity.x = 0;

							shooting = false;
							if (dist < getNpcRange (npcLevel))
								{
								// Aiming & shooting
								if (trgAngle > handAngle+2.f*Pi*time)
									handAngle += 2.f*Pi*time;
								if (trgAngle < handAngle-2.f*Pi*time)
									handAngle -= 2.f*Pi*time;
								else if (guns [currentGun].isReady ())
									{
									guns [currentGun].fire ();
									shooting = true;
									}
								}

							}
						else if (fightAction == npcAction::Back)
							{
							float angleBeforeUpdate = handAngle;

							// Walking
							if (trgDiff.x > 0)
								way = 1;
							else
								way = 0;
							walk (10.f, time);

							handAngle = angleBeforeUpdate;
							way = !way;

							// Aiming & shooting
							shooting = false;
							if (dist < getNpcRange (npcLevel))
								{
								if (trgAngle > handAngle+2.f*Pi*time)
									handAngle += 2.f*Pi*time;
								if (trgAngle < handAngle-2.f*Pi*time)
									handAngle -= 2.f*Pi*time;
								else if (guns [currentGun].isReady ())
									{
									guns [currentGun].fire ();
									shooting = true;
									}
								}

							}
						}

					guns [currentGun].update (time);

					break;
					}

				default:
					CheckBorders (lvl, time);
					break;
				}
			
			// Actions
			if (onGround)
				{
				if (fabs (velocity.x) > 11)
					action = Action::Sprint;
				else if (fabs (velocity.x) > 5)
					action = Action::Run;
				else if (fabs (velocity.x) <= 5)
					action = Action::Walk;
				else
					action = Action::Stay;
				}
			else
				{
				if (velocity.y < -2.5f) { action = Action::Jump; }
				else { action = Action::Fly; }
				}
			}
	public:
		NPC (sf::Image &image, sf::Image &gunImage, sf::Vector2f POS, float M, int npcId, int FRACTION, unsigned int npcLVL, bool WAY):Stickman (image, gunImage, POS, M)
			{
			type = npcId;
			way = WAY;
			fraction = FRACTION;
			npcLevel = npcLVL;
			hp = maxHp;

			switch (npcId)
				{
				case objectType::citizen:
					{
					fightAction = npcAction::Walk;
					
					guns [0] = hands;
					guns [1] = hands;
					guns [2] = hands;
					app.clothes = 2;
					break;
					}
				case objectType::solder:
					{
					velocity = sf::Vector2f (0, 0);
					handAngle = Pi*WAY-Pi/2.f;
					fightAction = npcAction::Walk;
					guns [0] = F12;
					guns [1] = hands;
					guns [2] = hands;
					app.clothes = 1;
					}
				default:
					break;
				}

			}
	};

class ChristmasTree: public Object
	{
	public:
		void Draw (sf::RenderWindow &window, float time)
			{
			sf::Sprite sp;
			sp.setTexture (texture);
			sp.setPosition (position);
			sp.setOrigin (94, 311);
			window.draw (sp);
			}

		int CheckBorders (Level &level, float time)
			{
			// floor
			if (level.PhysicalMap [int (position.y/100)] [int ((position.x - 30)/100)] == 1 || level.PhysicalMap [int (position.y/100)] [int ((position.x + 30)/100)] == 1)
				{
				position.y = int (position.y/100)*100.f;
				velocity.y = 0;
				onGround = true;
				}
			else
				onGround = false;

			// ceilling
			if (level.PhysicalMap [int ((position.y - size.y)/100)] [int (position.x/100)] == 1)
				{
				position.y = int ((position.y)/100)*100.f + int (size.y)%100;
				velocity *= -0.8f;
				}

			// side borders
			for (int i = 0; i < int (size.y + 99)/100; i++)
				{
				if (level.PhysicalMap [int (position.y)/100 - i - 1] [int ((position.x + size.x/2)/100)] == 1)
					{
					velocity.x *= -0.3f;
					position.x = int ((position.x + size.x/2)/100)*100 - size.x/2;
					return 2;
					}
				if (level.PhysicalMap [int (position.y)/100 - i - 1] [int ((position.x - size.x/2)/100)] == 1)
					{
					velocity.x *= -0.3f;
					position.x = int ((position.x - size.x/2)/100)*100 + 100 + size.x/2;
					return 3;
					}
				}

			return 1;
			}

		void Control (Level &lvl, sf::Vector2f target, float time)
			{
			CheckBorders (lvl, time);
			}


		ChristmasTree (sf::Image &image, sf::Vector2f POS, float M): Object (image, POS, M)
			{
			size = sf::Vector2f (188, 311);
			}

	};