#include "Board.h"
#include "SFML/Graphics.hpp"
#include "SFML/System.hpp"
#include "Thor/Math.hpp"
#include "Thor/Graphics.hpp"

using namespace thor;
using namespace sf;

Board::Board(sf::RenderWindow* pWindow):m_pPickedFrameNode(nullptr),m_iCenterRollTo(0)
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
	m_boardState = BOARD_STATE_IDLE;

	//set board size
	m_boardSizeInfo.setSize(BOARD_SIZE_WHOLE_RADIUS, BOARD_ELEMENT_OUTLINE_NUM, BOARD_ELEMENT_INLINE_NUM);

	//set frame nodes position
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		float x = m_boardSizeInfo.outlineRad * cosf((2.0f * Pi / 16 * i) - (0.5f*Pi));
		float y = m_boardSizeInfo.outlineRad * sinf((2.0f * Pi / 16 * i) - (0.5f*Pi));
		m_boardFrame.lstOutlineFrameNodes[i].nodePos = Vector2f(x, y);
		m_boardFrame.lstOutlineFrameNodes[i].nodeRad = m_boardSizeInfo.outlineElementRad;
	}
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		float x = m_boardSizeInfo.inlineRad * cosf((2.0f * Pi / 8 * i) - (0.5f*Pi));
		float y = m_boardSizeInfo.inlineRad * sinf((2.0f * Pi / 8 * i) - (0.5f*Pi));
		m_boardFrame.lstInlineFrameNodes[i].nodePos = Vector2f(x, y);
		m_boardFrame.lstInlineFrameNodes[i].nodeRad = m_boardSizeInfo.inlineElementRad;
	}
	m_boardFrame.centerNode.nodePos = Vector2f(0, 0);
	m_boardFrame.centerNode.nodeRad = m_boardSizeInfo.centerElementRad;

	//set elements to frame nodes
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		BoardElement element;
		element.setElementType(BOARDELEMENTTYPE_EMPTY);
		element.pos = m_boardFrame.lstOutlineFrameNodes[i].nodePos;
		element.rad = m_boardFrame.lstOutlineFrameNodes[i].nodeRad;
		m_boardFrame.lstOutlineFrameNodes[i].value = element;
	}
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		BoardElement element;
		element.setElementType(BOARDELEMENTTYPE_EMPTY);
		element.pos = m_boardFrame.lstInlineFrameNodes[i].nodePos;
		element.rad = m_boardFrame.lstInlineFrameNodes[i].nodeRad;
		m_boardFrame.lstInlineFrameNodes[i].value = element;
	}
	BoardElement element;
	element.setElementType(BOARDELEMENTTYPE_EMPTY);
	element.pos = m_boardFrame.centerNode.nodePos;
	element.rad = m_boardFrame.centerNode.nodeRad;
	m_boardFrame.centerNode.value = element;


	shuffleBoardElements();
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
	resetBoardOutLineElementsPosition();
	resetBoardInLineElementsPosition();
	resetBoardCenterElementsPosition();
}
void Board::resetBoardOutLineElementsPosition()
{
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		m_boardFrame.lstOutlineFrameNodes[i].value.pos = m_boardFrame.lstOutlineFrameNodes[i].nodePos;
	}
}
void Board::resetBoardInLineElementsPosition()
{
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		m_boardFrame.lstInlineFrameNodes[i].value.pos = m_boardFrame.lstInlineFrameNodes[i].nodePos;
	}
}
void Board::resetBoardCenterElementsPosition()
{
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
		if (m_boardFrame.lstOutlineFrameNodes[i].value.isRotated)
		{
			dAngle = thor::polarAngle(frameNodePos) - thor::polarAngle(currentElementPos);
			//prevent rotate whole board
			if (dAngle > 180.0f) dAngle -= 360.0f; else if (dAngle < -180.0f) dAngle += 360.0f;
			rotatedPos = thor::rotatedVector(	m_boardFrame.lstOutlineFrameNodes[i].value.pos - this->m_vPos,
												dAngle * BOARD_PARAM_RETURNING_SPEED);
			m_boardFrame.lstOutlineFrameNodes[i].value.pos = rotatedPos + this->m_vPos;
		}
		else
		{
			Vector2f dir = frameNodePos - currentElementPos;
			m_boardFrame.lstOutlineFrameNodes[i].value.pos += dir* BOARD_PARAM_RETURNING_SPEED;
		}
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
		if (m_boardFrame.lstInlineFrameNodes[i].value.isRotated)
		{
			dAngle = thor::polarAngle(frameNodePos) - thor::polarAngle(currentElementPos);
			//prevent rotate whole board
			if (dAngle > 180.0f) dAngle -= 360.0f; else if (dAngle < -180.0f) dAngle += 360.0f;
			rotatedPos = thor::rotatedVector(m_boardFrame.lstInlineFrameNodes[i].value.pos - this->m_vPos,
				dAngle * BOARD_PARAM_RETURNING_SPEED);
			m_boardFrame.lstInlineFrameNodes[i].value.pos = rotatedPos + this->m_vPos;
		}
		else
		{
			Vector2f dir = frameNodePos - currentElementPos;
			m_boardFrame.lstInlineFrameNodes[i].value.pos += dir* BOARD_PARAM_RETURNING_SPEED;
		}
	}
}
void Board::smoothReturnBoardCenter()
{
	Vector2f rotatedPos;
	float dAngle = 0.0f;

	Vector2f currentElementPos = m_boardFrame.centerNode.value.pos - this->m_vPos;
	Vector2f frameNodePos = m_boardFrame.centerNode.nodePos - this->m_vPos;
	if (m_boardFrame.centerNode.value.isRotated)
	{
		dAngle = thor::polarAngle(frameNodePos) - thor::polarAngle(currentElementPos);
		//prevent rotate whole board
		if (dAngle > 180.0f) dAngle -= 360.0f; else if (dAngle < -180.0f) dAngle += 360.0f;
		rotatedPos = thor::rotatedVector(m_boardFrame.centerNode.value.pos - this->m_vPos,
			dAngle * BOARD_PARAM_RETURNING_SPEED);
		m_boardFrame.centerNode.value.pos = rotatedPos + this->m_vPos;
	}
	else
	{
		Vector2f dir = frameNodePos - currentElementPos;
		m_boardFrame.centerNode.value.pos += dir* BOARD_PARAM_RETURNING_SPEED;
	}
}
void Board::rotateBoardOutLine(float dAngle)
{
	Vector2f rotatedPos;
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		m_boardFrame.lstOutlineFrameNodes[i].value.isRotated = true;
		rotatedPos = thor::rotatedVector(m_boardFrame.lstOutlineFrameNodes[i].nodePos - this->m_vPos, dAngle);
		m_boardFrame.lstOutlineFrameNodes[i].value.pos = rotatedPos + this->m_vPos;
	}
}
void Board::rotateBoardInLine(float dAngle)
{
	Vector2f rotatedPos;
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		m_boardFrame.lstInlineFrameNodes[i].value.isRotated = true;
		rotatedPos = thor::rotatedVector(m_boardFrame.lstInlineFrameNodes[i].nodePos - this->m_vPos, dAngle);
		m_boardFrame.lstInlineFrameNodes[i].value.pos = rotatedPos + this->m_vPos;
	}
}

