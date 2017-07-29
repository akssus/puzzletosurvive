#include "BoardElement.h"
#include "SpriteManager.h"

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
		m_sprite = SpriteManager::getInstance()->createSprite("res/ball.png");
		break;
	case BOARDELEMENTTYPE_NORMAL:
		m_sprite = SpriteManager::getInstance()->createSprite("res/ball.png");
		m_sprite.setColor(sf::Color(255, 0, 0));
		break;
	case BOARDELEMENTTYPE_1:
		m_sprite = SpriteManager::getInstance()->createSprite("res/ball.png");
		m_sprite.setColor(sf::Color(0, 255, 0));
		break;
	case BOARDELEMENTTYPE_2:
		m_sprite = SpriteManager::getInstance()->createSprite("res/ball.png");
		m_sprite.setColor(sf::Color(0, 0, 255));
		break;
	case BOARDELEMENTTYPE_3:
		m_sprite = SpriteManager::getInstance()->createSprite("res/ball.png");
		m_sprite.setColor(sf::Color(255, 255, 0));
		break;
	case BOARDELEMENTTYPE_4:
		m_sprite = SpriteManager::getInstance()->createSprite("res/ball.png");
		m_sprite.setColor(sf::Color(255, 0, 255));
		break;
	case BOARDELEMENTTYPE_5:
		m_sprite = SpriteManager::getInstance()->createSprite("res/ball.png");
		m_sprite.setColor(sf::Color(0, 255, 255));
		break;
	case BOARDELEMENTTYPE_6:
		m_sprite = SpriteManager::getInstance()->createSprite("res/ball.png");
		m_sprite.setColor(sf::Color(255, 255, 255));
		break;
	}
	m_sprite.setOrigin(	m_sprite.getLocalBounds().width*0.5,
						m_sprite.getLocalBounds().height*0.5);
	m_type = type;
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

}
void BoardElement::render(sf::RenderWindow* pWindow)
{
	m_sprite.setPosition(this->pos);
	if(this->m_type != BOARDELEMENTTYPE_EMPTY)
		pWindow->draw(m_sprite);
}