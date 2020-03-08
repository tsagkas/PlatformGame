#include "Entity.h"
#include <iostream>

// Entity

Entity::Entity()
{
	fPosX = 51.0f;
	fPosY = 51.0f;

	fVelX = 0.0f;
	fVelY = 0.0f;
}

sf::Sprite Entity::getSprite()
{
	return m_Sprite;
}

sf::Vector2f Entity::getPosition()
{
	sf::Vector2f tmp(fPosX, fPosY);
	return tmp;
}

sf::Vector2f Entity::getCenter()
{
	return sf::Vector2f(
		fPosX + m_Sprite.getGlobalBounds().width / 2,
		fPosY + m_Sprite.getGlobalBounds().height / 2);
}

// Player

Player::Player() 
{
	fPosX = 51.0f;
	fPosY = 51.0f;

	nHealth = 100;
	nMaxHealth = 100;

	m_nDirection = RIGHT;
	m_nState = STANDING;

	m_Sprite = sf::Sprite(*AssetHandler::get().GetTexture("mario"));
}

// 18/10 16¨00 παναγιωτοπουλος

void Player::update(float fElapsedTime, const Map& map)
{
	float fOldPosX = fPosX;
	float fOldPosY = fPosY;

	if (bMoveLeft)
	{
		fVelX -= (!bIsJumping ? 25.0f : 0.5f);
	}

	if (bMoveRight)
	{
		fVelX += (!bIsJumping ? 25.0f : 0.5f);
	}

	if (bJump && bCanJump)
	{
		bIsJumping = true;
		fVelY = 800.0f;
	}

	if (bIsJumping)
	{
		fTimeThisJump += fElapsedTime;
		bCanJump = false;
		bOnGround = false;

		if (fTimeThisJump < fJumpDuration)
		{
			fPosY -= fVelY * fElapsedTime;
		}
		else
		{
			bIsJumping = false;
			fTimeThisJump = 0.0f;
			fVelY = 0.0f;
		}
	}
	else if(!bOnGround)
	{
		fPosY += 500 * fElapsedTime;
	}

	fVelX += -3.0f * fVelX * fElapsedTime;

	if (fabs(fVelX) < 0.01f)
		fVelX = 0.0f;
	
	if (fVelX > 530.0f)
		fVelX = 530.0f;

	if (fVelX < -530.0f)
		fVelX = -530.0f;

	fPosX += fVelX * fElapsedTime;


	detectColision(map, fOldPosX, fOldPosY, fElapsedTime);

	m_Sprite.setPosition(fPosX, fPosY);

	bMoveRight = false;
	bMoveLeft = false;
	bJump = false;
}

void Player::jump()
{
	bJump = true;
}

void Player::moveLeft()
{
	bMoveLeft = true;
}

void Player::moveRight()
{
	bMoveRight = true;
}

bool Player::canPlayerJump()
{
	return bCanJump;
}