void Board::rollBoardOutLine(unsigned int index, Vector2f dir)
{
	unsigned int olNodeIndex_1 = index;
	unsigned int olNodeIndex_2 = (olNodeIndex_1 + BOARD_ELEMENT_OUTLINE_NUM / 2) % BOARD_ELEMENT_OUTLINE_NUM;
	unsigned int ilNodeIndex_1 = olNodeIndex_1 / 2;
	unsigned int ilNodeIndex_2 = (ilNodeIndex_1 + BOARD_ELEMENT_INLINE_NUM / 2) % BOARD_ELEMENT_INLINE_NUM;

	BoardFrameNode& olFrame_1 = m_boardFrame.lstOutlineFrameNodes[olNodeIndex_1];
	BoardFrameNode& olFrame_2 = m_boardFrame.lstOutlineFrameNodes[olNodeIndex_2];
	BoardFrameNode& ilFrame_1 = m_boardFrame.lstInlineFrameNodes[ilNodeIndex_1];
	BoardFrameNode& ilFrame_2 = m_boardFrame.lstInlineFrameNodes[ilNodeIndex_2];
	BoardFrameNode& centerFrame = m_boardFrame.centerNode;

	olFrame_1.value.pos = olFrame_1.nodePos + dir;
	olFrame_2.value.pos = olFrame_2.nodePos + dir;
	ilFrame_1.value.pos = ilFrame_1.nodePos + dir;
	ilFrame_2.value.pos = ilFrame_2.nodePos + dir;
	centerFrame.value.pos = centerFrame.nodePos + dir;

	olFrame_1.value.isRotated = false;
	olFrame_2.value.isRotated = false;
	ilFrame_1.value.isRotated = false;
	ilFrame_2.value.isRotated = false;
	centerFrame.value.isRotated = false;
}

