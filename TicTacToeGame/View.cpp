#include "View.h"

View::View(HWND hwnd, const int boardSize) : mHWnd(hwnd), mBoardSize(boardSize)
{
	UpdateWindow(mHWnd);
	mBoardCoordinates.resize(mBoardSize, std::vector<RECT>(mBoardSize, { -1, -1, -1, -1 }));

	for (int irow = 0; irow < mBoardSize; irow++)
	{
		for (int icol = 0; icol < mBoardSize; icol++)
		{
			RECT rect;
			rect.left = UserSettings::OFFSET_X + UserSettings::SZ_SQUARE * icol; // int currTopX
			rect.top = UserSettings::OFFSET_Y + UserSettings::SZ_SQUARE * irow; // int currTopY
			rect.right = rect.left + UserSettings::SZ_SQUARE; // int currBotX
			rect.bottom = rect.top + UserSettings::SZ_SQUARE; // int currBotY
			mBoardCoordinates[irow][icol] = rect;
		}
	}


	SetWindowPos
	(
		mHWnd,
		0,
		0, 0, mBoardCoordinates[2][2].right + 60, mBoardCoordinates[2][2].bottom + 75,
		SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER
	);
}

const std::vector<std::vector<RECT>>& View::getBoardCoordinates()
{
	return mBoardCoordinates;
}

std::pair<int, int> View::checkIfClickOnBoard(const int xPos, const int yPos)
{
	std::pair<int, int> resBoardBoxClicked = {-1, -1};
	for (int irow = 0; irow < mBoardSize; irow++)
	{
		for (int icol = 0; icol < mBoardSize; icol++)
		{
			const RECT& rect = mBoardCoordinates[irow][icol];
			if ((xPos > rect.left) && (xPos < rect.right) && (yPos > rect.top) && (yPos < rect.bottom))
			{
				resBoardBoxClicked = {irow, icol};
				break;
			}
		}
	}
	return resBoardBoxClicked;
}

void View::drawCircle(HDC hdc, RECT rect)
{
	Ellipse(hdc, rect.left, rect.top, rect.right, rect.bottom);
}

void View::drawCross(HDC hdc, RECT rect)
{
	MoveToEx(hdc, rect.left, rect.top, NULL);
	LineTo(hdc, rect.right, rect.bottom);
	MoveToEx(hdc, rect.left, rect.bottom, NULL);
	LineTo(hdc, rect.right, rect.top);
}

void View::updateBoard(const std::pair<int, int>& boxClicked, const int turn)
{
	HDC hdc = GetDC(mHWnd);
	HPEN pen = CreatePen(PS_SOLID, 4, UserSettings::COLOR_USER[turn % 2]);
	RECT rect = mBoardCoordinates[boxClicked.first][boxClicked.second];

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
	ReleaseDC(mHWnd, hdc);
}

const RECT View::getRectAtRC(const int row, const int col)
{
	return mBoardCoordinates[row][col];
}

View::~View()
{
}



void View::displayBoard()
{
	HDC hdc = GetDC(mHWnd);

	/*TCHAR textToDisplayAsHeader[] = L"Play TicTacToe - \n";
	TextOut(hdc, UserSettings::OFFSET_X, 10, textToDisplayAsHeader, (int)wcslen(textToDisplayAsHeader));*/

	for (int irow = 0; irow < mBoardSize; irow++)
	{
		for (int icol = 0; icol < mBoardSize; icol++)
		{
			RECT rect = mBoardCoordinates[irow][icol];
			Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		}
	}
	ReleaseDC(mHWnd, hdc);
}

