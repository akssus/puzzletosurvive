#include "BoardElement.h"
#include "SpriteManager.h"

using namespace sf;

BoardElement::BoardElement() : rad(0.0f), m_type(BOARDELEMENTTYPE_EMPTY),isRotated(false)
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
		break;
	case BOARDELEMENTTYPE_NORMAL:
		m_sprite = SpriteManager::getInstance()->createSprite("res/ball.png");
		break;
	}
	m_sprite.setOrigin(	m_sprite.getLocalBounds().width*0.5,
						m_sprite.getLocalBounds().height*0.5);
}

void BoardElement::setSize(float width, float height)
{
	Vector2u tSize = m_sprite.getTexture()->getSize();
	m_sprite.setScale(width / tSize.x, height / tSize.y);
}

const ElementType BoardElement::getType()
{
	return m_type;
}

void BoardElement::render(sf::RenderWindow* pWindow)
{
	m_sprite.setPosition(this->pos);
	pWindow->draw(m_sprite);
}