void Board::rollBoardInLine(unsigned int index, Vector2f dir)
{
	unsigned int ilNodeIndex_1 = index;
	unsigned int ilNodeIndex_2 = (ilNodeIndex_1 + BOARD_ELEMENT_INLINE_NUM / 2) % BOARD_ELEMENT_INLINE_NUM;
	unsigned int olNodeIndex_1 = ilNodeIndex_1 * 2;
	unsigned int olNodeIndex_2 = (olNodeIndex_1 + BOARD_ELEMENT_OUTLINE_NUM / 2) % BOARD_ELEMENT_OUTLINE_NUM;

	BoardFrameNode& olFrame_1 = m_boardFrame.lstOutlineFrameNodes[olNodeIndex_1];
	BoardFrameNode& olFrame_2 = m_boardFrame.lstOutlineFrameNodes[olNodeIndex_2];
	BoardFrameNode& ilFrame_1 = m_boardFrame.lstInlineFrameNodes[ilNodeIndex_1];
	BoardFrameNode& ilFrame_2 = m_boardFrame.lstInlineFrameNodes[ilNodeIndex_2];
	BoardFrameNode& centerFrame = m_boardFrame.centerNode;

	olFrame_1.value.pos = olFrame_1.nodePos + dir;
	olFrame_2.value.pos = olFrame_2.nodePos + dir;
	ilFrame_1.value.pos = ilFrame_1.nodePos + dir;
	ilFrame_2.value.pos = ilFrame_2.nodePos + dir;
	centerFrame.value.pos = centerFrame.nodePos + dir;

	olFrame_1.value.isRotated = false;
	olFrame_2.value.isRotated = false;
	ilFrame_1.value.isRotated = false;
	ilFrame_2.value.isRotated = false;
	centerFrame.value.isRotated = false;
}

void Board::rollBoardCenter(unsigned int toIndex, Vector2f dir)
{
	unsigned int ilNodeIndex_1 = toIndex;
	unsigned int ilNodeIndex_2 = (ilNodeIndex_1 + BOARD_ELEMENT_INLINE_NUM / 2) % BOARD_ELEMENT_INLINE_NUM;
	unsigned int olNodeIndex_1 = ilNodeIndex_1 * 2;
	unsigned int olNodeIndex_2 = (olNodeIndex_1 + BOARD_ELEMENT_OUTLINE_NUM / 2) % BOARD_ELEMENT_OUTLINE_NUM;

	BoardFrameNode& olFrame_1 = m_boardFrame.lstOutlineFrameNodes[olNodeIndex_1];
	BoardFrameNode& olFrame_2 = m_boardFrame.lstOutlineFrameNodes[olNodeIndex_2];
	BoardFrameNode& ilFrame_1 = m_boardFrame.lstInlineFrameNodes[ilNodeIndex_1];
	BoardFrameNode& ilFrame_2 = m_boardFrame.lstInlineFrameNodes[ilNodeIndex_2];
	BoardFrameNode& centerFrame = m_boardFrame.centerNode;

	olFrame_1.value.pos = olFrame_1.nodePos + dir;
	olFrame_2.value.pos = olFrame_2.nodePos + dir;
	ilFrame_1.value.pos = ilFrame_1.nodePos + dir;
	ilFrame_2.value.pos = ilFrame_2.nodePos + dir;
	centerFrame.value.pos = centerFrame.nodePos + dir;

	olFrame_1.value.isRotated = false;
	olFrame_2.value.isRotated = false;
	ilFrame_1.value.isRotated = false;
	ilFrame_2.value.isRotated = false;
	centerFrame.value.isRotated = false;
}

