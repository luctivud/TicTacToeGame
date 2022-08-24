#include "GameManager.h"


GameManager::GameManager(HWND hwnd) 
	: mHWnd(hwnd)
	, mpModel(new Model())
	, mpView(new View(hwnd, mpModel->getBoardSize()))
{
	mpView->displayBoard();
}

GameManager::~GameManager()
{
	delete mpView; //recommend using smart pointers
	delete mpModel;
}

const std::array<int, 3> GameManager::responseToClick(const int xPos, const int yPos) // why return by value 
{
	std::array<int, 3> responseArray = {-1, -1, -1};
	if (mpModel->getTurn() >= mpModel->getBoardSize() * mpModel->getBoardSize())
		return responseArray;
	std::pair<int, int> boxClicked = mpView->checkIfClickOnBoard(xPos, yPos);
	if ((boxClicked.first != -1) and (boxClicked.second != -1) and 
		(mpModel->isValidMove(boxClicked))) // click was on Board
	{
		mpModel->updateTurn();
		mpModel->addToValidMoves(boxClicked);
		mpModel->updateBoard(boxClicked);
		mpView->updateBoard(boxClicked, mpModel->getTurn());
		
		responseArray[0] = mpModel->checkIfGameEnded(boxClicked);
	}
	else
	{
		boxClicked = { -1, -1 };
	}

	responseArray[1] = boxClicked.first;
	responseArray[2] = boxClicked.second;
	return responseArray;
}

const std::vector<std::pair<int, int>> GameManager::getValidMovesPlayed()
{
	return mpModel->getValidMovesPlayed();
}

const RECT GameManager::getRectCoordinatesRC(std::pair<int, int> boxClicked)
{
	return mpView->getRectAtRC(boxClicked.first, boxClicked.second);
}

bool GameManager::checkGameOver()
{
	return mpModel->getGameOver();
}
