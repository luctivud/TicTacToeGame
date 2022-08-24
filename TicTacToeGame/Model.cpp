#include "Model.h"

Model::Model() : m_pnTurn(0), mIsGameOver(false)
{
	m_pBoardEntries.resize(m_pnBoardSize, std::vector<char>(m_pnBoardSize, '.'));
	m_pValidMovesPlayed.reserve(m_pnBoardSize * m_pnBoardSize);
}

Model::~Model()
{
}

int Model::updateTurn()
{
	return ++m_pnTurn;
}

const int Model::getBoardSize()
{
	return m_pnBoardSize;
}

bool Model::getGameOver()
{
	return mIsGameOver;
}

bool Model::isValidMove(const std::pair<int, int> boxClicked)
{
	return (m_pBoardEntries[boxClicked.first][boxClicked.second] == '.');
}

void Model::updateBoard(const std::pair<int, int> boxClicked)
{
	if (m_pnTurn % 2 == 0)
		m_pBoardEntries[boxClicked.first][boxClicked.second] = 'O';
	else
		m_pBoardEntries[boxClicked.first][boxClicked.second] = 'X';
}

int Model::checkWin(const std::pair<int, int> boxClicked)
{
	int rw = boxClicked.first;
	int cl = boxClicked.second;

	//check Col
	bool okCol = m_pBoardEntries[rw][0] != '.';
	for (int cl_temp = 1; cl_temp < m_pnBoardSize && okCol; cl_temp++)
	{
		okCol &= (m_pBoardEntries[rw][cl_temp] == m_pBoardEntries[rw][0]);
	}

	// check Row
	bool okRow = m_pBoardEntries[0][cl] != '.';
	for (int rw_temp = 1; rw_temp < m_pnBoardSize && okRow; rw_temp++)
	{
		okRow &= (m_pBoardEntries[rw_temp][cl] == m_pBoardEntries[0][cl]);
	}


	// check Diagonal 1
	bool okDiagonal1 = false;
	okDiagonal1 = m_pBoardEntries[0][0] != '.';
	for (int itemp = 1; itemp < m_pnBoardSize && okDiagonal1; itemp++)
	{
		okDiagonal1 &= (m_pBoardEntries[itemp][itemp] == m_pBoardEntries[0][0]);
	}


	bool okDiagonal2 = false; // check Diagonal 2
	okDiagonal2 = m_pBoardEntries[0][m_pnBoardSize - 1] != '.';
	for (int itemp = 0, itemp2 = m_pnBoardSize - 1; itemp < m_pnBoardSize && okDiagonal2; itemp++, itemp2--)
	{
		okDiagonal2 &= (m_pBoardEntries[itemp][itemp2] == m_pBoardEntries[0][m_pnBoardSize - 1]);
	}


	if (okRow or okCol or okDiagonal1 or okDiagonal2)
	{
		return m_pnTurn % 2;
	}
	return -1;
}

int Model::checkDraw(const std::pair<int, int> boxClicked)
{
	return m_pnTurn == (m_pnBoardSize * m_pnBoardSize); // game ends in a draw
}

int Model::checkIfGameEnded(const std::pair<int, int> boxClicked)
{
	int winStatus = checkWin(boxClicked);
	if (winStatus != -1) 
	{
		mIsGameOver = true;
		return winStatus;
	}
		
	if (checkDraw(boxClicked))
	{
		mIsGameOver = true;
		return m_pnBoardSize + 1; // no player won but game ended
	}
		
	return -1;
}

void Model::addToValidMoves(const std::pair<int, int> boxClicked) noexcept
{
	m_pValidMovesPlayed.push_back(boxClicked);
	return;
}

const std::vector<std::pair<int, int>>& Model::getValidMovesPlayed() noexcept
{
	return m_pValidMovesPlayed;
}

const int Model::getTurn()
{
	return m_pnTurn;
}


