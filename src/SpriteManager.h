#ifndef __SPRITE_MANAGER_H__
#define __SPRITE_MANAGER_H__
#include <map>
#include <memory>
#include <string>
#include "SFML/Graphics.hpp"

class SpriteManager 
{
public:
	~SpriteManager();
private:
	SpriteManager();

public:
	static SpriteManager* getInstance()
	{
		if (m_pInstance == nullptr)
		{
			m_pInstance = new SpriteManager();
		}
		return m_pInstance;
	};
	sf::Sprite createSprite(std::string imageFilePath);

private:
	void freeInstance();

public:
	

private:
	static SpriteManager* m_pInstance;
	std::map<std::string, std::shared_ptr<sf::Texture> > m_textureTable;

};



#endif
