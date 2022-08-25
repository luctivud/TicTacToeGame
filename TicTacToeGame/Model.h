#pragma once

#include "resource.h"

#include <vector>

class Model
{
public:
	Model();
	virtual ~Model();
	int updateTurn();
	const int getBoardSize();
	bool getIsGameOver();
	bool isValidMove(const std::pair<int, int> boxClicked);
	void updateBoard(const std::pair<int, int> boxClicked);
	int checkIfGameEnded(const std::pair<int, int> boxClicked);
	void addToValidMoves(const std::pair<int, int> boxClicked) noexcept;
	const std::vector<std::pair<int, int>>& getValidMovesPlayed() noexcept;
	const int getTurn();
private:
	
	std::vector<std::vector<char>> m_pBoardEntries;
	std::vector<std::pair<int, int>> m_pValidMovesPlayed;
	int checkWin(const std::pair<int, int> boxClicked);
	int checkDraw(const std::pair<int, int> boxClicked);

	static const int mBoardSize;

	bool mIsGameOver = false;
	int mTurn = 0;
};
