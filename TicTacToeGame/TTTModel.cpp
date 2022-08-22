#include "TTTModel.h"

TTTModel::TTTModel() : m_pnTurn(0)
{
	m_pBoardEntries.resize(m_pnBoardSize, std::vector<char>(m_pnBoardSize, '.'));
	
}

TTTModel::~TTTModel()
{
}

int TTTModel::updateTurn()
{
	return ++m_pnTurn;
}

const int TTTModel::getBoardSize()
{
	return m_pnBoardSize;
}

bool TTTModel::isValidMove(const std::pair<int, int> boxClicked)
{
	return (m_pBoardEntries[boxClicked.first][boxClicked.second] == '.');
}

void TTTModel::updateBoard(const std::pair<int, int> boxClicked)
{
	if (m_pnTurn % 2 == 0)
		m_pBoardEntries[boxClicked.first][boxClicked.second] = 'O';
	else
		m_pBoardEntries[boxClicked.first][boxClicked.second] = 'X';
}

bool TTTModel::checkIfGameEnded(const std::pair<int, int> boxClicked)
{
	//todo logic
	return false;
}

const int TTTModel::getTurn()
{
	return m_pnTurn;
}


