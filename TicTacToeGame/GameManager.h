#pragma once

#include "resource.h"

#include "Model.h"
#include "View.h"

#include <memory>

/**
* A Singleton Class, 
	since one manager can create model and view for different windows
*/
class GameManager
{
public:
	/**
	* @brief it will return an instance of GameManager which controls Model and View of that corresponding window
	* if there is no such instance, then the instance is created first and returned
	* @param handle to the window
	*/
	static GameManager* getInstance(HWND hwnd);

	/**
	* @brief it will return an instance of GameManager which controls Model and View of that corresponding window
	* if there is no such instance, then the instance is created first and returned
	* @param handle to the window
	*/
	static void	releaseInstance(HWND hwnd);

	/**
	* Starts a new game
	* @param nCmdShow is a parameter passed during intial window creation by the user 
		to open window in fullscreen, maximised, or background mode
	*/
	void startNewGame(int nCmdShow);

	/**
	* @brief checks the position of click and its validity and performs updation in model and view
		the response is of type array, where 
		[0] -> GameStatus, 
		[1] -> row clicked, 
		[2] -> col clicked
	* @param [in] x-coordinate on screen
	* @param [in] y-coordinate on screem
	* @param [in/out] response array to display message box
	*/
	void responseToClick(const int xPos, const int yPos, std::array<int, 3> &resClick);

	/**
	* @brief this method is called when user clicks replay button in the menu
		It gets all the valid moves played so far from the model and
		resets both model and view and then simulates the game played so far.
	*/
	void actionReplay();

	/**
	* @brief this method is called when LBUTTONDOWN event is triggered on the window
		It will perform an action based on the clicks.
	* @param [in] the handle to the current window
	* @param [in] lParam to parse the click to coordinates
	* @returns an integer:
		[1] -> if the game is over and user still click,
		[404] -> there is no instance of manager (rare mistake),
		[0] -> no error
	*/
	int LbuttonDown(HWND hwnd, LPARAM lParam);

	/**
	* @brief Display an exit message and destroy window
	*/
	void ExitGame();

private:
	/**
	* private constructor to prevent the object creation using new
	*/
	GameManager(HWND hwnd);

	/**
	* private destructor to prevent the object deletion using delete
	*/
	~GameManager();

	/**
	* @brief displays a message box based on resClick, 
		if game is over - display game over message and
		display the user who won or tied
	*/
	void displayMessageBoxBasedOnResponse(const std::array<int, 3>& resClick);

	/**
	* @brief static pointer instance of GameManager
	*/
	static GameManager* spManagerInstance;

	std::unique_ptr <Model>		mpModel;
	std::unique_ptr <View>		mpView;

	HWND mHWnd;
};
