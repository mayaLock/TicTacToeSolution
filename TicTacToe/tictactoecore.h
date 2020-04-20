/*
	Dipayan Sarker 
	April 18, 2020
*/

#ifndef _TIC_TAC_TOE_CORE_H_
#define _TIC_TAC_TOE_CORE_H_

class TicTacToeCore
{
public:
	typedef enum _playerDataTAG
	{
		None,
		X,
		O
	} PLAYER_DATA;

	typedef enum _winnerLineTAG
	{
		NONE,
		V1,
		V2,
		V3,
		H1,
		H2,
		H3,
		D1,
		D2
	} WINNER_LINE;

	TicTacToeCore();
	~TicTacToeCore();
	int getGameCount();
	int getHumanScore();
	int getComputerScore();
	int getDrawScore();
	void setGameCount();
	void setHumanScore();
	void setComputerScore();
	void setDrawScore();
	void reset();
	PLAYER_DATA getPlayerMove(int nPos);
	WINNER_LINE getWinnerLine();

	bool isWinner();
	bool isDraw();
	void setHumanMove(int nPos);
	void setComputerMove(int nPos);
	bool isFreeSpot(int nPos);
private:
	PLAYER_DATA m_logicalBoard[9];
	int m_gameCount;
	int m_humanScore;
	int m_computerScore;
	int m_drawScore;
	WINNER_LINE m_winnerLine;
	void resetLogicalBoard();	
	void setPlayerMove(PLAYER_DATA playerData, int nPos);
};

#endif // !_TIC_TAC_TOE_CORE_H_



