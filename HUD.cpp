#include "HUD.h"

HUD::HUD()
{
	sf::Vector2i resolution(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);

	m_Font.loadFromFile("Fonts/Deadpack.ttf");

	// Time text.
	m_TimeText.setFont(m_Font);
	m_TimeText.setCharacterSize(38);
	m_TimeText.setFillColor(sf::Color::Black);
	m_TimeText.setPosition(resolution.x - 450, 15);
	m_TimeText.setString("---");

	//Warning text.
	m_WarningText.setFont(m_Font);
	m_WarningText.setCharacterSize(38);
	m_WarningText.setFillColor(sf::Color::Red);
	m_WarningText.setPosition(20, 15);
	m_WarningText.setString("Time is running out!");
}

void HUD::setTime(float fTime)
{
	std::string time = "Remaining Time: ";
	time.append(std::to_string((int)fTime));
	m_TimeText.setString(time);
}

sf::Text HUD::getTime()
{
	return m_TimeText;
}

sf::Text HUD::getWarning()
{
	return m_WarningText;
}