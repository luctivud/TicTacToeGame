#include "Controller.h"
#include <Windowsx.h>
#include <string>
#include <array>

Controller* Controller::spControllerInstance = nullptr;

Controller* Controller::getInstance(HINSTANCE hInstance, int nCmdShow)
{
	if (spControllerInstance == nullptr)
		spControllerInstance = new Controller(hInstance, nCmdShow);
	return spControllerInstance;
}

Controller::Controller(HINSTANCE hInstance, int nCmdShow) : mHInstance(hInstance), mnCmdShow(nCmdShow), mpGameManager(nullptr)
{
	WNDCLASS wc = { };
	const wchar_t CLASS_NAME[] = L"TicTacToe";
	wc.lpfnWndProc = Controller::WindowProc;
	wc.hInstance = mHInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	HWND hwnd = CreateWindowW(
		CLASS_NAME,
		L"TicTacToeGame",
		WS_TILEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		mHInstance,
		NULL
	);

	if (hwnd == 0)
		return;// todo_uditg : throw new exception and handle it.

	mHWnd = hwnd;
}

void Controller::actionReplay()
{
	const std::vector<std::pair<int, int>> validMoves = mpGameManager->getValidMovesPlayed();
	delete mpGameManager;
	mpGameManager = new GameManager(mHWnd);
	for (const std::pair<int, int>& move : validMoves)
	{
		Sleep(500);
		const RECT rect = mpGameManager->getRectCoordinatesRC(move);
		std::pair<int, int> coordinatesToClick = { (rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2 };
		const std::array<int, 3> resClick = mpGameManager->responseToClick(coordinatesToClick.first, coordinatesToClick.second);
		displayMessageBoxBasedOnResponse(resClick);
	}
}

void Controller::displayMessageBoxBasedOnResponse(const std::array<int, 3>& resClick) noexcept
{
	if (resClick[0] != -1) // Game ended
	{
		std::string strTextToDisplay = "Game--Over!! ";
		if (resClick[0] < 2) // Game was won
		{
			strTextToDisplay += "User :: " + std::to_string(resClick[0]) + " Won!!";
		}
		else // Game ended in draw
		{
			strTextToDisplay += "Ended in a Draw!!";
		}
		//strTextToDisplay += "\nClick Continue to get an action replay, Cancel to exit.";
		std::wstring temp = std::wstring(strTextToDisplay.begin(), strTextToDisplay.end());
		LPCWSTR textToDisplay = (LPCWSTR)temp.c_str();

		int msgBoxID = MessageBox(
			spControllerInstance->mHWnd,
			textToDisplay,
			L"Game Over!!",
			MB_OK//MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
		);

		/*switch (msgBoxID)
		{
		case IDCANCEL:
			PostQuitMessage(0);
			break;
		case IDTRYAGAIN:
		{
		}
		break;
		case IDCONTINUE:
		{
			spControllerInstance->actionReplay();
		}
		break;
		}*/
	}
}

void Controller::releaseInstance()
{
	if (spControllerInstance != nullptr)
		delete spControllerInstance;
	spControllerInstance = nullptr;
}

Controller::~Controller()
{
	DestroyWindow(mHWnd);
	delete mpGameManager;
}

int Controller::startNewGame()
{
	ShowWindow(mHWnd, mnCmdShow);

	mpGameManager = new GameManager(mHWnd);

	RECT rect = mpGameManager->getRectCoordinatesRC({ 2, 2 });

	SetWindowPos(mHWnd, 0, 0, 0, rect.right + 60, rect.bottom + 75, SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER);

	HMENU hmenu = LoadMenuA(mHInstance, MAKEINTRESOURCEA(IDI_TICTACTOEGAME));
	
	EnableMenuItem(hmenu, ID_REPLAY, MF_ENABLED);
	SetMenu(mHWnd, hmenu);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 1;
}

int Controller::LbuttonDown(HWND hwnd, LPARAM lParam)
{
	if (mpGameManager->checkGameOver())
		return 1;
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if (spControllerInstance == nullptr or spControllerInstance->mpGameManager == nullptr)
		return 404;

	const std::array<int, 3> resClick = spControllerInstance->mpGameManager->responseToClick(xPos, yPos);
#if _DEBUG
	if (resClick[1] != -1 and resClick[2] != -1)
	{
		std::string strTextToDisplay = "Click on Rectange - , row: " + std::to_string(resClick[1])
			+ ", col: " + std::to_string(resClick[2]) + "\n";
		std::wstring temp = std::wstring(strTextToDisplay.begin(), strTextToDisplay.end());
		LPCWSTR textToDisplay = (LPCWSTR)temp.c_str();
		MessageBox(
			spControllerInstance->mHWnd,
			textToDisplay,
			L"Message Box",
			MB_OK
		);
	}
#endif
	spControllerInstance->displayMessageBoxBasedOnResponse(resClick);
	return 0;
}

void Controller::ExitGame(HWND hwnd)
{
	if (MessageBox(hwnd, L"Do you want to quit?", L"Quit - X", MB_OKCANCEL) == IDOK)
		DestroyWindow(hwnd);
}

LRESULT Controller::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case ID_NEWGAME32772:
		{
			HINSTANCE cpyhInstance = spControllerInstance->mHInstance;
			int cpynCmdShow = spControllerInstance->mnCmdShow;
			Controller::releaseInstance();
			Controller::getInstance(cpyhInstance, cpynCmdShow)->startNewGame();
			spControllerInstance->startNewGame();
		}
		break;

		case ID_EXIT:
			spControllerInstance->ExitGame(hwnd);  // application-defined
			break;
		case ID_REPLAY:
			spControllerInstance->actionReplay();
			break;
		}
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
	break;

	case WM_LBUTTONDOWN:
	{
		spControllerInstance->LbuttonDown(hwnd, lParam);
	}
	break;

	case WM_CLOSE:
	{
		spControllerInstance->ExitGame(hwnd);
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}