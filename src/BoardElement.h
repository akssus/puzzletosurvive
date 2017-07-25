#pragma once 

#include "MNL.h"

enum ElementType
{
	BBELEMENTTYPE_NORMAL,
};


class BBBoardElement : public MNSprite
{
public:
	//static cocos2d::Sprite*	createSprite();
	virtual bool	init();
	void			initWithElementType(ElementType type);

public:
	float					m_fRad;
	cocos2d::Vec2			m_vPos;

private:
	cocos2d::Sprite*		m_pSprite;
};
