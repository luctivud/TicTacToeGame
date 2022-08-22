#pragma once

#include <vector>
#include <Windows.h>

class TTTModel
{
public:
	TTTModel();
	virtual ~TTTModel();
	int updateTurn();
	const int getBoardSize();
	bool isValidMove(const std::pair<int, int> boxClicked);
	void updateBoard(const std::pair<int, int> boxClicked);
	int checkIfGameEnded(const std::pair<int, int> boxClicked);
	const int getTurn();
private:
	const int m_pnUsers = 2;
	const int m_pnBoardSize = 3;
	int m_pnTurn = 0;
	std::vector<std::vector<char>> m_pBoardEntries;
	int checkWin(const std::pair<int, int> boxClicked);
	int checkDraw(const std::pair<int, int> boxClicked);
};
