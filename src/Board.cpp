#include "Board.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "Thor/Math.hpp"
#include "Thor/Graphics.hpp"

using namespace thor;
using namespace sf;

Board::Board(sf::RenderWindow* pWindow):m_pPickedFrameNode(nullptr)
{
	m_pWindow = pWindow;
	init();
}
Board::~Board()
{

}

bool Board::init()
{
	//init variables
	m_pPickedFrameNode = NULL;
	m_fPickedElementRotatedAngle = 0.0f;

	//set board state
	m_boardState = BBSTATE_BOARD_IDLE;

	//set board size
	m_boardSizeInfo.setSize(BOARD_SIZE_WHOLE_RADIUS, BOARD_ELEMENT_OUTLINE_NUM, BOARD_ELEMENT_INLINE_NUM);

	//set frame nodes position
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		float x = m_boardSizeInfo.outlineRad * sinf(2 * Pi / 16 * i);
		float y = m_boardSizeInfo.outlineRad * cosf(2 * Pi / 16 * i);
		m_boardFrame.lstOutlineFrameNodes[i].nodePos = Vector2f(x, y);
		m_boardFrame.lstOutlineFrameNodes[i].nodeRad = m_boardSizeInfo.outlineElementRad;
	}
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		float x = m_boardSizeInfo.inlineRad * sinf(2 * Pi / 8 * i);
		float y = m_boardSizeInfo.inlineRad * cosf(2 * Pi / 8 * i);
		m_boardFrame.lstInlineFrameNodes[i].nodePos = Vector2f(x, y);
		m_boardFrame.lstInlineFrameNodes[i].nodeRad = m_boardSizeInfo.inlineElementRad;
	}
	m_boardFrame.centerNode.nodePos = Vector2f(0, 0);
	m_boardFrame.centerNode.nodeRad = m_boardSizeInfo.centerElementRad;

	//set elements to frame nodes
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		BoardElement element;
		element.setElementType(BOARDELEMENTTYPE_NORMAL);
		element.pos = m_boardFrame.lstOutlineFrameNodes[i].nodePos;
		element.rad = m_boardFrame.lstOutlineFrameNodes[i].nodeRad;
		m_boardFrame.lstOutlineFrameNodes[i].value = element;
	}
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		BoardElement element;
		element.setElementType(BOARDELEMENTTYPE_NORMAL);
		element.pos = m_boardFrame.lstInlineFrameNodes[i].nodePos;
		element.rad = m_boardFrame.lstInlineFrameNodes[i].nodeRad;
		m_boardFrame.lstInlineFrameNodes[i].value = element;
	}
	BoardElement element;
	element.setElementType(BOARDELEMENTTYPE_NORMAL);
	element.pos = m_boardFrame.centerNode.nodePos;
	element.rad = m_boardFrame.centerNode.nodeRad;
	m_boardFrame.centerNode.value = element;

	return true;
}


void Board::setPosition(sf::Vector2f pos)
{
	Vector2f dPos = pos - this->m_vPos;
	this->m_vPos = pos;
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		m_boardFrame.lstOutlineFrameNodes[i].nodePos += dPos;
	}
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		m_boardFrame.lstInlineFrameNodes[i].nodePos += dPos;
	}
	m_boardFrame.centerNode.nodePos += dPos;
	
	resetBoardElementsPosition();
}
void Board::resetBoardElementsPosition()
{
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		m_boardFrame.lstOutlineFrameNodes[i].value.pos = m_boardFrame.lstOutlineFrameNodes[i].nodePos;
	}
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		m_boardFrame.lstInlineFrameNodes[i].value.pos = m_boardFrame.lstInlineFrameNodes[i].nodePos;
	}
	m_boardFrame.centerNode.value.pos = m_boardFrame.centerNode.nodePos;
}

