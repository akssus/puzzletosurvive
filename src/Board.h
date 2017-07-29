#ifndef __BOARD_H__
#define __BOARD_H__

#include "SFML/Graphics.hpp"
#include "Thor/Math.hpp"
#include "Thor/Vectors.hpp"
#include "BoardElement.h"
#include "Parameters.h"
#include <vector>

struct BoardInfo {
	float wholeRad, outlineRad, inlineRad, centerRad;
	float outlineElementRad, inlineElementRad, centerElementRad;
	float olAngleGap, ilAngleGap;
	BoardInfo() : wholeRad(0.0f), outlineRad(0.0f), inlineRad(0.0f), centerRad(0.0f),
		outlineElementRad(0.0f), inlineElementRad(0.0f), centerElementRad(0.0f),
		olAngleGap(0.0f), ilAngleGap(0.0f) {};
	void setSize(float wRad, unsigned int olNum, unsigned int ilNum)
	{
		wholeRad = wRad;
		outlineElementRad = wRad * sinf(thor::Pi / (float)olNum);
		inlineElementRad = (wRad - outlineElementRad)*sinf(thor::Pi / (float)ilNum) / (1 + sinf(thor::Pi / (float)ilNum));
		centerElementRad = wRad - (outlineElementRad + 2 * inlineElementRad);
		centerRad = centerElementRad;
		inlineRad = centerRad + inlineElementRad;
		outlineRad = inlineRad + inlineElementRad + outlineElementRad;
		olAngleGap = 360.0f / (float)olNum;
		ilAngleGap = 360.0f / (float)ilNum;
	};
};

enum BoardFrameNodeLine
{
	FRAMENODEPOS_OUTLINE,
	FRAMENODEPOS_INLINE,
	FRAMENODEPOS_CENTER
};
struct BoardFrameNode
{
	BoardElement					value;
	std::vector<BoardFrameNode*>	adjacentNodes;
	BoardFrameNodeLine				nodeLine;
	sf::Vector2f					nodePos;
	float							nodeRad;
	unsigned int					nodeID;

	BoardFrameNode() : nodeRad(0.0f) {};
	void setElement(BoardElement value)
	{
		this->value = value;
	};
	void removeElement()
	{
		value.setElementType(BOARDELEMENTTYPE_EMPTY);
	}
	void addAdjacentNode(BoardFrameNode* adjNode)
	{
		if (adjNode != NULL) adjacentNodes.push_back(adjNode);
	};
	bool isEmpty() { return (value.getType() == BOARDELEMENTTYPE_EMPTY); };
	bool containsPoint(sf::Vector2f pos)
	{
		sf::Vector2f dist = pos - nodePos;
		return thor::length(dist) <= nodeRad;
	}
};
struct BoardFrame
{
	std::vector<BoardFrameNode>	lstOutlineFrameNodes;
	std::vector<BoardFrameNode>	lstInlineFrameNodes;
	BoardFrameNode				centerNode;
	BoardFrame() { init(); };
	void init()
	{
		//generate frame nodes
		lstOutlineFrameNodes.resize(BOARD_ELEMENT_OUTLINE_NUM);
		lstInlineFrameNodes.resize(BOARD_ELEMENT_INLINE_NUM);

		//bind frame nodes. i=0 is top elements in the board
		for (int i = 0; i < BOARD_ELEMENT_OUTLINE_NUM; ++i)
		{
			//bind to same line elements
			int iLeftNode = (i + BOARD_ELEMENT_OUTLINE_NUM - 1) % BOARD_ELEMENT_OUTLINE_NUM;
			int iRightNode = (i + BOARD_ELEMENT_OUTLINE_NUM + 1) % BOARD_ELEMENT_OUTLINE_NUM;
			lstOutlineFrameNodes[i].addAdjacentNode(&lstOutlineFrameNodes[iLeftNode]);
			lstOutlineFrameNodes[i].addAdjacentNode(&lstOutlineFrameNodes[iRightNode]);
			//bind to inline elements
			if (i % 2 == 0)	lstOutlineFrameNodes[i].addAdjacentNode(&lstInlineFrameNodes[i / 2]);
			//set line id
			lstOutlineFrameNodes[i].nodeLine = FRAMENODEPOS_OUTLINE;
			lstOutlineFrameNodes[i].nodeID = i;
		}
		//bind frame nodes i=0 is top elements in the board
		for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
		{
			//bind to same line elements
			int iLeftNode = (i + BOARD_ELEMENT_INLINE_NUM - 1) % BOARD_ELEMENT_INLINE_NUM;
			int iRightNode = (i + BOARD_ELEMENT_INLINE_NUM + 1) % BOARD_ELEMENT_INLINE_NUM;
			lstInlineFrameNodes[i].addAdjacentNode(&lstInlineFrameNodes[iLeftNode]);
			lstInlineFrameNodes[i].addAdjacentNode(&lstInlineFrameNodes[iRightNode]);
			//bind to outline elements
			if (i % 2 == 0)	lstInlineFrameNodes[i].addAdjacentNode(&lstInlineFrameNodes[i / 2]);
			//bind to center element
			lstInlineFrameNodes[i].addAdjacentNode(&centerNode);
			//set line id
			lstInlineFrameNodes[i].nodeLine = FRAMENODEPOS_INLINE;
			lstInlineFrameNodes[i].nodeID = i;
		}
		for (int i = 0; i < BOARD_ELEMENT_INLINE_NUM; ++i)
		{
			//center node is bidirectional
			centerNode.addAdjacentNode(&lstInlineFrameNodes[i]);
		}
		centerNode.nodeLine = FRAMENODEPOS_CENTER;
	}
};

