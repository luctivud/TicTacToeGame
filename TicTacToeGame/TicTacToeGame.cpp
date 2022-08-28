// TicTacToeGame.cpp : Defines the entry point for the application.
//

#include "resource.h"
#include "GameManager.h"


LRESULT WindowProc					
(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND	registerClassAndCreateWindow
(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow);


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
	HWND hwnd = registerClassAndCreateWindow(hInstance, pCmdLine, nCmdShow);
	if (hwnd == 0)
		return 0;

	HMENU hmenu = LoadMenuA(hInstance, MAKEINTRESOURCEA(IDI_TICTACTOEGAME));
	SetMenu(hwnd, hmenu);

	ShowWindow(hwnd, nCmdShow);

	GameManager::getInstance(hwnd)->startNewGame(nCmdShow);
	GameManager::releaseInstance(hwnd);

	DestroyWindow(hwnd);

	return 0;
}

LRESULT WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEWGAME32772:
		{
			GameManager::releaseInstance(hwnd);
			GameManager::getInstance(hwnd)->startNewGame(SW_SHOWDEFAULT);
		}
		break;

		case ID_EXIT:
			GameManager::getInstance(hwnd)->ExitGame();
			break;
		case ID_REPLAY:
			GameManager::getInstance(hwnd)->actionReplay();
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_LBUTTONDOWN:
	{
		GameManager::getInstance(hwnd)->LbuttonDown(hwnd, lParam);
	}
	break;

	case WM_CLOSE:
	{
		GameManager::getInstance(hwnd)->ExitGame();
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND registerClassAndCreateWindow(HINSTANCE hInstance, PWSTR pCmdLine, int nCmdShow)
{
	WNDCLASS wc = { };
	const wchar_t CLASS_NAME[] = L"TicTacToe";
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	return CreateWindowW
	(
		CLASS_NAME,
		L"TicTacToeGame",
		WS_OVERLAPPED | WS_SYSMENU,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL
	);
}