/************************************************************************************************/
/*********************************M E C H A N I C S**********************************************/
/************************************************************************************************/
void Board::shuffleBoardElements()
{
	//generate outline
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		bool typeFixed = false;
		while (!typeFixed)
		{
			ElementType randomType = (ElementType)random(1, BOARDELEMENTTYPE_TOTAL_COUNT-1);
			unsigned int prevNodeIndex = (i + BOARD_ELEMENT_OUTLINE_NUM - 1) % BOARD_ELEMENT_OUTLINE_NUM;
			unsigned int nextNodeIndex = (i + BOARD_ELEMENT_OUTLINE_NUM + 1) % BOARD_ELEMENT_OUTLINE_NUM;
			BoardFrameNode* pPrevNode = &m_boardFrame.lstOutlineFrameNodes[prevNodeIndex];
			BoardFrameNode* pNextNode = &m_boardFrame.lstOutlineFrameNodes[nextNodeIndex];
			ElementType prevNodeType = pPrevNode->value.getType();
			ElementType nextNodeType = pNextNode->value.getType();
			if (randomType == prevNodeType && randomType == nextNodeType) continue;
			else if (prevNodeType == randomType)
			{
				unsigned int prevPrevNodeIndex = (prevNodeIndex + BOARD_ELEMENT_OUTLINE_NUM - 1) % BOARD_ELEMENT_OUTLINE_NUM;
				BoardFrameNode* pPrevPrevNode = &m_boardFrame.lstOutlineFrameNodes[prevPrevNodeIndex];
				ElementType prevPrevNodeType = pPrevPrevNode->value.getType();
				if (prevPrevNodeType == randomType) continue;
			}
			else if (nextNodeType == randomType)
			{
				unsigned int nextNextNodeIndex = (nextNodeIndex + BOARD_ELEMENT_OUTLINE_NUM + 1) % BOARD_ELEMENT_OUTLINE_NUM;
				BoardFrameNode* pNextNextNode = &m_boardFrame.lstOutlineFrameNodes[nextNextNodeIndex];
				ElementType nextNextNodeType = pNextNextNode->value.getType();
				if (nextNextNodeType == randomType) continue;
			}
			//all condition matched
			typeFixed = true;
			m_boardFrame.lstOutlineFrameNodes[i].value.setElementType(randomType);
		}
	}
	//generate inline
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		bool typeFixed = false;
		while (!typeFixed)
		{
			ElementType randomType = (ElementType)random(1, BOARDELEMENTTYPE_TOTAL_COUNT-1);
			unsigned int symmericNodeIndex = (i + 4) % BOARD_ELEMENT_INLINE_NUM;
			BoardFrameNode* pSymmetricNode = &m_boardFrame.lstInlineFrameNodes[symmericNodeIndex];
			ElementType symmetricNodeType = pSymmetricNode->value.getType();
			if (symmetricNodeType == randomType) continue;

			unsigned int prevNodeIndex = (i + BOARD_ELEMENT_INLINE_NUM - 1) % BOARD_ELEMENT_INLINE_NUM;
			unsigned int nextNodeIndex = (i + BOARD_ELEMENT_INLINE_NUM + 1) % BOARD_ELEMENT_INLINE_NUM;
			BoardFrameNode* pPrevNode = &m_boardFrame.lstInlineFrameNodes[prevNodeIndex];
			BoardFrameNode* pNextNode = &m_boardFrame.lstInlineFrameNodes[nextNodeIndex];
			ElementType prevNodeType = pPrevNode->value.getType();
			ElementType nextNodeType = pNextNode->value.getType();
			if (randomType == prevNodeType && randomType == nextNodeType) continue;
			else if (prevNodeType == randomType)
			{
				unsigned int prevPrevNodeIndex = (prevNodeIndex + BOARD_ELEMENT_INLINE_NUM - 1) % BOARD_ELEMENT_INLINE_NUM;
				BoardFrameNode* pPrevPrevNode = &m_boardFrame.lstInlineFrameNodes[prevPrevNodeIndex];
				ElementType prevPrevNodeType = pPrevPrevNode->value.getType();
				if (prevPrevNodeType == randomType) continue;
			}
			else if (nextNodeType == randomType)
			{
				unsigned int nextNextNodeIndex = (nextNodeIndex + BOARD_ELEMENT_INLINE_NUM + 1) % BOARD_ELEMENT_INLINE_NUM;
				BoardFrameNode* pNextNextNode = &m_boardFrame.lstInlineFrameNodes[nextNextNodeIndex];
				ElementType nextNextNodeType = pNextNextNode->value.getType();
				if (nextNextNodeType == randomType) continue;
			}
			//all condition matched
			typeFixed = true;
			m_boardFrame.lstInlineFrameNodes[i].value.setElementType(randomType);
		}
	}
	bool generatingSuccess = false;
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		ElementType randomType = (ElementType)random(1, BOARDELEMENTTYPE_TOTAL_COUNT - 1);
		BoardFrameNode* pOutLineNode = &m_boardFrame.lstOutlineFrameNodes[i * 2];
		BoardFrameNode* pInLineNode = &m_boardFrame.lstInlineFrameNodes[i];
		ElementType outLineElementType = pOutLineNode->value.getType();
		ElementType inLineElementType = pInLineNode->value.getType();
		if (randomType == outLineElementType && randomType == inLineElementType) continue;
		else
		{
			generatingSuccess = true;
			m_boardFrame.centerNode.value.setElementType(randomType);
			break;
		}
	}
	if(!generatingSuccess) shuffleBoardElements(); //do once again
}

