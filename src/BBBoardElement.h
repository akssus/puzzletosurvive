#ifndef __BBBOARD_ELEMENT_H__
#define __BBBOARD_ELEMENT_H__

#include "cocos2d.h"

enum ElementType
{
	BBELEMENTTYPE_NORMAL,
};


class BBBoardElement : public cocos2d::Sprite
{
public:
	//static cocos2d::Sprite*	createSprite();
	virtual bool	init();
	void			initWithElementType(ElementType type);
	CREATE_FUNC(BBBoardElement);


public:
	float					m_fRad;
	cocos2d::Vec2			m_vPos;

private:
	cocos2d::Sprite*		m_pSprite;
};

#endif