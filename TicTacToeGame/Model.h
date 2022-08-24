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
	bool getGameOver();
	bool isValidMove(const std::pair<int, int> boxClicked);
	void updateBoard(const std::pair<int, int> boxClicked);
	int checkIfGameEnded(const std::pair<int, int> boxClicked);
	void addToValidMoves(const std::pair<int, int> boxClicked) noexcept;
	const std::vector<std::pair<int, int>>& getValidMovesPlayed() noexcept;
	const int getTurn();
private:
	bool mIsGameOver = false;
	const int m_pnUsers = 2;
	const int m_pnBoardSize = 3;
	int m_pnTurn = 0;
	std::vector<std::vector<char>> m_pBoardEntries;
	std::vector<std::pair<int, int>> m_pValidMovesPlayed;
	int checkWin(const std::pair<int, int> boxClicked);
	int checkDraw(const std::pair<int, int> boxClicked);
};
