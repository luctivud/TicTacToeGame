#include "GameManager.h"

#include <Windowsx.h>
#include <string>
#include <array>
#include <chrono>
#include <thread>

GameManager* GameManager::spManagerInstance = nullptr;

GameManager* GameManager::getInstance(HWND hwnd)
{
	if (spManagerInstance == nullptr)
		spManagerInstance = new GameManager(hwnd);
	return spManagerInstance;
}

void GameManager::releaseInstance(HWND hwnd)
{
	if (spManagerInstance != nullptr)
		delete spManagerInstance;
	spManagerInstance = nullptr;
}

GameManager::GameManager(HWND hwnd)
	: mHWnd(hwnd)
	, mpModel(new Model())
	, mpView(new View(hwnd, mpModel->getBoardSize()))
{
}

GameManager::~GameManager()
{
}

void GameManager::startNewGame(int nCmdShow)
{
	mpView->displayBoard();

	MSG msg = { };
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
}

void GameManager::responseToClick(const int xPos, const int yPos, std::array<int, 7>& responseArray) // why return by value
{
	responseArray = { -1, -1, -1 };

	if (mpModel->updateTurn(false) >= mpModel->getBoardSize() * mpModel->getBoardSize())
		return;

	std::pair<int, int> boxClicked = mpView->checkIfClickOnBoard(xPos, yPos);

	if ((boxClicked.first != -1) and (boxClicked.second != -1) and
		(mpModel->isValidMove(boxClicked))) // click was on Board
	{
		mpModel->updateTurn(true);
		mpModel->addToValidMoves(boxClicked);
		mpModel->updateBoard(boxClicked);
		mpView->updateBoard(boxClicked, mpModel->updateTurn(false));

		mpModel->checkIfGameEnded(boxClicked, responseArray);
	}
	else
	{
		boxClicked = { -1, -1 };
	}

	responseArray[1] = boxClicked.first;
	responseArray[2] = boxClicked.second;

	displayMessageBoxBasedOnResponse(responseArray);

	drawLineIfWin(responseArray);

	return;
}

void GameManager::displayMessageBoxBasedOnResponse(const std::array<int, 7>& resClick)
{
#ifdef _DEBUG
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

		int msgBoxID = MessageBox
		(
			mHWnd,
			textToDisplay,
			L"Game Over!!",
			MB_OK//MB_CANCELTRYCONTINUE | MB_DEFBUTTON2
		);
	}
#endif
}

void GameManager::drawLineIfWin(const std::array<int, 7>& resClick)
{
	if (resClick[0] == -1)
		return;
	RECT rect1 = mpView->getRectAtRC(resClick[3], resClick[4]);
	RECT rect2 = mpView->getRectAtRC(resClick[5], resClick[6]);
	mpView->drawAnimatedLine(
		{ (rect1.left + rect1.right) / 2, (rect1.top + rect1.bottom) / 2 }, 
		{ (rect2.left + rect2.right) / 2, (rect2.top + rect2.bottom) / 2 },
		PS_SOLID, 2, UserSettings::COLOR_USER[mpModel->updateTurn(false) % 2]);
}


int GameManager::LbuttonDown(HWND hwnd, LPARAM lParam)
{
	if (mpModel->getIsGameOver())
		return 1;
	int xPos = GET_X_LPARAM(lParam);
	int yPos = GET_Y_LPARAM(lParam);
	if (spManagerInstance == nullptr)
		return 404;

	std::array<int, 7> resClick;
	responseToClick(xPos, yPos, resClick);
	if (resClick[1] != -1)
	{
		HMENU hmenu = LoadMenuA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(IDI_TICTACTOEGAME));
		EnableMenuItem(hmenu, ID_REPLAY, MF_ENABLED);
		SetMenu(mHWnd, hmenu);
	}
	return 0;
}

void GameManager::actionReplay()
{
	HMENU hmenu = LoadMenuA(GetModuleHandleA(NULL), MAKEINTRESOURCEA(IDI_TICTACTOEGAME));
	EnableMenuItem(hmenu, ID_REPLAY, MF_DISABLED);
	SetMenu(mHWnd, hmenu);
	const std::vector<std::pair<int, int>> validMoves = mpModel->getValidMovesPlayed();
	mpModel.reset(new Model());
	mpView->InvalidateMoves();
	for (const std::pair<int, int>& move : validMoves)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(500));
		const RECT rect = mpView->getRectAtRC(move.first, move.second);
		std::pair<int, int> coordinatesToClick = { (rect.left + rect.right) / 2, (rect.top + rect.bottom) / 2 };
		std::array<int, 7> resClick;
		responseToClick(coordinatesToClick.first, coordinatesToClick.second, resClick);
	}
	EnableMenuItem(hmenu, ID_REPLAY, MF_ENABLED);
	SetMenu(mHWnd, hmenu);
}

void GameManager::ExitGame()
{
	if (MessageBox(mHWnd, L"Do you want to quit?", L"Quit - X", MB_OKCANCEL) == IDOK)
		DestroyWindow(mHWnd);
	return;
}