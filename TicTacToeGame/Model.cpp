#include "Model.h"

const int Model::mBoardSize = 3;

Model::Model() : mTurn(0), mIsGameOver(false)
{
	m_pBoardEntries.resize(mBoardSize, std::vector<char>(mBoardSize, '.'));
	m_pValidMovesPlayed.reserve(mBoardSize * mBoardSize);
}

Model::~Model()
{
}
const int& Model::updateTurn(bool ifUpdateTurn) noexcept
{
	if (ifUpdateTurn)
		return ++mTurn;
	else
		return mTurn;
}

const int& Model::getBoardSize()
{
	return mBoardSize;
}

bool Model::getIsGameOver()
{
	return mIsGameOver;
}

bool Model::isValidMove(const std::pair<int, int>& boxClicked)
{
	return (m_pBoardEntries[boxClicked.first][boxClicked.second] == '.');
}

void Model::updateBoard(const std::pair<int, int>& boxClicked)
{
	if (mTurn % 2 == 0)
		m_pBoardEntries[boxClicked.first][boxClicked.second] = 'O';
	else
		m_pBoardEntries[boxClicked.first][boxClicked.second] = 'X';
}

int Model::checkWin(const std::pair<int, int>& boxClicked, std::array<int, 7>& responseArray)
{
	int rw = boxClicked.first;
	int cl = boxClicked.second;

	//check Col
	bool okCol = m_pBoardEntries[rw][0] != '.';
	for (int cl_temp = 1; cl_temp < mBoardSize && okCol; cl_temp++)
	{
		okCol &= (m_pBoardEntries[rw][cl_temp] == m_pBoardEntries[rw][0]);
	}
	if (okCol)
	{
		responseArray[3] = rw;
		responseArray[4] = 0;
		responseArray[5] = rw;
		responseArray[6] = mBoardSize - 1;
	}

	// check Row
	bool okRow = m_pBoardEntries[0][cl] != '.';
	for (int rw_temp = 1; rw_temp < mBoardSize && okRow; rw_temp++)
	{
		okRow &= (m_pBoardEntries[rw_temp][cl] == m_pBoardEntries[0][cl]);
	}
	if (okRow)
	{
		responseArray[3] = 0;
		responseArray[4] = cl;
		responseArray[5] = mBoardSize - 1;
		responseArray[6] = cl;
	}

	// check Diagonal 1
	bool okDiagonal1 = false;
	okDiagonal1 = m_pBoardEntries[0][0] != '.';
	for (int itemp = 1; itemp < mBoardSize && okDiagonal1; itemp++)
	{
		okDiagonal1 &= (m_pBoardEntries[itemp][itemp] == m_pBoardEntries[0][0]);
	}
	if (okDiagonal1)
	{
		responseArray[3] = 0;
		responseArray[4] = 0;
		responseArray[5] = mBoardSize - 1;
		responseArray[6] = mBoardSize - 1;
	}

	bool okDiagonal2 = false; // check Diagonal 2
	okDiagonal2 = m_pBoardEntries[0][mBoardSize - 1] != '.';
	for (int itemp = 0, itemp2 = mBoardSize - 1; itemp < mBoardSize && okDiagonal2; itemp++, itemp2--)
	{
		okDiagonal2 &= (m_pBoardEntries[itemp][itemp2] == m_pBoardEntries[0][mBoardSize - 1]);
	}
	if (okDiagonal2)
	{
		responseArray[3] = 0;
		responseArray[4] = mBoardSize - 1;
		responseArray[5] = mBoardSize - 1;
		responseArray[6] = 0;
	}

	if (okRow or okCol or okDiagonal1 or okDiagonal2)
	{
		return mTurn % 2;
	}
	return -1;
}

int Model::checkDraw(const std::pair<int, int>& boxClicked)
{
	return mTurn == (mBoardSize * mBoardSize); // game ends in a draw
}

void Model::checkIfGameEnded(const std::pair<int, int>& boxClicked, std::array<int, 7>& responseArray)
{
	int winStatus = checkWin(boxClicked, responseArray);
	if (winStatus != -1)
	{
		mIsGameOver = true;
		responseArray[0] = winStatus;
		return;
	}

	if (checkDraw(boxClicked))
	{
		mIsGameOver = true;
		responseArray[0] = mBoardSize + 1; // no player won but game ended
		return;
	}

	responseArray[0] = -1;
	return;
}

void Model::addToValidMoves(const std::pair<int, int>& boxClicked) noexcept
{
	m_pValidMovesPlayed.push_back(boxClicked);
	return;
}

const std::vector<std::pair<int, int>> Model::getValidMovesPlayed() noexcept
{
	return m_pValidMovesPlayed;
}