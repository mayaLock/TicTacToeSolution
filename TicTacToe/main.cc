/*
	Dipayan Sarker
	April 18, 2020
*/

#include "main.h"
#include "resource.h"
#include "tictactoecore.h"

#define WM_COMPUTER_PLAY (WM_USER + 0X0001)

static const TCHAR szClassName[] = _T("TicTacToeWndClass");
static const TCHAR szWindowTitle[] = _T("TicTacToe");
static int gameOn;
static const RECT physicalGameBoard[9] = {
	{39, 26, 127, 74},
	{152, 26, 239, 74},
	{265, 26, 352, 74},
	{39, 96, 127, 149},
	{152, 96, 239, 149},
	{265, 96, 352, 149},
	{39, 179, 127, 229},
	{152, 179, 239, 229},
	{265, 179, 352,229}
};
HINSTANCE hInstanceMain;
HMENU hMenu;
TicTacToeCore ticCore;
PLAYER currentPlayer;
HWND hWnd;

int APIENTRY _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpszCmdLine, int nCmdShow)
{

	// initialize seed
	srand((unsigned int)time(NULL));

	// fill WNDCLASSEX array
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = wndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)::GetStockObject(WHITE_BRUSH);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = szClassName;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	// try register the window class
	if (!RegisterClassEx(&wcex))
	{
		MessageBox(NULL, _T("Failed register window"), _T("Error"), MB_OK | MB_ICONERROR);
		return 1;
	}

	hInstanceMain = hInstance;

	hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MENU1));

	POINT pCord;
	centerWindowScreen(400, 400, &pCord);
	hWnd = CreateWindow(
		szClassName,
		szWindowTitle,
		WS_SYSMENU,
		pCord.x,
		pCord.y,
		400,
		400,
		NULL,
		hMenu,
		hInstance,
		NULL);

	if (!hWnd)
	{
		MessageBox(NULL, _T("Failed create window"), _T("Error"), MB_OK | MB_ICONERROR);
		return 1;
	}

	// display window
	::ShowWindow(hWnd, nCmdShow);
	::UpdateWindow(hWnd);

	// message loop
	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0)
	{
		::TranslateMessage(&msg);
		DispatchMessage(&msg);		
	}

	return (int)msg.wParam;	
}

LRESULT wndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDc;
	int nPos;
	WORD code = LOWORD(wParam);
	switch (uMsg)
	{		
	case WM_COMMAND:		
		switch (code)
		{
		case ID_HELP_EXIT:
			::PostQuitMessage(0);
			break;
		case ID_FILE_NEWGAME:
			createNewGame();			
			break;
		case ID_HELP_ABOUT:
			MessageBox(hWnd, _T("TicTacToe.exe\r\n(C) Dipayan Sarker 2020\r\n"), _T("About"), MB_OK);
			break;
		}
		break;

	case WM_LBUTTONDOWN:
		if (gameOn)
		{
			if (currentPlayer == PLAYER::HUMAN)
			{
				nPos = getPos(LOWORD(lParam), HIWORD(lParam));
				if (nPos == -1)
				{
					MessageBox(NULL, _T("Outsite!"), _T("Warning"), MB_OK | MB_ICONWARNING);
					return 0L;
				}
				if (ticCore.isFreeSpot(nPos))
				{
					ticCore.setHumanMove(nPos);
					hDc = ::GetDC(hWnd);
					drawX(hDc, nPos);
					::ReleaseDC(hWnd, hDc);										
				}
				else
				{
					MessageBox(NULL, _T("Not free spot!"), _T("Warning!"), MB_OK | MB_ICONWARNING);
					return 0L;
				}
				if (ticCore.isWinner())
				{
					hDc = ::GetDC(hWnd);
					drawWinnerLine(hDc);
					::ReleaseDC(hWnd, hDc);
					MessageBox(NULL, _T("You win!"), _T("Winner!"), MB_OK | MB_ICONINFORMATION);
					ticCore.setHumanScore();
					gameOn = 0;
					return 0L;
				}
				if (ticCore.isDraw())
				{
					MessageBox(NULL, _T("It's a draw"), _T("Draw!"), MB_OK | MB_ICONINFORMATION);
					ticCore.setDrawScore();
					gameOn = 0;
					return 0L;
				}
				currentPlayer = PLAYER::COMPUTER;
				SendMessage(hWnd, WM_COMPUTER_PLAY, NULL, NULL);
			}
		}
		break;

	case WM_COMPUTER_PLAY:
		computerMove();
		break;

#ifdef _DEBUG
	case WM_MOUSEMOVE:
		POINT p;
		::GetCursorPos(&p);
		::ScreenToClient(hWnd, &p);
		TCHAR rr[20];
		_stprintf_s(rr, _T("x %d y %d\n"), p.x, p.y);
		OutputDebugString(rr);
		break;
#endif // DEBUG

	case WM_PAINT:
		hDc = ::BeginPaint(hWnd, &ps);	
		drawBoard(hDc);	
		::EndPaint(hWnd, &ps);
		break;

	case WM_DESTROY:
		if (hMenu != NULL)
		{
			::DestroyMenu(hMenu);
		}
		::PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
		break;
	}
	return (LRESULT)0;
}

