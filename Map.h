#pragma once
#include <SFML/Graphics.hpp>

class Map
{
private:
	sf::Vector2i m_LevelSize; 
	sf::Vector2f m_LevelSpawnPos;

	int** m_indices{ nullptr };

public:
	const int TILE_SIZE{ 64 };
	const int VERTS_IN_QUAD{ 4 };

	int** loadLevel(sf::VertexArray& rVaLevel);
	sf::Vector2i getLevelSize() const;

	int getIndex(int x, int y) const;
};

