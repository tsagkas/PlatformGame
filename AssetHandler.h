#pragma once
#include <map>
#include <SFML/Graphics.hpp>


class AssetHandler
{
public:
	static AssetHandler& get()
	{
		static AssetHandler me;
		return me;
	}

	AssetHandler(AssetHandler const&) = delete;
	void operator=(AssetHandler const&) = delete;

private:
	AssetHandler();
	~AssetHandler();

	std::map<std::string, sf::Texture*> m_Textures;

public:

	sf::Texture* GetTexture(std::string filename)
	{
		return m_Textures[filename];
	}

	void LoadTextures();
};

