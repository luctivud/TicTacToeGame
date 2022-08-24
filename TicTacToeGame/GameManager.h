#pragma once

#include "Model.h"
#include "View.h"
#include <array>

class GameManager
{
public:
	GameManager(HWND hwnd);
	virtual ~GameManager();
	const std::array<int, 3> responseToClick(const int xPos, const int yPos);
	const std::vector<std::pair<int, int>> getValidMovesPlayed();
	const RECT getRectCoordinatesRC(std::pair<int, int> boxClicked);
	bool checkGameOver();
private:
	HWND mHWnd;
	//std::unique_ptr<Model> mpModel;
	Model* mpModel;
	View* mpView;
};