VOID drawBoard(HDC hDc)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 10, RGB(0, 255, 0));
	HPEN hOldPen = (HPEN)::SelectObject(hDc, hPen);

	// vertical 1
	::MoveToEx(hDc, 140, 15, NULL);
	::LineTo(hDc, 140, 240);
	
	// vertical 2
	::MoveToEx(hDc, 252, 15, NULL);
	::LineTo(hDc, 252, 240);

	// horizontal 1
	::MoveToEx(hDc, 40, 90, NULL);
	::LineTo(hDc, 340, 90);

	// horizontal 2
	::MoveToEx(hDc, 40, 170, NULL);
	::LineTo(hDc, 340, 170);

	::SelectObject(hDc, hOldPen);
	::DeleteObject(hPen);

	for (int i = 0; i < 9; i++)
	{
		TicTacToeCore::PLAYER_DATA iMove = ticCore.getPlayerMove(i);
		if (iMove != TicTacToeCore::PLAYER_DATA::None)
		{
			if (iMove == TicTacToeCore::PLAYER_DATA::X)
			{
				drawX(hDc, i);
			}
			else
			{
				drawO(hDc, 0);
			}
		}
		else
		{
			continue;
		}
	}

	TCHAR score[120];
	_stprintf_s(
		score,
		_T("Game number: \t\t%d\r\nYour score: \t\t%d\r\nComputer's score: \t%d\r\nDraw score: \t\t%d\r\n"), 
		ticCore.getGameCount(),
		ticCore.getHumanScore(),
		ticCore.getComputerScore(),
		ticCore.getDrawScore());
	RECT rc;
	::SetRect(&rc, 15, 270, 250, 350);
	::DrawText(hDc, score, (int)_tcslen(score), &rc, DT_LEFT | DT_EXTERNALLEADING | DT_WORDBREAK | DT_EXPANDTABS);
}

VOID drawX(HDC hDc, int nPos)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 15, RGB(255, 0, 0));
	HPEN hPenOld = (HPEN)::SelectObject(hDc, hPen);

	RECT r = physicalGameBoard[nPos];
	r.left += 10;
	r.top += 10;
	r.right -= 10;
	r.bottom -= 10;

	::MoveToEx(hDc, r.left, r.top, NULL);
	::LineTo(hDc, r.right, r.bottom);
	::MoveToEx(hDc, r.left, r.bottom, NULL);
	::LineTo(hDc, r.right, r.top);

	::SelectObject(hDc, hPenOld);
	::DeleteObject(hPen);
}

VOID drawO(HDC hDc, int nPos)
{
	HPEN hPen = ::CreatePen(PS_SOLID, 10, RGB(0, 0, 255));
	HPEN hPenOld = (HPEN)::SelectObject(hDc, hPen);
	HBRUSH hBrushOld = (HBRUSH)::SelectObject(hDc, ::GetStockObject(NULL_BRUSH));

	RECT r = physicalGameBoard[nPos];
	r.left += 10;
	r.top += 10;
	r.right -= 10;
	r.bottom -= 10;

	::Ellipse(hDc, r.left, r.top, r.right, r.bottom);

	::SelectObject(hDc, hPenOld);
	::SelectObject(hDc, hBrushOld);
	::DeleteObject(hPen);
}

