#include <SFML/Graphics.hpp>
#include "AssetHandler.h"
#include "Map.h"

class Entity
{
public:
	Entity();

protected:
	float fPosX;
	float fPosY;
	float fVelX;
	float fVelY;

public:
	sf::Sprite m_Sprite;

public:
	sf::Sprite getSprite();
	sf::Vector2f getPosition();
	sf::Vector2f getCenter();

public:
	virtual void update(float fElapsedTime, const Map& map) = 0;
};

class Creature : public Entity
{
protected:
	int nHealth;
	int nMaxHealth;

	enum { LEFT = 0, RIGHT = 1, UP = 3 } m_nDirection;
	enum { STANDING, WALKING, JUMPING, DEAD } m_nState;
};

class Player : public Creature
{
public:
	Player();

private:
	float fJumpDuration = 0.2f;
	float fTimeThisJump = 0.00f;

	bool bOnGround{ false };
	bool bCanJump{ true };
	bool bIsJumping{ false };

	bool bJump{ false };
	bool bMoveRight{ false };
	bool bMoveLeft{ false };

public:
	virtual void update(float fElapsedTime, const Map& map) override;

public:
	void jump();
	void moveRight();
	void moveLeft();
	void respawn();
	bool canPlayerJump();

	bool bCanMoveRight{ true };
	bool bCanMoveLeft{ true };
	bool bFell{ false };

private:
	void detectColision(const Map& map, float fOldPosX, float fOldPosY, float fElapsedTime);
};

