#pragma once
#include <windows.h>
#include <vector>
class TTTView
{
public:
	TTTView(HWND hwnd, const int boardSize);
	~TTTView();
	void displayBoard();
	const std::vector<std::vector<RECT>>& getBoardCoordinates();
	std::pair<int, int> checkIfClickOnBoard(const int xPos, const int yPos);
	void updateBoard(const std::pair<int, int>& boxClicked, const int turn);
private:
	HWND m_pHWnd;
	int m_pnBoardSize;
	std::vector<std::vector<RECT>> m_pBoardCoordinates;
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
