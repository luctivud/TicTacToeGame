#pragma once

#include "TTTModel.h"
#include "TTTView.h"
#include <array>

class TTTGameManager
{
public:
	TTTGameManager(HWND hwnd);
	virtual ~TTTGameManager();
	const std::array<int, 3> responseToClick(const int xPos, const int yPos);
	const std::vector<std::pair<int, int>> getValidMovesPlayed();
	const std::pair<int, int> getCoordinatesForActionReplay(std::pair<int, int> boxClicked);
private:
	HWND m_pHWnd;
	TTTModel* m_pTTTModel;
	TTTView* m_pTTTView;
};