std::list<ChainBunch> Board::getChainedNodesInBoard()
{
	bool isOutlineNodeActivated[BOARD_ELEMENT_OUTLINE_NUM] = { true, };
	bool isinlineNodeActivated[BOARD_ELEMENT_OUTLINE_NUM] = { true, };
	bool isCenterNodeActivated = true;

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
	float size = 0.0f;
	for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
	{
		size = m_boardFrame.lstOutlineFrameNodes[i].nodeRad*2;
		m_boardFrame.lstOutlineFrameNodes[i].value.setSize(size,size);
		m_boardFrame.lstOutlineFrameNodes[i].value.render(pWindow);
	}
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		size = m_boardFrame.lstInlineFrameNodes[i].nodeRad*2;
		m_boardFrame.lstInlineFrameNodes[i].value.setSize(size, size);
		m_boardFrame.lstInlineFrameNodes[i].value.render(pWindow);
	}
	size = m_boardFrame.centerNode.nodeRad*2;
	m_boardFrame.centerNode.value.setSize(size, size);
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
	case BOARD_STATE_IDLE: update_idle(); break;
	case BOARD_STATE_PICKUP: update_pickup(); break;
	case BOARD_STATE_ROTATE: update_rotate(); break;
	case BOARD_STATE_ROLL: update_roll(); break;
	case BOARD_STATE_CHECK_MATCH: update_check_match(); break;
	case BOARD_STATE_SUPPLY: update_supply(); break;
	}

	m_vLastTouchPoint = m_vTouchPoint;
}
void Board::update_idle()
{
	//update elements position
	//need to add smooth move animation

	//check if picked up
	if (m_bTouching)
	{
		bool picked = false;
		if (m_boardState == BOARD_STATE_IDLE)
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
				m_vTouchStartPoint = m_vTouchPoint;
				m_vLastTouchPoint = m_vTouchPoint;
				if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_OUTLINE)		resetBoardOutLineElementsPosition();
				else if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_INLINE)	resetBoardInLineElementsPosition();
				else if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_CENTER)	resetBoardCenterElementsPosition();
				m_boardState = BOARD_STATE_PICKUP;
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
		//check whether null is picked
		if (m_pPickedFrameNode == nullptr) return;

		Vector2f originToTouch = m_vTouchPoint - this->m_vPos;
		Vector2f originToNode = m_pPickedFrameNode->nodePos - this->m_vPos;
		Vector2f nodeToTouch = m_vTouchPoint - m_pPickedFrameNode->nodePos;
		Vector2f originToFirstTouch = m_vTouchStartPoint - this->m_vPos;
		Vector2f firstTouchToCurrentTouch = m_vTouchPoint - m_vTouchStartPoint;

		float originAngle = polarAngle(originToFirstTouch);
		float movedAngle = polarAngle(firstTouchToCurrentTouch);
		float dAngle = movedAngle - originAngle;
		float dLength = length(firstTouchToCurrentTouch);

		bool isPickedNodeAbleToRoll = (m_pPickedFrameNode->nodeID % 2 == 0) || (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_INLINE);

		//check conditions to move other states
		//center node only moves to roll state
		if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_CENTER)
		{
			//if draged enough length then move to roll state of with center node
			bool isMovedCertainDistance = (dLength > 100.0f);
			if (isMovedCertainDistance)
			{
				//_RPT1(0, "%f\n", dLength);
				m_boardState = BOARD_STATE_ROLL;
			}

				float mouseMovedDirAngle = polarAngle(firstTouchToCurrentTouch) + 90;
				if (mouseMovedDirAngle < 0)
				{
					mouseMovedDirAngle += 360;
				}
				unsigned int nodeToRoll = getNodeIDOfCenterNodeDirected(mouseMovedDirAngle);
				if (m_iCenterRollTo != nodeToRoll)
				{
					m_iCenterRollTo = nodeToRoll;
					resetBoardElementsPosition();
				}

				Vector2f dir = projectedVector(firstTouchToCurrentTouch, m_boardFrame.lstInlineFrameNodes[m_iCenterRollTo].nodePos - this->m_vPos);
				rollBoardCenter(m_iCenterRollTo, dir);
		}
		else //if the picked node is outline or inline node
		{
			bool isMovedCertainDistance = (dLength > 5.0f);
			if (isMovedCertainDistance)
			{
				bool isRotatedAlongSideLine = (abs(dAngle) >= 45) && (abs(dAngle)<=135);
				if (isRotatedAlongSideLine) //move to rotate
				{
					m_boardState = BOARD_STATE_ROTATE;
				}
				else //moved to roll
				{
					if (isPickedNodeAbleToRoll)
					{
						resetBoardElementsPosition();
						m_boardState = BOARD_STATE_ROLL;
					}
				}
			}
		}
		/*if (isPickedNodeAbleToRoll)
			m_pPickedFrameNode->value.pos = m_vTouchPoint - nodeToTouch;*/
		if (m_pPickedFrameNode->nodeLine != FRAMENODEPOS_CENTER)
		{
			smoothReturnBoardInLine();
			smoothReturnBoardOutLine();
			smoothReturnBoardCenter();
		}
		//smoothReturnBoardInLine();
		//smoothReturnBoardOutLine();
		//smoothReturnBoardCenter();
	}
	else
	{
		//check whether moved elements
		m_boardState = BOARD_STATE_IDLE;
	}
}
void Board::update_rotate()
{
	if (m_bTouching)
	{
		Vector2f originToTouch = m_vTouchPoint - this->m_vPos;
		Vector2f originToNode = m_pPickedFrameNode->nodePos - this->m_vPos;
		Vector2f nodeToTouch = m_vTouchPoint - m_pPickedFrameNode->nodePos;
		Vector2f originToFirstTouch = m_vTouchStartPoint - this->m_vPos;
		Vector2f firstTouchToCurrentTouch = m_vTouchPoint - m_vTouchStartPoint;

		float originAngle = polarAngle(originToFirstTouch);
		float movedAngle = polarAngle(originToTouch);
		float dAngle = movedAngle - originAngle;
		float dLength = length(firstTouchToCurrentTouch);


		if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_OUTLINE)
		{
			rotateBoardOutLine(dAngle);
			smoothReturnBoardInLine();
			smoothReturnBoardCenter();
		}
		else if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_INLINE)
		{
			rotateBoardInLine(dAngle);
			smoothReturnBoardOutLine();
			smoothReturnBoardCenter();
		}
		else //center
		{
			smoothReturnBoardInLine();
			smoothReturnBoardOutLine();
		}
		float dl = length(m_pPickedFrameNode->nodePos - m_pPickedFrameNode->value.pos);
		bool isNotGoingToRotate = (dl <= 5.0f);//(dLength <= 5.0f);
		if (isNotGoingToRotate)
		{
			m_boardState = BOARD_STATE_PICKUP;
		}
	}
	else
	{
		//check whether moved elements
		m_boardState = BOARD_STATE_IDLE;
	}
}
void Board::update_roll()
{
	if (m_bTouching)
	{
		Vector2f originToTouch = m_vTouchPoint - this->m_vPos;
		Vector2f originToNode = (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_CENTER)?(m_boardFrame.lstInlineFrameNodes[m_iCenterRollTo].nodePos - this->m_vPos) : (m_pPickedFrameNode->nodePos - this->m_vPos);
		Vector2f nodeToTouch = m_vTouchPoint - m_pPickedFrameNode->nodePos;
		Vector2f originToFirstTouch = m_vTouchStartPoint - this->m_vPos;
		Vector2f firstTouchToCurrentTouch = m_vTouchPoint - m_vTouchStartPoint;

		Vector2f dir = projectedVector(firstTouchToCurrentTouch, originToNode);
		float dLength = length(dir);
		//_RPT1(0, "%f\n", dLength);
		
		if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_CENTER)
		{
			bool isNotGoingToRoll = (dLength < 100.0f);
			if (isNotGoingToRoll)
			{
				m_boardState = BOARD_STATE_PICKUP;
			}
			float boardBorder = m_boardSizeInfo.wholeRad + m_pPickedFrameNode->nodeRad*0.3;
			if (length(dir) > boardBorder)
			{
				setLength(dir,boardBorder);
			}
			rollBoardCenter(m_iCenterRollTo, dir);
		}
		else
		{
			float boardBorder = m_boardSizeInfo.wholeRad + m_pPickedFrameNode->nodeRad*0.3;
			if (length(originToNode+dir) > boardBorder)
			{
				if(dotProduct(dir,originToNode)<0)
					setLength(dir, boardBorder + length(originToNode));
				else
					setLength(dir, boardBorder - length(originToNode));
			}
			if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_OUTLINE)
			{
				rollBoardOutLine(m_pPickedFrameNode->nodeID, dir);
			}
			else if (m_pPickedFrameNode->nodeLine == FRAMENODEPOS_INLINE)
			{
				rollBoardInLine(m_pPickedFrameNode->nodeID, dir);
			}
			//return to pickup state
			bool isNotGoingToRoll = (dLength < 5.0f);
			if (isNotGoingToRoll)
			{
				m_vTouchStartPoint = m_vTouchPoint;
				m_boardState = BOARD_STATE_PICKUP;
			}
		}
	}
	else
	{
		//check whether moved elements
		m_boardState = BOARD_STATE_IDLE;
	}
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


