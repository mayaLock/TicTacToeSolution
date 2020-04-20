/*
	Dipayan Sarker
	April 18, 2020
*/

#include "tictactoecore.h"

TicTacToeCore::TicTacToeCore() 
{
	this->m_gameCount = 0;
	this->m_humanScore = 0;
	this->m_computerScore = 0;
	this->m_drawScore = 0;
	this->m_winnerLine = WINNER_LINE::NONE;
	this->resetLogicalBoard();
}

TicTacToeCore::~TicTacToeCore()
{
}

int TicTacToeCore::getGameCount()
{
	return this->m_gameCount;
}

int TicTacToeCore::getHumanScore()
{
	return this->m_humanScore;
}

int TicTacToeCore::getComputerScore()
{
	return this->m_computerScore;
}

int TicTacToeCore::getDrawScore()
{
	return this->m_drawScore;
}

void TicTacToeCore::setGameCount()
{
	this->m_gameCount++;
}

void TicTacToeCore::setHumanScore()
{
	this->m_humanScore++;
}

void TicTacToeCore::setComputerScore()
{
	this->m_computerScore++;
}

void TicTacToeCore::setDrawScore()
{
	this->m_drawScore++;
}

void TicTacToeCore::reset()
{
	this->resetLogicalBoard();
	this->m_winnerLine = WINNER_LINE::NONE;
	this->m_gameCount++;
}

TicTacToeCore::PLAYER_DATA TicTacToeCore::getPlayerMove(int nPos)
{
	if (nPos < 0 || nPos > 8)
	{
		return PLAYER_DATA::None;
	}
	return this->m_logicalBoard[nPos];
}

TicTacToeCore::WINNER_LINE TicTacToeCore::getWinnerLine()
{
	return this->m_winnerLine;
}

void TicTacToeCore::setPlayerMove(PLAYER_DATA playerData, int nPos)
{
	if (nPos >= 0 || nPos <= 8)
	{
		if (this->isFreeSpot(nPos))
		{
			this->m_logicalBoard[nPos] = playerData;
		}
	}
}

bool TicTacToeCore::isWinner()
{
	if (this->m_logicalBoard[0] != PLAYER_DATA::None && (this->m_logicalBoard[0] & this->m_logicalBoard[1] & this->m_logicalBoard[2]) == this->m_logicalBoard[0])
	{
		this->m_winnerLine = WINNER_LINE::H1;
		return true;
	}
	else if (this->m_logicalBoard[3] != PLAYER_DATA::None && (this->m_logicalBoard[3] & this->m_logicalBoard[4] & this->m_logicalBoard[5]) == this->m_logicalBoard[3])
	{
		this->m_winnerLine = WINNER_LINE::H2;
		return true;
	}
	else if (this->m_logicalBoard[6] != PLAYER_DATA::None && (this->m_logicalBoard[6] & this->m_logicalBoard[7] & this->m_logicalBoard[8]) == this->m_logicalBoard[6])
	{
		this->m_winnerLine = WINNER_LINE::H3;
		return true;
	}
	else if (this->m_logicalBoard[0] != PLAYER_DATA::None && (this->m_logicalBoard[0] & this->m_logicalBoard[3] & this->m_logicalBoard[6]) == this->m_logicalBoard[0])
	{
		this->m_winnerLine = WINNER_LINE::V1;
		return true;
	}
	else if (this->m_logicalBoard[1] != PLAYER_DATA::None && (this->m_logicalBoard[1] & this->m_logicalBoard[4] & this->m_logicalBoard[7]) == this->m_logicalBoard[1])
	{
		this->m_winnerLine = WINNER_LINE::V2;
		return true;
	}
	else if (this->m_logicalBoard[2] != PLAYER_DATA::None && (this->m_logicalBoard[2] & this->m_logicalBoard[5] & this->m_logicalBoard[8]) == this->m_logicalBoard[2])
	{
		this->m_winnerLine = WINNER_LINE::V3;
		return true;
	}
	else if (this->m_logicalBoard[0] != PLAYER_DATA::None && (this->m_logicalBoard[0] & this->m_logicalBoard[4] & this->m_logicalBoard[8]) == this->m_logicalBoard[0])
	{
		this->m_winnerLine = WINNER_LINE::D1;
		return true;
	}
	else if (this->m_logicalBoard[2] != PLAYER_DATA::None && (this->m_logicalBoard[2] & this->m_logicalBoard[4] & this->m_logicalBoard[6]) == this->m_logicalBoard[2])
	{
		this->m_winnerLine = WINNER_LINE::D2;
		return true;
	}
	else
	{
		return false;
	}

	

}

bool TicTacToeCore::isDraw()
{
	for (int i = 0; i < 9; i++)
	{
		if (this->m_logicalBoard[i] != PLAYER_DATA::None)
		{
			continue;
		}
		else
		{
			return false;
		}
	}
	return true;
}

void TicTacToeCore::setHumanMove(int nPos)
{
	this->setPlayerMove(PLAYER_DATA::X, nPos);
}

void TicTacToeCore::setComputerMove(int nPos)
{
	this->setPlayerMove(PLAYER_DATA::O, nPos);
}

void TicTacToeCore::resetLogicalBoard()
{
	for (int i = 0; i < 9; i++)
	{
		this->m_logicalBoard[i] = PLAYER_DATA::None;
	}
}

bool TicTacToeCore::isFreeSpot(int nPos)
{
	if (nPos < 0 || nPos > 8)
	{
		return false;
	}
	return this->m_logicalBoard[nPos] == PLAYER_DATA::None;
}
