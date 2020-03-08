#include "Map.h"
#include <fstream>

int** Map::loadLevel(sf::VertexArray& rVaLevel)
{
	m_LevelSize.x = 0;
	m_LevelSize.y = 0;

	std::string levelToLoad;
	levelToLoad = "Maps/level.txt";

	m_LevelSpawnPos.x = 100;
	m_LevelSpawnPos.y = 100;

	// Load Map to **ArrayLevel[][]
	std::ifstream inputFile(levelToLoad);
	std::string s;

	while (getline(inputFile, s))
	{
		m_LevelSize.y++;
	}

	m_LevelSize.x = s.length();

	inputFile.clear();
	inputFile.seekg(0, std::ios::beg);

	int** arrayLevel = new int* [m_LevelSize.y];

	for (int i = 0; i < m_LevelSize.y; i++)
	{
		arrayLevel[i] = new int[m_LevelSize.x];
	}

	std::string row;

	int y = 0;

	while (inputFile >> row)
	{
		for (int x = 0; x < row.length(); x++)
		{
			const char val = row[x];
			arrayLevel[y][x] = atoi(&val);
		}
		y++;
	}

	inputFile.close();

	rVaLevel.setPrimitiveType(sf::Quads);
	rVaLevel.resize(m_LevelSize.x * m_LevelSize.y * VERTS_IN_QUAD);

	int currentVertex = 0;

	for (int x = 0; x < m_LevelSize.x; x++)
	{
		for (int y = 0; y < m_LevelSize.y; y++)
		{
			// Position each vertex in the current quad
			rVaLevel[currentVertex + 0].position = sf::Vector2f(x * TILE_SIZE, y * TILE_SIZE);

			rVaLevel[currentVertex + 1].position = sf::Vector2f((x * TILE_SIZE) + TILE_SIZE, y * TILE_SIZE);

			rVaLevel[currentVertex + 2].position = sf::Vector2f((x * TILE_SIZE) + TILE_SIZE, (y * TILE_SIZE) + TILE_SIZE);

			rVaLevel[currentVertex + 3].position = sf::Vector2f((x * TILE_SIZE), (y * TILE_SIZE) + TILE_SIZE);

			// Which tile from the sprite sheet should we use
			int verticalOffset = arrayLevel[y][x] * TILE_SIZE;

			rVaLevel[currentVertex + 0].texCoords = sf::Vector2f(0, 0 + verticalOffset);

			rVaLevel[currentVertex + 1].texCoords = sf::Vector2f(TILE_SIZE, 0 + verticalOffset);

			rVaLevel[currentVertex + 2].texCoords = sf::Vector2f(TILE_SIZE, TILE_SIZE + verticalOffset);

			rVaLevel[currentVertex + 3].texCoords = sf::Vector2f(0, TILE_SIZE + verticalOffset);

			// Position ready for the next four vertices
			currentVertex = currentVertex + VERTS_IN_QUAD;
		}
	}

	m_indices = arrayLevel;

	return arrayLevel;
}

sf::Vector2i Map::getLevelSize() const
{
	return m_LevelSize;
}

int Map::getIndex(int x, int y) const
{
	return m_indices[y][x];
}