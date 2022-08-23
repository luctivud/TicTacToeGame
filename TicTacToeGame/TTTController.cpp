#include "TTTController.h"
#include <Windowsx.h>
#include <string>
#include <array>

TTTController* TTTController::m_pControllerInstance = nullptr;

TTTController* TTTController::getInstance(HINSTANCE hInstance, int nCmdShow)
{
	if (m_pControllerInstance == nullptr)
		m_pControllerInstance = new TTTController(hInstance, nCmdShow);
	return m_pControllerInstance;
}

TTTController::TTTController(HINSTANCE hInstance, int nCmdShow) : m_phInstance(hInstance), m_pnCmdShow(nCmdShow), m_ptttGameManager(nullptr)
{
	WNDCLASS wc = { };
	const wchar_t CLASS_NAME[] = L"TicTacToe";
	wc.lpfnWndProc = TTTController::WindowProc;
	wc.hInstance = m_phInstance;
	wc.lpszClassName = CLASS_NAME;
	RegisterClass(&wc);

	HWND hwnd = CreateWindowW(
		CLASS_NAME,
		L"TicTacToeGame",
		WS_TILEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		m_phInstance,
		NULL
	);

	if (hwnd == 0)
		return;// todo_uditg : throw new exception and handle it.

	m_pHWnd = hwnd;
}

void TTTController::actionReplay()
{
	const std::vector<std::pair<int, int>> validMoves = m_ptttGameManager->getValidMovesPlayed();
	delete m_ptttGameManager;
	m_ptttGameManager = new TTTGameManager(m_pHWnd);
	for (const std::pair<int, int>& move : validMoves)
	{
		Sleep(1000);
		std::pair<int, int> coordinatesToClick = m_ptttGameManager->getCoordinatesForActionReplay(move);
		const std::array<int, 3> resClick = m_ptttGameManager->responseToClick(coordinatesToClick.first, coordinatesToClick.second);
		displayMessageBoxBasedOnResponse(resClick);
	}
}

void TTTController::displayMessageBoxBasedOnResponse(const std::array<int, 3>& resClick) noexcept
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
		strTextToDisplay += "\nClick Continue to get an action replay, Cancel to exit.";
		std::wstring temp = std::wstring(strTextToDisplay.begin(), strTextToDisplay.end());
		LPCWSTR textToDisplay = (LPCWSTR)temp.c_str();

		int msgBoxID = (MessageBox(
			m_pControllerInstance->m_pHWnd,
			textToDisplay,
			L"Game Over!!",
			MB_CANCELTRYCONTINUE | MB_DEFBUTTON2)
			);

		switch (msgBoxID)
		{
		case IDCANCEL:
			PostQuitMessage(0);
			break;
		case IDTRYAGAIN:
		{
			HINSTANCE cpyhInstance = m_pControllerInstance->m_phInstance;
			int cpynCmdShow = m_pControllerInstance->m_pnCmdShow;
			TTTController::releaseInstance();
			TTTController::getInstance(cpyhInstance, cpynCmdShow)->startNewGame();
		}
		break;
		case IDCONTINUE:
		{
			m_pControllerInstance->actionReplay();
		}
		break;
		}
	}
}


void TTTController::releaseInstance()
{
	if (m_pControllerInstance != nullptr)
		delete m_pControllerInstance;
	m_pControllerInstance = nullptr;
}

TTTController::~TTTController()
{
	DestroyWindow(m_pHWnd);
	delete m_ptttGameManager;
}

int TTTController::startNewGame()
{
	ShowWindow(m_pHWnd, m_pnCmdShow);

	m_ptttGameManager = new TTTGameManager(m_pHWnd);

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 1;
}

LRESULT TTTController::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

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
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		if (m_pControllerInstance == nullptr or m_pControllerInstance->m_ptttGameManager == nullptr)
			return 404;

		const std::array<int, 3> resClick = m_pControllerInstance->m_ptttGameManager->responseToClick(xPos, yPos);
#if _DEBUG
		if (resClick[1] != -1 and resClick[2] != -1)
		{
			std::string strTextToDisplay = "Click on Rectange - , row: " + std::to_string(resClick[1])
				+ ", col: " + std::to_string(resClick[2]) + "\n";
			std::wstring temp = std::wstring(strTextToDisplay.begin(), strTextToDisplay.end());
			LPCWSTR textToDisplay = (LPCWSTR)temp.c_str();
			MessageBox(
				m_pControllerInstance->m_pHWnd,
				textToDisplay,
				L"Message Box",
				MB_OK
			);
		}
#endif
		m_pControllerInstance->displayMessageBoxBasedOnResponse(resClick);
	}
	break;

	case WM_CLOSE:
	{
		if (MessageBox(hwnd, L"Do you want to quit?", L"Quit - X", MB_OKCANCEL) == IDOK)
			DestroyWindow(hwnd);
		// Else: User canceled. Do nothing.
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

LRESULT TTTController::ChildWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hwnd, &ps);
		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));
		EndPaint(hwnd, &ps);
	}
	break;

	case WM_CLOSE:
	{
		if (MessageBox(hwnd, L"Do you want to quit?", L"Quit - X", MB_OKCANCEL) == IDOK)
			DestroyWindow(hwnd);
		// Else: User canceled. Do nothing.
		return 0;
	}
	}
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}