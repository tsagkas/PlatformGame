#pragma once
#include <SFML/Graphics.hpp>

class HUD
{
public:
	HUD();

private:
	sf::Font m_Font;
	sf::Text m_TimeText;
	sf::Text m_WarningText;

public:
	void setTime(float fTime);

	sf::Text getTime();
	sf::Text getWarning();
};

