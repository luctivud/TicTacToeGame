#pragma once

#include "TTTModel.h"
#include "TTTView.h"

class TTTGameManager
{
public:
	TTTGameManager(HWND hwnd);
	virtual ~TTTGameManager();
	const std::pair<int, int> responseToClick(const int xPos, const int yPos);
private:
	HWND m_pHWnd;
	TTTModel* m_pTTTModel;
	TTTView* m_pTTTView;
};

