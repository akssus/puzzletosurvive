#include "BBBoardElement.h"

USING_NS_CC;

bool BBBoardElement::init()
{
	m_fRad = 0.0f;
	m_pSprite = NULL;
	return true;
}

void BBBoardElement::initWithElementType(ElementType type)
{
	Sprite* newSprite = NULL;
	switch (type)
	{
	case BBELEMENTTYPE_NORMAL:
		newSprite = Sprite::create("CloseNormal.png");
	}
	this->addChild(newSprite);
}