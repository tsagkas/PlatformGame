#include "AssetHandler.h"

AssetHandler::AssetHandler()
{
}

AssetHandler::~AssetHandler()
{
}

void AssetHandler::LoadTextures()
{
	sf::Texture* texture1 = new sf::Texture();
	texture1->loadFromFile("Graphics/Backgrounds/1.jpg");
	m_Textures.insert({ "background", texture1 });

	/*sf::Texture* texture2 = new sf::Texture();
	texture2->loadFromFile("Graphics/Characters/boop.png", sf::IntRect(0, 0, 512, 513));
	m_Textures.insert({ "mario", texture2 });*/

	sf::Texture* texture2 = new sf::Texture();
	texture2->loadFromFile("Graphics/Characters/Face.png");
	m_Textures.insert({ "mario", texture2 });

	sf::Texture* texture3 = new sf::Texture();
	texture3->loadFromFile("Graphics/WorldTiles/TileSheet.png");
	m_Textures.insert({ "world", texture3 });

	//sf::Texture* texture4 = new sf::Texture();
	//texture4->loadFromFile("Graphics/Characters/mush.png");
	//m_Textures.insert({ "mush", texture4 });

	//sf::Texture* texture5 = new sf::Texture();
	//texture5->loadFromFile("Graphics/Characters/bob.png");
	//m_Textures.insert({ "bob", texture5 });
}