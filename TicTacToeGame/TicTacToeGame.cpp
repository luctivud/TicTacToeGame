// TicTacToeGame.cpp : Defines the entry point for the application.
//

#include "resource.h"
#include "TTTController.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	TTTController::getInstance(hInstance, nCmdShow)->startNewGame();
	TTTController::releaseInstance();
	return 0;
}

