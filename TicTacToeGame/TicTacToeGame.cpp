// TicTacToeGame.cpp : Defines the entry point for the application.
//

#include "resource.h"
#include "Controller.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	Controller::getInstance(hInstance, nCmdShow)->startNewGame();
	Controller::releaseInstance();
	return 0;
}

