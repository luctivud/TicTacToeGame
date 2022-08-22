// TicTacToeGame.cpp : Defines the entry point for the application.
//

#include <Windows.h>
#include <windowsx.h>
#include "TTTController.h"


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	TTTController::getInstance(hInstance, nCmdShow)->startNewGame();
	TTTController::releaseInstance();
	return 0;
}

