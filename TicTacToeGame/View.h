#pragma once
#include <windows.h>
#include <vector>
class View
{
public:
	/**
	* \brief create a view on the user interface (window) determined by hwnd and boardsize
	* \param hwnd -- handle to window
	* \param boardSize -- size of the board
	*/
	View(HWND hwnd, const int boardSize);

	/**
	* \brief destructor of the View class
	*/
	~View();

	/**
	* \brief displays the board by updateWindow and then creating the board
	*/
	void displayBoard();

	/**
	* \brief get coordinates of boards
	* \return vector of RECT of the board cells
	*/
	const std::vector<std::vector<RECT>>& getBoardCoordinates();

	/**
	* \brief checks if the click was on board or not and if yes returns the row and column of cell
	* \param xPos -- x-coordinate of click
	* \param yPos -- y-coordinate of click
	* \return pair {row, column} for the cell in which click happened
	*/
	std::pair<int, int> checkIfClickOnBoard(const int xPos, const int yPos);

	/**
	* \brief update the board depending upon the turn
	* \param boxClicked -- pair of {row, column} in which cell has to be updated
	* \param turn -- current turn number
	*/
	void updateBoard(const std::pair<int, int>& boxClicked, const int turn);

	/**
	* \brief get the RECT coordinates at any cell
	* \param row -- row of cell
	* \param col -- column of cell
	* \return RECT structure representing the coordinates
	*/
	const RECT getRectAtRC(const int row, const int col);
	void InvalidateMoves();
	void drawAnimatedLine(
		const std::pair<int, int>& start,
		const std::pair<int, int>& end,
		int iStyle, int cWidth, COLORREF cref
	);

private:
	void drawCircle(HDC hdc, RECT rect);
	void drawCross(HDC hdc, RECT rect);
	

	HWND	mHWnd;
	int		mBoardSize;

	std::vector<std::vector<RECT>>	mBoardCoordinates;
};


/**
 * \todo read all the settings from a file (such as config file) and store it in this namespace.
 */
namespace UserSettings
{
	constexpr int		SZ_SQUARE = 120;
	constexpr int		OFFSET_X = 40;
	constexpr int		OFFSET_Y = 40;
	constexpr int		OFFSET_BORDER = 20;
	constexpr COLORREF	COLOR_USER[] = { RGB(0, 0, 255), RGB(255, 0, 0) };
}