/***************************HELPERS**************************************************************/
BoardFrameNode* Board::getClosestFrameNodeFromPoint(sf::Vector2f point, bool ignoreCenterNode = false, bool ignoreOutlineNode = false, bool ignoreInlineNode = false)
{
	BoardFrameNode* closestNode = nullptr;
	float minDist = m_boardSizeInfo.wholeRad;
	
	if (!ignoreInlineNode)
	{
		for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
		{
			float distance = abs(length(m_boardFrame.lstInlineFrameNodes[i].nodePos - point));
			if (distance < minDist)
			{
				closestNode = &m_boardFrame.lstInlineFrameNodes[i];
				minDist = distance;
			}
		}
	}
	if (!ignoreOutlineNode)
	{
		for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
		{
			float distance = abs(length(m_boardFrame.lstOutlineFrameNodes[i].nodePos - point));
			if (distance < minDist)
			{
				closestNode = &m_boardFrame.lstOutlineFrameNodes[i];
				minDist = distance;
			}
		}
	}
	if (!ignoreCenterNode)
	{
		float distance = abs(length(m_boardFrame.centerNode.nodePos - point));
		if (distance < minDist)
		{
			closestNode = &m_boardFrame.centerNode;
			minDist = distance;
		}
	}
	return closestNode;
}

unsigned int Board::getNodeIDOfCenterNodeDirected(float dirAngle)
{
	unsigned int retID = 0;
	for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
	{
		float gapOffset = (i % 2 == 0) ? (m_boardSizeInfo.ilAngleGap*0.3) : (m_boardSizeInfo.ilAngleGap*0.7);
		bool isMovingDirInGap = dirAngle <= m_boardSizeInfo.ilAngleGap*i + gapOffset;
		if (isMovingDirInGap)
		{
			retID = i;
			break;
		}
	}
	return retID;
}