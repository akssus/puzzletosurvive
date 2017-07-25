#include "SpriteManager.h"

SpriteManager* SpriteManager::m_pInstance = nullptr;

SpriteManager::SpriteManager()
{

}
SpriteManager::~SpriteManager()
{
	freeInstance();
}

void SpriteManager::freeInstance()
{
	m_textureTable.clear();

	if (m_pInstance != nullptr)
	{
		delete m_pInstance;
	}
	m_pInstance = nullptr;
}

sf::Sprite SpriteManager::createSprite(std::string imageFilePath)
{
	sf::Sprite		newSprite;
	sf::Texture*	newTexture;
	bool textureLoadSuccess = true;
	//check whether already loaded texture
	if (m_textureTable.count(imageFilePath) == 0)
	{
		newTexture = new sf::Texture();
		textureLoadSuccess = newTexture->loadFromFile(imageFilePath);
		if (!textureLoadSuccess)
		{
			delete newTexture;
			//return as default sprite
		}
		else
		{
			newSprite.setTexture(*newTexture);
			m_textureTable[imageFilePath] = std::shared_ptr<sf::Texture>(newTexture);
		}
	}
	else
	{
		newTexture = m_textureTable[imageFilePath].get();
		newSprite.setTexture(*newTexture);
	}
	return newSprite;
}