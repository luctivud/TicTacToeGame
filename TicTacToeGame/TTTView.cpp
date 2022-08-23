#include "TTTView.h"

TTTView::TTTView(HWND hwnd, const int boardSize) : m_pHWnd(hwnd), m_pnBoardSize(boardSize)
{
	m_pBoardCoordinates.resize(m_pnBoardSize, std::vector<RECT>(m_pnBoardSize, { -1, -1, -1, -1 }));

	for (int irow = 0; irow < m_pnBoardSize; irow++)
	{
		for (int icol = 0; icol < m_pnBoardSize; icol++)
		{
			RECT rect;
			rect.left = UserSettings::OFFSET_X + UserSettings::SZ_SQUARE * icol; // int currTopX
			rect.top = UserSettings::OFFSET_Y + UserSettings::SZ_SQUARE * irow; // int currTopY
			rect.right = rect.left + UserSettings::SZ_SQUARE; // int currBotX
			rect.bottom = rect.top + UserSettings::SZ_SQUARE; // int currBotY
			m_pBoardCoordinates[irow][icol] = rect;
		}
	}

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(m_pHWnd, &ps);
	FillRect(hdc, &ps.rcPaint, (HBRUSH)(CreateSolidBrush(RGB(255, 255, 255)))); // COLOR_WINDOW + 1
	EndPaint(m_pHWnd, &ps);
}

const std::vector<std::vector<RECT>>& TTTView::getBoardCoordinates()
{
	return m_pBoardCoordinates;
}

std::pair<int, int> TTTView::checkIfClickOnBoard(const int xPos, const int yPos)
{
	std::pair<int, int> resBoardBoxClicked = {-1, -1};
	for (int irow = 0; irow < m_pnBoardSize; irow++)
	{
		for (int icol = 0; icol < m_pnBoardSize; icol++)
		{
			const RECT& rect = m_pBoardCoordinates[irow][icol];
			if ((xPos >= rect.left) && (xPos <= rect.right) && (yPos >= rect.top) && (yPos <= rect.bottom))
			{
				resBoardBoxClicked = {irow, icol};
				break;
			}
		}
	}
	return resBoardBoxClicked;
}

void TTTView::drawCircle(HDC hdc, RECT rect)
{
	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
}

void TTTView::drawCross(HDC hdc, RECT rect)
{
	MoveToEx(hdc, rect.left, rect.top, NULL);
	LineTo(hdc, rect.right, rect.bottom);
	MoveToEx(hdc, rect.left, rect.bottom, NULL);
	LineTo(hdc, rect.right, rect.top);
}

void TTTView::updateBoard(const std::pair<int, int>& boxClicked, const int turn)
{
	HDC hdc = GetDC(m_pHWnd);
	HPEN pen = CreatePen(PS_SOLID, 4, UserSettings::COLOR_USER[turn % 2]);
	RECT rect = m_pBoardCoordinates[boxClicked.first][boxClicked.second];

	rect.top += UserSettings::OFFSET_BORDER;
	rect.bottom -= UserSettings::OFFSET_BORDER;
	rect.left += UserSettings::OFFSET_BORDER;
	rect.right -= UserSettings::OFFSET_BORDER;

	SelectObject(hdc, pen);

	
	if (turn % 2 == 0)
		drawCircle(hdc, rect);
	else
		drawCross(hdc, rect);

	DeleteObject(pen);
	ReleaseDC(m_pHWnd, hdc);
}

const RECT TTTView::getRectAtRC(const int row, const int col)
{
	return m_pBoardCoordinates[row][col];
}

TTTView::~TTTView()
{
}



void TTTView::displayBoard()
{
	HDC hdc = GetDC(m_pHWnd);

	TCHAR textToDisplayAsHeader[] = L"Play TicTacToe - \n";
	TextOut(hdc, UserSettings::OFFSET_X, 10, textToDisplayAsHeader, (int)wcslen(textToDisplayAsHeader));

	for (int irow = 0; irow < m_pnBoardSize; irow++)
	{
		for (int icol = 0; icol < m_pnBoardSize; icol++)
		{
			RECT rect = m_pBoardCoordinates[irow][icol];
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		}
	}
	ReleaseDC(m_pHWnd, hdc);
}

