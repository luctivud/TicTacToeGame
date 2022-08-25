#pragma once

#include "resource.h"

#include <vector>

class Model
{
public:
	/**
	* @brief creates a 3x3 model that stores data for the tictactoe game
		sets turns to zero and
		gameover status to false
	*/
	Model();

	/**
	* destructor to the model class
	*/
	virtual ~Model();

	/**
	 * \brief getter for the board size of current board, in terms of numbers of rows/columns.
	 * \return int -- boardSize
	 */
	const int& getBoardSize();

	/**
	 * get the status of game.
	 * 
	 * \return true -- if the game is over
	 * \return false -- if the game is not over
	 */
	bool getIsGameOver();

	/**
	* @brief if param is true -- updates the turn, i.e, increase by 1 and returns it
	* @brief if param is false -- returns the current turn
	* @param boolean to indicate if the turn needs to be updated or not
	* @returns current/updated turn based on param
	*/
	const int& updateTurn(bool ifUpdateTurn) noexcept;

	/**
	* @brief checks if the current move is valid as it does not manipulate an already filled box
	* @param pair of ints {row, col} to define the cell to check
	*/
	bool isValidMove(const std::pair<int, int>& boxClicked);

	/**
	* @brief update the board based on the turn if it is odd or even
	* @param pair of ints {row, col} to define the cell to update
	*/
	void updateBoard(const std::pair<int, int>& boxClicked);

	/**
	* @brief checks if after updation of current cell is the game over or not
	* @param pair of ints {row, col} to define the cell which was last updated
	*/
	int checkIfGameEnded(const std::pair<int, int>& boxClicked);

	/**
	* \brief adds the current move to the vector of valid moves
	* \param boxClicked -- pair of ints {row, col} to define the cell which was last updated
	* \return int -- 0 or 1 if any user won, 4 if the game ended in draw or -1 if game has not ended
	*/
	void addToValidMoves(const std::pair<int, int>& boxClicked) noexcept;

	/**.
	* \brief get all the valid moves played in the current game
	* \return vector of all the moves played
	* \todo pass by reference made possible
	*/
	const std::vector<std::pair<int, int>> getValidMovesPlayed() noexcept;
private:

	/**
	* \brief helper method to check if the update ended the game in a win.
	*
	* \param boxClicked -- pair of ints {row, col} to define the cell which was last updated
	* \return int -- 0 or 1 if any user won, -1 otherwise
	*/
	int checkWin(const std::pair<int, int>& boxClicked);

	/**
	* \brief helper method to check if the update ended the game in a draw.
	*
	* \param boxClicked -- pair of ints {row, col} to define the cell which was last updated
	* \return int -- 4 if game ended in draw, -1 otherwise
	*/
	int checkDraw(const std::pair<int, int>& boxClicked);

	static const int mBoardSize;

	bool	mIsGameOver;
	int		mTurn = 0;

	std::vector<std::vector<char>>		m_pBoardEntries;
	std::vector<std::pair<int, int>>	m_pValidMovesPlayed;
};