VOID drawWinnerLine(HDC hDc)
{

	HPEN hPen = ::CreatePen(PS_SOLID, 10, RGB(0, 0, 0));
	HPEN hPenOld = (HPEN)::SelectObject(hDc, hPen);

	TicTacToeCore::WINNER_LINE winLine = ticCore.getWinnerLine();
	switch (winLine)
	{
	case TicTacToeCore::WINNER_LINE::V1:
		// v1
		::MoveToEx(hDc, 84, 22, NULL);
		::LineTo(hDc, 84, 230);
		break;
	case TicTacToeCore::WINNER_LINE::V2:
		// v2
		::MoveToEx(hDc, 195, 22, NULL);
		::LineTo(hDc, 195, 230);
		break;
	case TicTacToeCore::WINNER_LINE::V3:
		// v3
		::MoveToEx(hDc, 306, 22, NULL);
		::LineTo(hDc, 306, 230);
		break;
	case TicTacToeCore::WINNER_LINE::H1:
		// h1
		::MoveToEx(hDc, 41, 49, NULL);
		::LineTo(hDc, 339, 49);
		break;
	case TicTacToeCore::WINNER_LINE::H2:
		// h2
		::MoveToEx(hDc, 41, 121, NULL);
		::LineTo(hDc, 339, 121);
		break;
	case TicTacToeCore::WINNER_LINE::H3:
		// h3
		::MoveToEx(hDc, 41, 201, NULL);
		::LineTo(hDc, 339, 201);
		break;
	case TicTacToeCore::WINNER_LINE::D1:
		// d1
		::MoveToEx(hDc, 45, 24, NULL);
		::LineTo(hDc, 334, 216);
		break;
	case TicTacToeCore::WINNER_LINE::D2:
		// d2
		::MoveToEx(hDc, 41, 224, NULL);
		::LineTo(hDc, 338, 22);
		break;
	default:
		break;
	}

	::SelectObject(hDc, hPenOld);
	::DeleteObject(hPen);
}

INT getPos(int x, int y)
{
	for (int i = 0; i < 9; i++)
	{
		if (x > physicalGameBoard[i].left
			&& x < physicalGameBoard[i].right
			&& y > physicalGameBoard[i].top
			&& y < physicalGameBoard[i].bottom)
		{
			return i;
		}
	}
	return -1;
}

INT getComputerChoice()
{
	int nPos = 10;
	while (!ticCore.isDraw() || !ticCore.isWinner() || ticCore.isFreeSpot(nPos))
	{
		nPos = rand() % 9;
		if (ticCore.isFreeSpot(nPos))
		{
			ticCore.setComputerMove(nPos);
			break;
		}
	}
	return nPos;
}

LRESULT computerMove()
{
	if (currentPlayer == PLAYER::COMPUTER)
	{
		int nChoice = getComputerChoice();
		HDC hDc = ::GetDC(hWnd);
		drawO(hDc, nChoice);
		::ReleaseDC(hWnd, hDc);
		if (ticCore.isWinner())
		{
			hDc = ::GetDC(hWnd);
			drawWinnerLine(hDc);
			::ReleaseDC(hWnd, hDc);
			MessageBox(NULL, _T("Computer Wins!"), _T("Winner!"), MB_OK | MB_ICONINFORMATION);
			ticCore.setComputerScore();
			gameOn = 0;
			return 0L;
		}
		if (ticCore.isDraw())
		{
			MessageBox(NULL, _T("It's a draw!"), _T("Draw!"), MB_OK | MB_ICONINFORMATION);
			ticCore.setDrawScore();
			gameOn = 0;
			return 0L;
		}		
		currentPlayer = PLAYER::HUMAN;
		return 0L;
	}
	return 0L;
}

VOID reset()
{
	ticCore.reset();
	RECT rect;
	::GetClientRect(hWnd, &rect);
	::InvalidateRect(hWnd, &rect, TRUE);
}

VOID createNewGame()
{
	reset();
	gameOn = 1;
	int nPlayerChoice = MessageBox(hWnd, _T("Click \"YES\" if you want to start or \"NO\" to let computer start!"), _T("Info"), MB_YESNO | MB_ICONINFORMATION);
	if (nPlayerChoice == IDYES)
	{
		currentPlayer = PLAYER::HUMAN;
	}
	else
	{
		currentPlayer = PLAYER::COMPUTER;
		computerMove();
	}
}

VOID centerWindowScreen(int nRight, int nBottom, LPPOINT lpPoint)
{
	ZeroMemory(lpPoint, sizeof(LPPOINT));
	int nScreenX = GetSystemMetrics(SM_CXSCREEN);
	int nScreenY = GetSystemMetrics(SM_CYSCREEN);
	lpPoint->x = (nScreenX / 2) - (nRight / 2);
	lpPoint->y = (nScreenY / 2) - (nBottom / 2);
}
