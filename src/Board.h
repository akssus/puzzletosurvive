#pragma once
#include <vector>

struct BBBoardInfo {
	float wholeRad, outlineRad, inlineRad, centerRad;
	float outlineElementRad, inlineElementRad, centerElementRad;
	float olAngleGap, ilAngleGap;
	BBBoardInfo() : wholeRad(0.0f), outlineRad(0.0f), inlineRad(0.0f), centerRad(0.0f),
		outlineElementRad(0.0f), inlineElementRad(0.0f), centerElementRad(0.0f),
		olAngleGap(0.0f), ilAngleGap(0.0f) {};
	void setSize(float wRad, unsigned int olNum, unsigned int ilNum)
	{
		wholeRad = wRad;
		outlineElementRad = wRad * sinf(M_PI / (float)olNum);
		inlineElementRad = (wRad - outlineElementRad)*sinf(M_PI / (float)ilNum) / (1 + sinf(M_PI / (float)ilNum));
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
struct BBBoardFrameNode
{
	BBBoardElement*					value;
	std::vector<BBBoardFrameNode*>	adjacentNodes;
	BoardFrameNodeLine				nodeLine;
	cocos2d::Vec2					nodePos;
	float							nodeRad;
	unsigned int					nodeID;

	BBBoardFrameNode() :value(0), nodeRad(0.0f) {};
	void setElement(BBBoardElement* value)
	{
		if (value != NULL) this->value = value;
	};
	void removeElement()
	{
		value = NULL; //don't worry, auto release!
	}
	void addAdjacentNode(BBBoardFrameNode* adjNode)
	{
		if (adjNode != NULL) adjacentNodes.push_back(adjNode);
	};
	bool isEmpty() { return (value == NULL); };
	bool containsPoint(cocos2d::Vec2 pos)
	{
		cocos2d::Vec2 dist = pos - nodePos;
		return dist.length() <= nodeRad;
	}
};
struct BBBoardFrame
{
	std::vector<BBBoardFrameNode>	lstOutlineFrameNodes;
	std::vector<BBBoardFrameNode>	lstInlineFrameNodes;
	BBBoardFrameNode				centerNode;
	BBBoardFrame() { init(); };
	void init()
	{
		//generate frame nodes
		lstOutlineFrameNodes.resize(BB_ELEMENT_OUTLINE_NUM);
		lstInlineFrameNodes.resize(BB_ELEMENT_INLINE_NUM);

		//bind frame nodes. i=0 is top elements in the board
		for (int i = 0; i < BB_ELEMENT_OUTLINE_NUM; ++i)
		{
			//bind to same line elements
			int iLeftNode = (i + BB_ELEMENT_OUTLINE_NUM - 1) % BB_ELEMENT_OUTLINE_NUM;
			int iRightNode = (i + BB_ELEMENT_OUTLINE_NUM + 1) % BB_ELEMENT_OUTLINE_NUM;
			lstOutlineFrameNodes[i].addAdjacentNode(&lstOutlineFrameNodes[iLeftNode]);
			lstOutlineFrameNodes[i].addAdjacentNode(&lstOutlineFrameNodes[iRightNode]);
			//bind to inline elements
			if (i % 2 == 0)	lstOutlineFrameNodes[i].addAdjacentNode(&lstInlineFrameNodes[i / 2]);
			//set line id
			lstOutlineFrameNodes[i].nodeLine = FRAMENODEPOS_OUTLINE;
			lstOutlineFrameNodes[i].nodeID = i;
		}
		//bind frame nodes i=0 is top elements in the board
		for (int i = 0; i < BB_ELEMENT_INLINE_NUM; ++i)
		{
			//bind to same line elements
			int iLeftNode = (i + BB_ELEMENT_INLINE_NUM - 1) % BB_ELEMENT_INLINE_NUM;
			int iRightNode = (i + BB_ELEMENT_INLINE_NUM + 1) % BB_ELEMENT_INLINE_NUM;
			lstInlineFrameNodes[i].addAdjacentNode(&lstInlineFrameNodes[iLeftNode]);
			lstInlineFrameNodes[i].addAdjacentNode(&lstInlineFrameNodes[iRightNode]);
			//bind to outline elements (does it really need??)
			//if (i % 2 == 0)	lstInlineFrameNodes[i].addAdjacentNode(&lstInlineFrameNodes[i / 2]);
			//bind to center element
			lstInlineFrameNodes[i].addAdjacentNode(&centerNode);
			//set line id
			lstInlineFrameNodes[i].nodeLine = FRAMENODEPOS_INLINE;
			lstInlineFrameNodes[i].nodeID = i;
		}
		centerNode.nodeLine = FRAMENODEPOS_CENTER;
		for (int i = 0; i < BB_ELEMENT_INLINE_NUM; ++i)
		{
			//centerNode.addAdjacentNode(&lstInlineFrameNodes[i]);
		}
	}
};

enum BoardState
{
	BBSTATE_BOARD_IDLE,
	BBSTATE_BOARD_PICKUP,
	BBSTATE_BOARD_ROTATE,
	BBSTATE_BOARD_ROLL,
	BBSTATE_BOARD_CHECK_MATCH,
	BBSTATE_BOARD_MATCHING,
	BBSTATE_BOARD_SUPPLY
};

class BBBoard : public cocos2d::Layer
{
public:
	virtual bool init();

	void update();
	void update_idle();
	void update_pickup();
	void update_rotate();
	void update_roll();
	void update_check_match();
	void update_matching();
	void update_supply();

	CREATE_FUNC(BBBoard);

public:
	virtual bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
	virtual void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);


private:
	void reposeBoardElements();
	void resetBoardElementsPosition();
	void smoothReturnBoardOutLine();
	void smoothReturnBoardInLine();
	void smoothReturnBoardCenter();
	void rotateBoardOutLine(float dAngle);
	void rotateBoardInLine(float dAngle);
	//void rollBoard

private:
	BoardState				m_boardState;
	BBBoardFrame			m_boardFrame;
	BBBoardInfo				m_boardSizeInfo;
	BBBoardFrameNode*		m_pPickedFrameNode;
	float					m_fPickedElementRotatedAngle;
	bool					m_bTouching;
	cocos2d::Vec2			m_vTouchPoint;

};
