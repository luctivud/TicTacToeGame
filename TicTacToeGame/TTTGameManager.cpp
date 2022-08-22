#include "TTTGameManager.h"


TTTGameManager::TTTGameManager(HWND hwnd) : m_pHWnd(hwnd), m_pTTTModel(new TTTModel()), m_pTTTView(new TTTView(hwnd, m_pTTTModel->getBoardSize()))
{
	m_pTTTView->displayBoard();
}

TTTGameManager::~TTTGameManager()
{
	delete m_pTTTView;
	delete m_pTTTModel;
}

const std::array<int, 3> TTTGameManager::responseToClick(const int xPos, const int yPos)
{
	std::array<int, 3> responseArray = {0, -1, -1};
	std::pair<int, int> boxClicked = m_pTTTView->checkIfClickOnBoard(xPos, yPos);
	if ((boxClicked.first != -1) and (boxClicked.second != -1) and 
		(m_pTTTModel->isValidMove(boxClicked))) // click was on Board
	{
		m_pTTTModel->updateTurn();
		m_pTTTModel->updateBoard(boxClicked);
		m_pTTTView->updateBoard(boxClicked, m_pTTTModel->getTurn());
		responseArray[0] = m_pTTTModel->checkIfGameEnded(boxClicked);
	}
	else
	{
		boxClicked = { -1, -1 };
	}

	responseArray[1] = boxClicked.first;
	responseArray[2] = boxClicked.second;
	return responseArray;
}
