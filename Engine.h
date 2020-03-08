#pragma once
#include <SFML/Graphics.hpp>
#include "AssetHandler.h"
#include "Entity.h"
#include "Map.h"
#include "HUD.h"

class Engine
{
public:
	Engine();
	~Engine();

private:
	sf::Time m_GameTotalTime; 
	float m_fTimeRemaining; 

	sf::RenderWindow m_Window;

	sf::View m_View; 

	sf::View m_BackgroundView;
	sf::Sprite m_BackgroundSprite; 

	Player* m_Player;

	Map m_Map;

	HUD m_HUD;
	sf::View m_HudView;

private:
	float fTimeRemaining{30.0f};

public:
	void run();

private:
	sf::VertexArray m_VertexArray;
	int** m_ArrayLevel{ nullptr };
	sf::Texture m_TextureTiles;

private:
	void input();
	void update(float fElapsedTime);
	void draw(); 

private:
	void loadLevel();
	void restartLevel();
};

