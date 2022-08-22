#include "TTTController.h"
#include <Windowsx.h>
#include <string>

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

void TTTController::releaseInstance()
{
	if (m_pControllerInstance != nullptr)
		delete m_pControllerInstance;
	m_pControllerInstance = nullptr;
}

TTTController::~TTTController()
{
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

		const std::pair<int, int> boxClicked = m_pControllerInstance->m_ptttGameManager->responseToClick(xPos, yPos);
#if _DEBUG
		if (boxClicked.first != -1 and boxClicked.second != -1) 
		{
			std::string strTextToDisplay = "Click on Rectange - , row: " + std::to_string(boxClicked.first)
				+ ", col: " + std::to_string(boxClicked.second) + "\n";
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

