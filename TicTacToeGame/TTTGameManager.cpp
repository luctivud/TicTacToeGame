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

const std::pair<int, int> TTTGameManager::responseToClick(const int xPos, const int yPos)
{
	std::pair<int, int> boxClicked = m_pTTTView->checkIfClickOnBoard(xPos, yPos);
	if (boxClicked.first != -1 and boxClicked.second != -1 and m_pTTTModel->isValidMove(boxClicked)) // clicked on Board
	{
		m_pTTTModel->updateTurn();
		m_pTTTModel->updateBoard(boxClicked);
		m_pTTTView->updateBoard(boxClicked, m_pTTTModel->getTurn());
		m_pTTTModel->checkIfGameEnded(boxClicked);
	}
	else
	{
		boxClicked = { -1, -1 };
	}
	return boxClicked;
}
