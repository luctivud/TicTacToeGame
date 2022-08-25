#pragma once
#include <windows.h>
#include <vector>
class View
{
public:
	View(HWND hwnd, const int boardSize);
	~View();
	void displayBoard();
	const std::vector<std::vector<RECT>>& getBoardCoordinates();
	std::pair<int, int> checkIfClickOnBoard(const int xPos, const int yPos);
	void updateBoard(const std::pair<int, int>& boxClicked, const int turn);
	const RECT getRectAtRC(const int row, const int col);
private:
	HWND mHWnd;
	int mBoardSize;
	std::vector<std::vector<RECT>> mBoardCoordinates;
	void drawCircle(HDC hdc, RECT rect);
	void drawCross(HDC hdc, RECT rect);
};

namespace UserSettings
{
	constexpr int SZ_SQUARE = 120;
	constexpr int OFFSET_X = 40;
	constexpr int OFFSET_Y = 40;
	constexpr int OFFSET_BORDER = 20;
	constexpr COLORREF COLOR_USER[] = { RGB(0, 0, 255), RGB(255, 0, 0) };
}

