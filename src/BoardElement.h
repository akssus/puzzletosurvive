#ifndef __BOARD_ELEMENT_H__
#define __BOARD_ELEMENT_H__
#include "SFML/Graphics.hpp"

enum ElementType
{
	BOARDELEMENTTYPE_EMPTY,
	BOARDELEMENTTYPE_NORMAL,
};


class BoardElement
{
public:
	BoardElement();
	~BoardElement();

public:
	void setElementType(ElementType type);
	const ElementType getType();
	void render(sf::RenderWindow* pWindow);
public:
	float				rad;
	sf::Vector2f		pos;

private:
	ElementType			m_type;
	sf::Sprite			m_sprite;
};



#endif