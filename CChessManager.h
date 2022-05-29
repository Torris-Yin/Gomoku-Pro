#pragma once
#include"CChess.h"
#define MAX_ROWS 15
#define MAX_COLS 15
#define MAX_CHESS MAX_ROWS * MAX_COLS
#define WIN_NUM 5
class CChessManager
{
	CChess m_aChess[MAX_CHESS];
	int m_nChess;
	COLOR m_Color;
	bool CheckRows();
	bool CheckCols();
	bool CheckLSlash();
	bool CheckRSlash();
	int Map[20][20];
	//-1-WHITE 1-BLACK
	int AI_Color;
public:
	CChessManager();
	~CChessManager();
	void NewGame()
	{
		m_nChess = 0;
		m_Color = BLACK;
	}
	bool Xy2Xy(int x0,int y0,int &x1,int &y1);
	int Add(int x,int y);
	void Direct_Add(int x, int y);
	void Set_AI_Color(int color)
	{
		AI_Color = color;
		return;
	}
	void AI_Decision();
	int getScore(const int x, const int y, const int color);
	void Color_Reverse();
	void Map_Reset()
	{
		memset(Map, 0, sizeof(Map));
		return;
	}
	void Regret();
	void Load(int x,int y,COLOR color);
	void Show(CDC* pDC);
	bool GameOver();
	int GetColor();
	void SetColor(int color);
	int GetNum()
	{
		return m_nChess;
	}
	COLOR GetWinner()
	{
		return m_aChess[m_nChess - 1].GetColor();
	}
	CChess* GetQz(int x, int y);
	CChess* GetChess(int n)
	{
		return &m_aChess[n];
	}
	int GetMap(int x, int y);
};