#pragma once
#include "resource.h"
#include "TTTGameManager.h"

class TTTController
{
public:
	int startNewGame();
	static TTTController* getInstance(HINSTANCE hInstance, int nCmdShow);
	static void releaseInstance();
	virtual ~TTTController();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	TTTController(HINSTANCE hInstance, int nCmdShow);
private:
	static TTTController* m_pControllerInstance;
	void actionReplay();
	HINSTANCE m_phInstance;
	HWND m_pHWnd;
	int m_pnCmdShow;
	TTTGameManager* m_ptttGameManager;
	void displayMessageBoxBasedOnResponse(const std::array<int, 3>& resClick) noexcept;
	void ExitGame(HWND hwnd);
	int LbuttonDown(HWND hwnd, LPARAM lParam);
};
