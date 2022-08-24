#pragma once
#include "resource.h"
#include "GameManager.h"

class Controller
{
public:
	/*
	* Starts a new game
	* @returns int NonZero value if successful, 0 otherwise
	*/
	int startNewGame();

	static Controller* getInstance(HINSTANCE hInstance, int nCmdShow);
	
	static void releaseInstance();
	virtual ~Controller();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	Controller(HINSTANCE hInstance, int nCmdShow); //mke it private
private:
	
	void actionReplay();
	void displayMessageBoxBasedOnResponse(const std::array<int, 3>& resClick) noexcept;
	void ExitGame(HWND hwnd);
	int LbuttonDown(HWND hwnd, LPARAM lParam);

	static Controller* spControllerInstance;

	HINSTANCE mHInstance;
	HWND mHWnd = nullptr;
	int mnCmdShow;
	GameManager* mpGameManager;
	
};
