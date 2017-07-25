#include "BBBoard.h"
#include "BBParameters.h"
#include "cocos2d.h"

USING_NS_CC;

bool BBBoard::init()
{
	//init variables
	m_pPickedFrameNode = NULL;
	m_fPickedElementRotatedAngle = 0.0f;

	//set board state
	m_boardState = BBSTATE_BOARD_IDLE;

	//add touch event
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(BBBoard::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(BBBoard::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(BBBoard::onTouchEnded, this);

	auto dispatcher = this->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//set board size
	m_boardSizeInfo.setSize(BBSIZE_BOARD_WHOLE_RADIUS, BB_ELEMENT_OUTLINE_NUM, BB_ELEMENT_INLINE_NUM);
	
	//set frame nodes position
	for (int i = 0; i < BB_ELEMENT_OUTLINE_NUM; ++i)
	{
		float x = m_boardSizeInfo.outlineRad * sinf(2*M_PI / 16 * i);
		float y = m_boardSizeInfo.outlineRad * cosf(2*M_PI / 16 * i);
		m_boardFrame.lstOutlineFrameNodes[i].nodePos = Vec2(x, y);
		m_boardFrame.lstOutlineFrameNodes[i].nodeRad = m_boardSizeInfo.outlineElementRad;
	}
	for (int i = 0; i < BB_ELEMENT_INLINE_NUM; ++i)
	{
		float x = m_boardSizeInfo.inlineRad * sinf(2*M_PI / 8 * i);
		float y = m_boardSizeInfo.inlineRad * cosf(2*M_PI / 8 * i);
		m_boardFrame.lstInlineFrameNodes[i].nodePos = Vec2(x, y);
		m_boardFrame.lstInlineFrameNodes[i].nodeRad = m_boardSizeInfo.inlineElementRad;
	}
	m_boardFrame.centerNode.nodePos = Vec2(0, 0);
	m_boardFrame.centerNode.nodeRad = m_boardSizeInfo.centerElementRad;

	//set elements to frame nodes
	for (int i = 0; i < BB_ELEMENT_OUTLINE_NUM; ++i)
	{
		BBBoardElement* pElement = BBBoardElement::create();
		pElement->initWithElementType(BBELEMENTTYPE_NORMAL);
		pElement->m_vPos = m_boardFrame.lstOutlineFrameNodes[i].nodePos;
		pElement->m_fRad = m_boardFrame.lstOutlineFrameNodes[i].nodeRad;
		pElement->setPosition(pElement->m_vPos);
		m_boardFrame.lstOutlineFrameNodes[i].value = pElement;
	}
	for (int i = 0; i < BB_ELEMENT_INLINE_NUM; ++i)
	{
		BBBoardElement* pElement = BBBoardElement::create();
		pElement->initWithElementType(BBELEMENTTYPE_NORMAL);
		pElement->m_vPos = m_boardFrame.lstInlineFrameNodes[i].nodePos;
		pElement->m_fRad = m_boardFrame.lstInlineFrameNodes[i].nodeRad;
		pElement->setPosition(pElement->m_vPos);
		m_boardFrame.lstInlineFrameNodes[i].value = pElement;
	}
	BBBoardElement* pElement = BBBoardElement::create();
	pElement->initWithElementType(BBELEMENTTYPE_NORMAL);
	pElement->m_vPos = m_boardFrame.centerNode.nodePos;
	pElement->m_fRad = m_boardFrame.centerNode.nodeRad;
	pElement->setPosition(pElement->m_vPos);
	m_boardFrame.centerNode.value = pElement;

	//add to board layer
	for (int i = 0; i < BB_ELEMENT_OUTLINE_NUM; ++i)
	{
		this->addChild(m_boardFrame.lstOutlineFrameNodes[i].value);
	}
	for (int i = 0; i < BB_ELEMENT_INLINE_NUM; ++i)
	{
		this->addChild(m_boardFrame.lstInlineFrameNodes[i].value);
	}
	this->addChild(m_boardFrame.centerNode.value);


	return true;
}


void BBBoard::reposeBoardElements()
{
	for (int i = 0; i < BB_ELEMENT_OUTLINE_NUM; ++i)
	{
		m_boardFrame.lstOutlineFrameNodes[i].value->setPosition(m_boardFrame.lstOutlineFrameNodes[i].value->m_vPos);
	}
	for (int i = 0; i < BB_ELEMENT_INLINE_NUM; ++i)
	{
		m_boardFrame.lstInlineFrameNodes[i].value->setPosition(m_boardFrame.lstInlineFrameNodes[i].value->m_vPos);
	}
	m_boardFrame.centerNode.value->setPosition(m_boardFrame.centerNode.value->m_vPos);
}

void BBBoard::resetBoardElementsPosition()
{
	for (int i = 0; i < BB_ELEMENT_OUTLINE_NUM; ++i)
	{
		m_boardFrame.lstOutlineFrameNodes[i].value->m_vPos = m_boardFrame.lstOutlineFrameNodes[i].nodePos;
	}
	for (int i = 0; i < BB_ELEMENT_INLINE_NUM; ++i)
	{
		m_boardFrame.lstInlineFrameNodes[i].value->m_vPos = m_boardFrame.lstInlineFrameNodes[i].nodePos;
	}
	m_boardFrame.centerNode.value->m_vPos = m_boardFrame.centerNode.nodePos;

	reposeBoardElements();
}

void BBBoard::smoothReturnBoardOutLine()
{
	Vec2 rotatedPos;
	float dAngle = 0.0f;
	for (int i = 0; i < BB_ELEMENT_OUTLINE_NUM; ++i)
	{
		Vec2 currentElementPos = m_boardFrame.lstOutlineFrameNodes[i].value->m_vPos;
		Vec2 frameNodePos = m_boardFrame.lstOutlineFrameNodes[i].nodePos;
		dAngle = frameNodePos.getAngle()-currentElementPos.getAngle();
		//prevent rotate whole board
		if (dAngle > M_PI) dAngle -= 2 * M_PI; else if (dAngle < -M_PI) dAngle += 2 * M_PI;
		rotatedPos = m_boardFrame.lstOutlineFrameNodes[i].value->m_vPos.rotateByAngle(Vec2(0, 0), dAngle * BBPARAM_BOARD_RETURNING_SPEED);
		m_boardFrame.lstOutlineFrameNodes[i].value->m_vPos = rotatedPos;
	}
}
void BBBoard::smoothReturnBoardInLine()
{
	Vec2 rotatedPos;
	float dAngle = 0.0f;
	for (int i = 0; i < BB_ELEMENT_INLINE_NUM; ++i)
	{
		Vec2 currentElementPos = m_boardFrame.lstInlineFrameNodes[i].value->m_vPos;
		Vec2 frameNodePos = m_boardFrame.lstInlineFrameNodes[i].nodePos;
		dAngle = frameNodePos.getAngle() - currentElementPos.getAngle();
		//prevent rotate whole board
		if (dAngle > M_PI) dAngle -= 2 * M_PI; else if (dAngle < -M_PI) dAngle += 2 * M_PI;
		rotatedPos = m_boardFrame.lstInlineFrameNodes[i].value->m_vPos.rotateByAngle(Vec2(0, 0), dAngle * BBPARAM_BOARD_RETURNING_SPEED);
		m_boardFrame.lstInlineFrameNodes[i].value->m_vPos = rotatedPos;
	}
}
void BBBoard::smoothReturnBoardCenter()
{
	//to do
}
void BBBoard::rotateBoardOutLine(float dAngle)
{
	Vec2 rotatedPos;
	for (int i = 0; i < BB_ELEMENT_OUTLINE_NUM; ++i)
	{
		rotatedPos = m_boardFrame.lstOutlineFrameNodes[i].nodePos.rotateByAngle(Vec2(0, 0), dAngle);
		m_boardFrame.lstOutlineFrameNodes[i].value->m_vPos = rotatedPos;
	}
}
void BBBoard::rotateBoardInLine(float dAngle)
{
	Vec2 rotatedPos;
	for (int i = 0; i < BB_ELEMENT_INLINE_NUM; ++i)
	{
		rotatedPos = m_boardFrame.lstInlineFrameNodes[i].nodePos.rotateByAngle(Vec2(0, 0), dAngle);
		m_boardFrame.lstInlineFrameNodes[i].value->m_vPos = rotatedPos;
	}
}


/************************************************************************************************/
/*********************************UPDATE METHODS*************************************************/
/************************************************************************************************/
void BBBoard::update()
{
	switch (m_boardState)
	{
	case BBSTATE_BOARD_IDLE: update_idle(); break;
	case BBSTATE_BOARD_PICKUP: update_pickup(); break;
	case BBSTATE_BOARD_ROTATE: update_rotate(); break;
	case BBSTATE_BOARD_ROLL: update_roll(); break;
	case BBSTATE_BOARD_CHECK_MATCH: update_check_match(); break;
	case BBSTATE_BOARD_SUPPLY: update_supply(); break;
	}
}
void BBBoard::update_idle()
{
	//update elements position
	//need to add smooth move animation

	//check if picked up
	if (m_bTouching)
	{
		bool picked = false;
		if (m_boardState == BBSTATE_BOARD_IDLE)
		{
			for (int i = 0; i < BB_ELEMENT_OUTLINE_NUM; ++i)
			{
				if (m_boardFrame.lstOutlineFrameNodes[i].containsPoint(m_vTouchPoint))
				{
					m_pPickedFrameNode = &m_boardFrame.lstOutlineFrameNodes[i];
					picked = true;
					break;
				}
			}
			if (!picked)
				for (int i = 0; i < BB_ELEMENT_INLINE_NUM; ++i)
				{
					if (m_boardFrame.lstInlineFrameNodes[i].containsPoint(m_vTouchPoint))
					{
						m_pPickedFrameNode = &m_boardFrame.lstInlineFrameNodes[i];
						picked = true;
						break;
					}
				}
			if (!picked)
				if (m_boardFrame.centerNode.containsPoint(m_vTouchPoint))
				{
					m_pPickedFrameNode = &m_boardFrame.centerNode;
					picked = true;
				}
			if (picked) {
				//resetBoardElementsPosition();
				m_boardState = BBSTATE_BOARD_PICKUP;
				log("touched %d,%d\n", m_pPickedFrameNode->nodeID, m_pPickedFrameNode->nodeLine);
			}
		}
	}

	//smoothing animation when drop down elements
	smoothReturnBoardOutLine();
	smoothReturnBoardInLine();
	smoothReturnBoardCenter();

	reposeBoardElements();
}
void BBBoard::update_pickup()
{
	if (m_bTouching)
	{
		Vec2 originToTouch = m_vTouchPoint;
		float originAngle = m_pPickedFrameNode->nodePos.getAngle();
		float movedAngle = originToTouch.getAngle();
		float dAngle = movedAngle - originAngle;
		
		Vec2 nodeToTouch = m_vTouchPoint - m_pPickedFrameNode->nodePos;
		if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_OUTLINE)
		{
			//if ((nodeToTouch.getAngle() > M_PI_4 && nodeToTouch.getAngle() < 3*M_PI_4) || (nodeToTouch.getAngle() > M_PI_2 + M_PI_4 && nodeToTouch.getAngle() < M_PI) )
			//{
				rotateBoardOutLine(dAngle);
				smoothReturnBoardInLine();
			//}
		}
		else if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_INLINE)
		{
			rotateBoardInLine(dAngle);
			smoothReturnBoardOutLine();

		}
		else //center
		{

		}
		reposeBoardElements();
	}
	else
	{
		//check whether moved elements
		m_boardState = BBSTATE_BOARD_IDLE;
	}
}
void BBBoard::update_rotate()
{

}
void BBBoard::update_roll()
{

}
void BBBoard::update_check_match()
{

}
void BBBoard::update_matching()
{

}
void BBBoard::update_supply()

{

}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/






bool BBBoard::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
	auto touchPoint = touch->getLocation() - this->getPosition();
	m_vTouchPoint = touchPoint;
	m_bTouching = true;
	return true;
}
void BBBoard::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
	auto touchPoint = touch->getLocation() - this->getPosition();
	m_vTouchPoint = touchPoint;

}
void BBBoard::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
	m_bTouching = false;
}