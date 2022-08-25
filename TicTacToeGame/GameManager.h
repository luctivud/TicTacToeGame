#pragma once

#include "resource.h"

#include "Model.h"
#include "View.h"

#include <memory>

class GameManager
{
public:
	
	static GameManager* getInstance(HWND hwnd);
	static void releaseInstance();

	~GameManager();

	/**
	* Starts a new game
	*/
	void startNewGame(int nCmdShow);
	const std::array<int, 3> responseToClick(const int xPos, const int yPos);
	const std::vector<std::pair<int, int>> getValidMovesPlayed();
	const RECT getRectCoordinatesRC(std::pair<int, int> boxClicked);
	void actionReplay();
	int LbuttonDown(HWND hwnd, LPARAM lParam);
	void ExitGame();
private:
	GameManager(HWND hwnd);
	void displayMessageBoxBasedOnResponse(const std::array<int, 3>& resClick);

	static GameManager* spManagerInstance;

	std::unique_ptr <Model> mpModel;
	std::unique_ptr <View> mpView;

	HWND mHWnd;
	
};