void Board::smoothReturnBoardOutLine()
{
	Vector2f rotatedPos;
	float dAngle = 0.0f;
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		Vector2f currentElementPos	= m_boardFrame.lstOutlineFrameNodes[i].value.pos - this->m_vPos;
		Vector2f frameNodePos		= m_boardFrame.lstOutlineFrameNodes[i].nodePos - this->m_vPos;
		dAngle = thor::polarAngle(frameNodePos) - thor::polarAngle(currentElementPos);
		//prevent rotate whole board
		if (dAngle > 180.0f) dAngle -= 360.0f; else if (dAngle < -180.0f) dAngle += 360.0f;
		rotatedPos = thor::rotatedVector(	m_boardFrame.lstOutlineFrameNodes[i].value.pos - this->m_vPos,
											dAngle * BOARD_PARAM_RETURNING_SPEED);
		m_boardFrame.lstOutlineFrameNodes[i].value.pos = rotatedPos + this->m_vPos;
	}
}
void Board::smoothReturnBoardInLine()
{
	Vector2f rotatedPos;
	float dAngle = 0.0f;
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		Vector2f currentElementPos = m_boardFrame.lstInlineFrameNodes[i].value.pos - this->m_vPos;
		Vector2f frameNodePos = m_boardFrame.lstInlineFrameNodes[i].nodePos - this->m_vPos;
		dAngle = thor::polarAngle(frameNodePos) - thor::polarAngle(currentElementPos);
		//prevent rotate whole board
		if (dAngle > 180.0f) dAngle -= 360.0f; else if (dAngle < -180.0f) dAngle += 360.0f;
		rotatedPos = thor::rotatedVector(	m_boardFrame.lstInlineFrameNodes[i].value.pos - this->m_vPos,
											dAngle * BOARD_PARAM_RETURNING_SPEED);
		m_boardFrame.lstInlineFrameNodes[i].value.pos = rotatedPos + this->m_vPos;
	}
}
void Board::smoothReturnBoardCenter()
{
	//to do
}
void Board::rotateBoardOutLine(float dAngle)
{
	Vector2f rotatedPos;
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		rotatedPos = thor::rotatedVector(m_boardFrame.lstOutlineFrameNodes[i].nodePos - this->m_vPos, dAngle);
		m_boardFrame.lstOutlineFrameNodes[i].value.pos = rotatedPos + this->m_vPos;
	}
}
void Board::rotateBoardInLine(float dAngle)
{
	Vector2f rotatedPos;
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		rotatedPos = thor::rotatedVector(m_boardFrame.lstInlineFrameNodes[i].nodePos - this->m_vPos, dAngle);
		m_boardFrame.lstInlineFrameNodes[i].value.pos = rotatedPos + this->m_vPos;
	}
}
/************************************************************************************************/
/*********************************R E N D E R I N G**********************************************/
/************************************************************************************************/


void Board::render()
{
	renderBoardElements(m_pWindow);
}

void Board::renderBoardElements(sf::RenderWindow* pWindow)
{
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		m_boardFrame.lstOutlineFrameNodes[i].value.render(pWindow);
	}
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		m_boardFrame.lstInlineFrameNodes[i].value.render(pWindow);
	}
	m_boardFrame.centerNode.value.render(pWindow);
}

/************************************************************************************************/
/*********************************UPDATE METHODS*************************************************/
/************************************************************************************************/

void Board::update()
{
	m_bTouching = sf::Mouse::isButtonPressed(sf::Mouse::Left);
	if (m_bTouching) m_vTouchPoint = (Vector2f)sf::Mouse::getPosition(*m_pWindow);

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
void Board::update_idle()
{
	//update elements position
	//need to add smooth move animation

	//check if picked up
	if (m_bTouching)
	{
		bool picked = false;
		if (m_boardState == BBSTATE_BOARD_IDLE)
		{
			for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
			{
				if (m_boardFrame.lstOutlineFrameNodes[i].containsPoint(m_vTouchPoint))
				{
					m_pPickedFrameNode = &m_boardFrame.lstOutlineFrameNodes[i];
					picked = true;
					break;
				}
			}
			if (!picked)
				for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
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
			}
		}
	}

	//smoothing animation when drop down elements
	smoothReturnBoardOutLine();
	smoothReturnBoardInLine();
	smoothReturnBoardCenter();
}
void Board::update_pickup()
{
	if (m_bTouching)
	{
		Vector2f originToTouch = m_vTouchPoint;
		float originAngle = thor::polarAngle(m_pPickedFrameNode->nodePos - this->m_vPos);
		float movedAngle = thor::polarAngle(originToTouch - this->m_vPos);
		float dAngle = movedAngle - originAngle;

		Vector2f nodeToTouch = m_vTouchPoint - m_pPickedFrameNode->nodePos;
		if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_OUTLINE)
		{
			//if ((nodeToTouch.getAngle() > Pi_4 && nodeToTouch.getAngle() < 3*Pi_4) || (nodeToTouch.getAngle() > Pi_2 + Pi_4 && nodeToTouch.getAngle() < Pi) )
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
			smoothReturnBoardInLine();
			smoothReturnBoardOutLine();
		}
	}
	else
	{
		//check whether moved elements
		m_boardState = BBSTATE_BOARD_IDLE;
	}
}
void Board::update_rotate()
{

}
void Board::update_roll()
{

}
void Board::update_check_match()
{

}
void Board::update_matching()
{

}
void Board::update_supply()

{

}

/************************************************************************************************/
/************************************************************************************************/
/************************************************************************************************/
