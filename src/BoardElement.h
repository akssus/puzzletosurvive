#ifndef __BOARD_ELEMENT_H__
#define __BOARD_ELEMENT_H__
#include "SFML/Graphics.hpp"

enum ElementType
{
	BOARDELEMENTTYPE_EMPTY,
	BOARDELEMENTTYPE_NORMAL,
	BOARDELEMENTTYPE_1,
	BOARDELEMENTTYPE_2,
	BOARDELEMENTTYPE_3,
	BOARDELEMENTTYPE_4,
	BOARDELEMENTTYPE_5,
	BOARDELEMENTTYPE_6,
	BOARDELEMENTTYPE_7,
	BOARDELEMENTTYPE_TOTAL_COUNT,
};


class BoardElement
{
public:
	BoardElement();
	~BoardElement();

public:
	void				setElementType(ElementType type);
	void				setSize(float width, float height);
	float				getRad();
	void				setRad(float rad);
	const ElementType	getType();
	void				setWithRandomType();

	void destroy();
	void render(sf::RenderWindow* pWindow);
public:
	sf::Vector2f		pos;
	bool				isRotated;
	bool				isDestroyed;
private:
	float				rad;
	ElementType			m_type;
	sf::Sprite			m_sprite;
};



#endif