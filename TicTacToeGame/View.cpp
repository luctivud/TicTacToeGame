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
	std::pair<int, int> resBoardBoxClicked = { -1, -1 };
	for (int irow = 0; irow < mBoardSize; irow++)
	{
		for (int icol = 0; icol < mBoardSize; icol++)
		{
			const RECT& rect = mBoardCoordinates[irow][icol];
			if ((xPos > rect.left) && (xPos < rect.right) && (yPos > rect.top) && (yPos < rect.bottom))
			{
				resBoardBoxClicked = { irow, icol };
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

void View::InvalidateMoves()
{
	HDC hdc = GetDC(mHWnd);
	for (int irow = 0; irow < mBoardSize; irow++)
	{
		for (int icol = 0; icol < mBoardSize; icol++)
		{
			RECT rect = mBoardCoordinates[irow][icol];
			rect.left += 4,
			rect.top += 4,
			rect.right -= 4,
			rect.bottom -= 4;
			FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
		}
	}
	ReleaseDC(mHWnd, hdc);
}

View::~View()
{
	HDC hdc = GetDC(mHWnd);
	RECT rect = {
		getRectAtRC(0, 0).left - 2,
		getRectAtRC(0, 0).top - 2,
		getRectAtRC(2, 2).right + 2,
		getRectAtRC(2, 2).bottom + 2
	};
	FillRect(hdc, &rect, CreateSolidBrush(RGB(255, 255, 255)));
	ReleaseDC(mHWnd, hdc);
}

void View::drawAnimatedLine(const std::pair<int, int>& start, const std::pair<int, int>& end, int iStyle, int cWidth, COLORREF cref)
{
	HDC hdc = GetDC(mHWnd);
	HPEN pen = CreatePen(iStyle, cWidth, cref);
	SelectObject(hdc, pen);

	int delayMilliSeconds = 25;
	double sizeUpdate = 20;

	int height = abs(start.first - end.first);
	int base = abs(start.second - end.second);

	double theta = atan(double(base) / double(height));

	std::pair<int, int> currPos = start;
	for (; currPos.first <= end.first and currPos.second <= end.second; )
	{
		std::pair<int, int> prevPos = currPos;
		currPos.first += floor(sizeUpdate * cos(theta));
		currPos.second += floor(sizeUpdate * sin(theta));
		if (currPos.first <= end.first and currPos.second <= end.second)
		{
			Sleep(delayMilliSeconds);
			MoveToEx(hdc, prevPos.first, prevPos.second, NULL);
			LineTo(hdc, currPos.first, currPos.second);
		}
	}

	DeleteObject(pen);
	ReleaseDC(mHWnd, hdc);
}

void View::displayBoard()
{
	RECT rect1 = getRectAtRC(0, 1);
	RECT rect2 = getRectAtRC(2, 1);
	drawAnimatedLine({ rect1.left, rect1.top }, { rect2.left, rect2.bottom }, PS_SOLID, 4, RGB(0, 0, 0));
	drawAnimatedLine({ rect1.right , rect1.top }, { rect2.right, rect2.bottom }, PS_SOLID, 4, RGB(0, 0, 0));
	rect1 = getRectAtRC(1, 0);
	rect2 = getRectAtRC(1, 2);
	drawAnimatedLine({ rect1.left, rect1.top }, { rect2.right, rect2.top }, PS_SOLID, 4, RGB(0, 0, 0));
	drawAnimatedLine({ rect1.left, rect1.bottom }, { rect2.right, rect2.bottom }, PS_SOLID, 4, RGB(0, 0, 0));
}