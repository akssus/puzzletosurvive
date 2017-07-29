#include "BoardElement.h"
#include "SpriteManager.h"
#include "Thor/Math.hpp"

using namespace sf;

BoardElement::BoardElement() : rad(0.0f), m_type(BOARDELEMENTTYPE_EMPTY),isRotated(false),isDestroyed(false)
{

}
BoardElement::~BoardElement()
{

}

void BoardElement::setElementType(ElementType type)
{
	switch (type)
	{
	case BOARDELEMENTTYPE_EMPTY:
		m_sprite = SpriteManager::getInstance()->createSprite("res/node.png");
		break;
	case BOARDELEMENTTYPE_NORMAL:
		m_sprite = SpriteManager::getInstance()->createSprite("res/node.png");
		m_sprite.setColor(sf::Color(255, 0, 0));
		break;
	case BOARDELEMENTTYPE_1:
		m_sprite = SpriteManager::getInstance()->createSprite("res/node.png");
		m_sprite.setColor(sf::Color(0, 255, 0));
		break;
	case BOARDELEMENTTYPE_2:
		m_sprite = SpriteManager::getInstance()->createSprite("res/node.png");
		m_sprite.setColor(sf::Color(0, 0, 255));
		break;
	case BOARDELEMENTTYPE_3:
		m_sprite = SpriteManager::getInstance()->createSprite("res/node.png");
		m_sprite.setColor(sf::Color(255, 255, 0));
		break;
	case BOARDELEMENTTYPE_4:
		m_sprite = SpriteManager::getInstance()->createSprite("res/node.png");
		m_sprite.setColor(sf::Color(255, 0, 255));
		break;
	case BOARDELEMENTTYPE_5:
		m_sprite = SpriteManager::getInstance()->createSprite("res/node.png");
		m_sprite.setColor(sf::Color(0, 255, 255));
		break;
	case BOARDELEMENTTYPE_6:
		m_sprite = SpriteManager::getInstance()->createSprite("res/node.png");
		m_sprite.setColor(sf::Color(255, 255, 255));
		break;
	case BOARDELEMENTTYPE_7:
		m_sprite = SpriteManager::getInstance()->createSprite("res/node.png");
		m_sprite.setColor(sf::Color(100, 100, 125));
		break;
	}
	m_sprite.setOrigin(	m_sprite.getLocalBounds().width*0.5,
						m_sprite.getLocalBounds().height*0.5);
	m_type = type;
}

void BoardElement::setWithRandomType()
{
	ElementType randomType = (ElementType)thor::random(1, BOARDELEMENTTYPE_TOTAL_COUNT - 1);
	setElementType(randomType);
}

void BoardElement::setSize(float width, float height)
{
	Vector2u tSize = m_sprite.getTexture()->getSize();
	m_sprite.setScale(width / tSize.x, height / tSize.y);
}
void BoardElement::setRad(float rad)
{
	this->rad = rad;
}
float BoardElement::getRad()
{
	return rad;
}

const ElementType BoardElement::getType()
{
	return m_type;
}

void BoardElement::destroy()
{
	//simple animation
	sf::Color color = m_sprite.getColor();
	int alpha = color.a;
	alpha -= 5;
	if (alpha < 0)
	{
		alpha = 0;
		isDestroyed = true;
		this->setElementType(BOARDELEMENTTYPE_EMPTY);
	}
	color.a = alpha;
	m_sprite.setColor(color);
}
void BoardElement::render(sf::RenderWindow* pWindow)
{
	m_sprite.setPosition(this->pos);
	if(this->m_type != BOARDELEMENTTYPE_EMPTY)
		pWindow->draw(m_sprite);
}