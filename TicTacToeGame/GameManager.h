#pragma once

#include "resource.h"

#include "Model.h"
#include "View.h"

#include <memory>

class GameManager
{
public:
	
	static GameManager* getInstance(HWND hwnd);
	static void			releaseInstance();

	~GameManager();

	/*
	* Starts a new game
	* @param nCmdShow is a parameter passed during intial window creation by the user 
		to open window in fullscreen, maximised, or background mode
	*/
	void startNewGame(int nCmdShow);

	/*
	* @param [in] x-coordinate on screen
	* @param [in] y-coordinate on screem
	* @param [in/out] response array to display message box
	* @brief checks the position of click and its validity and performs updation in model and view
	* the response is of type array, where 
	* [0] -> GameStatus, 
	* [1] -> row clicked, 
	* [2] -> col clicked
	*/
	void responseToClick(const int xPos, const int yPos, std::array<int, 3> &resClick);

	/*
	* 
	*/
	const std::vector<std::pair<int, int>> getValidMovesPlayed();
	const RECT getRectCoordinatesRC(std::pair<int, int> boxClicked);
	void actionReplay();
	int LbuttonDown(HWND hwnd, LPARAM lParam);
	void ExitGame();

private:
	GameManager(HWND hwnd);
	void displayMessageBoxBasedOnResponse(const std::array<int, 3>& resClick);

	static GameManager* spManagerInstance;

	std::unique_ptr <Model>		mpModel;
	std::unique_ptr <View>		mpView;

	HWND mHWnd;
};
