#include "Engine.h"

Engine::Engine()
{
	// Screen resolution.
	sf::Vector2f resolution;
	resolution.x = sf::VideoMode::getDesktopMode().width;
	resolution.y = sf::VideoMode::getDesktopMode().height;

	m_Window.create(sf::VideoMode(resolution.x, resolution.y),"Mario Style Game", sf::Style::Fullscreen);
	m_View.setSize(resolution);
	m_View.setCenter(960.0f, 300.0f);

	m_HudView.reset(sf::FloatRect(0, 0, resolution.x, resolution.y));
	
	// Load graphics only once.
	AssetHandler::get().LoadTextures();

	// Background sprite has to be 
	m_BackgroundSprite.setTexture(*AssetHandler::get().GetTexture("background"));
	m_TextureTiles = *AssetHandler::get().GetTexture("world");
	
	// I used a pointer for the Player because I needed first to load all the textures. 
	m_Player = new Player();
}

Engine::~Engine()
{
	delete m_Player;
}

void Engine::run()
{
	sf::Clock clock;
	
	while (m_Window.isOpen())
	{
		sf::Time dt = clock.restart();
		m_GameTotalTime += dt;

		float fElapsedTime = dt.asSeconds();

		input();
		update(fElapsedTime);
		draw();
	}

	m_Window.close();
}


void Engine::input()
{	

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
	{
		m_Window.close();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && m_Player->bCanMoveRight)
	{
		m_Player->moveRight();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && m_Player->bCanMoveLeft)
	{
		m_Player->moveLeft();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
	{
		m_Player->jump();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
	{
		restartLevel();
	}
}

void Engine::update(float fElapsedTime)
{
	loadLevel();
	m_Player->update(fElapsedTime, m_Map);

	if (m_Player->bFell)
	{
		restartLevel();
	}

	if (m_View.getCenter().x - m_Player->getPosition().x < 300.0f)
		m_View.setCenter(m_Player->getPosition().x + 300.0f, 300.0f);

	// Update time for the HUD. 
	fTimeRemaining -= fElapsedTime;

	if (fTimeRemaining < 0)
	{
		restartLevel();
	}	
}

void Engine::draw()
{
	m_Window.clear(sf::Color::White);

	// Start

	m_Window.setView(m_BackgroundView);
	m_Window.draw(m_BackgroundSprite);

	m_Window.setView(m_View);

	m_Window.draw(m_VertexArray, &m_TextureTiles);

	m_Window.draw(m_Player->getSprite());

	m_Window.setView(m_HudView);
	m_HUD.setTime(fTimeRemaining);
	m_Window.draw(m_HUD.getTime());

	if ((int)fTimeRemaining < 5 && (int)fTimeRemaining > 2)
		m_Window.draw(m_HUD.getWarning());

	// End

	m_Window.display();
}

void Engine::loadLevel()
{
	for (int i = 0; i < m_Map.getLevelSize().y; i++)
	{
		delete[] m_ArrayLevel[i];

	}
	delete[] m_ArrayLevel;

	m_ArrayLevel = m_Map.loadLevel(m_VertexArray);
}

void Engine::restartLevel()
{
	m_Player->respawn();
	m_View.setCenter(960.0f, 300.0f);
	fTimeRemaining = 30.0f;
}