void Player::detectColision(const Map& map, float fOldPosX, float fOldPosY, float fElapsedTime)
{
	// Colission detection with map boundaries: x-axis.
	if (fPosX < map.TILE_SIZE)
		fPosX = map.TILE_SIZE;

	else if (fPosX > (float)map.getLevelSize().x * map.TILE_SIZE - 2 * map.TILE_SIZE)
		fPosX = map.getLevelSize().x * map.TILE_SIZE - 2 * map.TILE_SIZE;

	// Colission detection with map boundaries: y-axis.
	if (fPosY < map.TILE_SIZE)
		fPosY = map.TILE_SIZE;

	// Colision with the void/hole. 
	else if (fPosY > (float)map.getLevelSize().y * map.TILE_SIZE - 2 * map.TILE_SIZE)
	{
		fPosY = map.getLevelSize().y * map.TILE_SIZE - 2 * map.TILE_SIZE;

		bOnGround = true;
		bCanJump = true;

		bFell = true;
	}
		
	// Floor Tile Collision. (X: +/- 1 so that I can jump inside of pipes && Y: - 1 so that player does not flow)
	if (map.getIndex(floor((fPosX - 1 + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY + map.TILE_SIZE) / map.TILE_SIZE)) != 0 ||
		map.getIndex(floor((fPosX + 1) / map.TILE_SIZE), floor((fPosY + map.TILE_SIZE) / map.TILE_SIZE)) != 0)
	{
		fPosX = fOldPosX;
		fPosY = fOldPosY;

		bOnGround = true;
		bCanJump = true;
	}
	else
	{
		bOnGround = false;
		bCanJump = false;
	}

	// Ceiling Tile collision. (X: +/- 1 so that I can jump inside of pipes && Y: no changes so that jump is not allowed inside of tunnels)
	if (map.getIndex(floor((fPosX + 1) / map.TILE_SIZE), floor((fPosY + 1) / map.TILE_SIZE)) != 0 ||
		map.getIndex(floor((fPosX - 1 + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY + 1) / map.TILE_SIZE)) != 0)
	{
		fPosX = fOldPosX;
		fPosY = fOldPosY;	

		bIsJumping = false;
		fTimeThisJump = 0.0f;
	}

	// Allow smooth movement on LEFT pipe edges.
	if (map.getIndex(floor((fPosX) / map.TILE_SIZE), floor(fPosY / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX) / map.TILE_SIZE), floor((fPosY + map.TILE_SIZE) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX) / map.TILE_SIZE), floor((fPosY + map.TILE_SIZE + 1) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX + map.TILE_SIZE + 2) / map.TILE_SIZE), floor((fPosY + map.TILE_SIZE + 2) / map.TILE_SIZE)) != 0)
	{
		bCanMoveLeft = false;
		fPosX = fOldPosX;
	}

	// Allow smooth movement on RIGHT pipe edges.
	if (map.getIndex(floor((fPosX + map.TILE_SIZE) / map.TILE_SIZE), floor(fPosY / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY + map.TILE_SIZE) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY + map.TILE_SIZE + 1) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX - 2) / map.TILE_SIZE), floor((fPosY - 2) / map.TILE_SIZE)) != 0)
	{
		bCanMoveRight = false;
		fPosX = fOldPosX;
	}

	// Don't get stuck while jumping on the LEFT.
	if (map.getIndex(floor((fPosX + map.TILE_SIZE) / map.TILE_SIZE), floor(((fPosY ) + map.TILE_SIZE) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor(fPosX / map.TILE_SIZE), floor(((fPosY ) + map.TILE_SIZE) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor((fPosX + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY ) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor((fPosX - 1)/ map.TILE_SIZE), floor((fPosY + 1) / map.TILE_SIZE)) != 0 ||
		
		map.getIndex(floor((fPosX + map.TILE_SIZE) / map.TILE_SIZE), floor(((fPosY ) + map.TILE_SIZE) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor((fPosX - 1) / map.TILE_SIZE), floor((fPosY + map.TILE_SIZE - 1) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY ) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor(fPosX / map.TILE_SIZE), floor((fPosY ) / map.TILE_SIZE)) == 0 ||
		
		map.getIndex(floor((fPosX + map.TILE_SIZE) / map.TILE_SIZE), floor(((fPosY + 1) + map.TILE_SIZE) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor((fPosX - 1) / map.TILE_SIZE), floor((fPosY + map.TILE_SIZE - 1) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor((fPosX - 1) / map.TILE_SIZE), floor((fPosY + 1) / map.TILE_SIZE)) != 0)
	{
		bCanMoveLeft = false;
		fVelX = 0.0f;
	}
	else
	{
		bCanMoveLeft = true;
	}

	// Don't get stuck while jumping on the RIGHT.
	if (map.getIndex(floor((fPosX + map.TILE_SIZE) / map.TILE_SIZE), floor(((fPosY ) + map.TILE_SIZE) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor(fPosX / map.TILE_SIZE), floor(((fPosY ) + map.TILE_SIZE) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor((fPosX + map.TILE_SIZE + 1) / map.TILE_SIZE), floor((fPosY + 1) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor(fPosX / map.TILE_SIZE), floor((fPosY ) / map.TILE_SIZE)) == 0 ||
		
		map.getIndex(floor((fPosX + map.TILE_SIZE + 1) / map.TILE_SIZE), floor(((fPosY ) + map.TILE_SIZE) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor(fPosX / map.TILE_SIZE), floor(((fPosY ) + map.TILE_SIZE) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor((fPosX + map.TILE_SIZE ) / map.TILE_SIZE), floor((fPosY - 1) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor(fPosX / map.TILE_SIZE), floor((fPosY ) / map.TILE_SIZE)) == 0 ||
		
		map.getIndex(floor((fPosX + map.TILE_SIZE + 1) / map.TILE_SIZE), floor(((fPosY) + map.TILE_SIZE) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor(fPosX / map.TILE_SIZE), floor(((fPosY + 1) + map.TILE_SIZE) / map.TILE_SIZE)) == 0 &&
		map.getIndex(floor((fPosX + map.TILE_SIZE + 1) / map.TILE_SIZE), floor((fPosY + 1) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor(fPosX / map.TILE_SIZE), floor((fPosY) / map.TILE_SIZE)) == 0)
	{
		bCanMoveRight = false;
		fVelX = 0.0f;
	}
	else
	{
		bCanMoveRight = true;
	}

	// Stop left and right movement while jumping inside a pipe.
	if (map.getIndex(floor((fPosX - 1) / map.TILE_SIZE), floor((fPosY + 1) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX + 1 + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY + 1) / map.TILE_SIZE)) != 0 ||

		map.getIndex(floor((fPosX - 1) / map.TILE_SIZE), floor((fPosY - 1+ map.TILE_SIZE) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX + 1 + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY - 1 + map.TILE_SIZE) / map.TILE_SIZE)) != 0)
	{
		bCanMoveRight = false;
		bCanMoveLeft = false;
	}

	// Forbid jumping inside of tunnels.
	if (map.getIndex(floor((fPosX + 1) / map.TILE_SIZE), floor((fPosY - 1) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX + 1) / map.TILE_SIZE), floor((fPosY + 1 + map.TILE_SIZE) / map.TILE_SIZE)) != 0 ||

		map.getIndex(floor((fPosX - 1 + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY - 1) / map.TILE_SIZE)) != 0 &&
		map.getIndex(floor((fPosX - 1 + map.TILE_SIZE) / map.TILE_SIZE), floor((fPosY + 1 + map.TILE_SIZE) / map.TILE_SIZE)) != 0)
	{
		bCanJump = false;
	}
}

void Player::respawn()
{
	fPosX = 51.0f;
	fPosY = 51.0f;

	bFell = false;
}