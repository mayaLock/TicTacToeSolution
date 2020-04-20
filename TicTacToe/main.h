/*
	Dipayan Sarker
	April 18, 2020
*/

#ifndef _TIC_TAC_TOE_MAIN_H_
#define _TIC_TAC_TOE_MAIN_H_

#include <Windows.h>
#include <tchar.h>
#include <time.h>
#include <stdlib.h>

typedef enum _playerTAG
{
	HUMAN,
	COMPUTER
} PLAYER;

LRESULT CALLBACK wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
VOID drawBoard(HDC hDc);
VOID drawX(HDC hDc, int nPos);
VOID drawO(HDC hDc, int nPos);
VOID drawWinnerLine(HDC hDc);
INT getPos(int x, int y);
INT getComputerChoice();
LRESULT computerMove();
VOID reset();
VOID createNewGame();
VOID centerWindowScreen(int nRight, int nBottom, LPPOINT lpPoint);

#endif // !_TIC_TAC_TOE_MAIN_H_