enum BoardState
{
	BOARD_STATE_IDLE,
	BOARD_STATE_PICKUP,
	BOARD_STATE_ROTATE,
	BOARD_STATE_ROLL,
	BOARD_STATE_CHECK_MATCH,
	BOARD_STATE_MATCHING,
	BOARD_STATE_SUPPLY
};

struct ChainBunch
{
	std::list<BoardFrameNode*> chainedNodes;
};

class Board
{
public:
	Board(sf::RenderWindow* pWindow);
	~Board();
public:
	bool init();
	void update();
	void render();

	void setPosition(sf::Vector2f pos);

private:
	void update_idle();
	void update_pickup();
	void update_rotate();
	void update_roll();
	void update_check_match();
	void update_matching();
	void update_supply();

private:
	void renderBoardElements(sf::RenderWindow* pWindow);
	void resetBoardElementsPosition();
	void resetBoardOutLineElementsPosition();
	void resetBoardInLineElementsPosition();
	void resetBoardCenterElementsPosition();
	void smoothReturnBoardOutLine();
	void smoothReturnBoardInLine();
	void smoothReturnBoardCenter();
	void adjustElementsSize();
	void rotateBoardOutLine(float dAngle);
	void rotateBoardInLine(float dAngle);
	void rollBoardOutLine(unsigned int index, sf::Vector2f dir);
	void rollBoardInLine(unsigned int index, sf::Vector2f dir);
	void rollBoardCenter(unsigned int toIndex, sf::Vector2f dir);
	void pushBoardOutLineCW(unsigned int pushDistance);
	void pushBoardInLineCW(unsigned int pushDistance);
	void pushBoardRail(unsigned int pushDistance, std::vector<BoardFrameNode*>& candidateNodes);
	void dropOutlineNodeToEmptyInlineNode();

private:
	void					shuffleBoardElements();
	std::list<ChainBunch>	getChainedNodesInBoard();
	void					getSameTypeOfAdjacents(BoardFrameNode* base, ChainBunch* container);

private:
	BoardFrameNode*					getClosestFrameNodeFromPoint(sf::Vector2f , bool ignoreCenterNode, bool ignoreOutlineNode, bool ignoreInlineNode);
	unsigned int					getNodeIDOfCenterNodeDirected(float dirAngle);
	std::vector<BoardFrameNode*>	getRailOfOutlineNodeIndex(unsigned int index);
	unsigned int					getLinearDistanceBetweenNodes(BoardFrameNode* node_1, BoardFrameNode* node_2);
	std::list<BoardFrameNode*>		getEmptyNodes();

	//void rollBoard


private:
	sf::RenderWindow*		m_pWindow;
	BoardState				m_boardState;
	BoardFrame				m_boardFrame;
	BoardInfo				m_boardSizeInfo;
	BoardFrameNode*			m_pPickedFrameNode;
	float					m_fPickedElementRotatedAngle;
	bool					m_bTouching;
	sf::Vector2f			m_vTouchPoint;
	sf::Vector2f			m_vTouchStartPoint;
	sf::Vector2f			m_vLastTouchPoint;
	sf::Vector2f			m_vPos;
	unsigned int			m_iCenterRollTo;
	std::list<ChainBunch>	m_lstChainedBunches;

};

#endif
