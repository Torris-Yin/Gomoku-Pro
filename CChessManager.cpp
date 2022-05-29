#include "afx.h"
#include "pch.h"
#include "CChessManager.h"
#include "GomokuDlg.h"
#include "CChess.h"
#include <vector>
#include <cmath>
using namespace std;

CChessManager::CChessManager()
{
	m_nChess = 0;
	m_Color = BLACK;
	memset(Map,0,sizeof(Map));
}
CChessManager::~CChessManager()
{

}
bool CChessManager::Xy2Xy(int x0, int y0, int& x1, int& y1)
{
	int x, y;
	for(int i=0;i<15;i++)
		for (int j = 0; j < 15; j++)
		{
			x = (int)(CChess::m_dx + i * CChess::m_d + CChess::m_d * 0.5);
			y = (int)(CChess::m_dy + j * CChess::m_d + CChess::m_d * 0.5);
			if (sqrt((x - x0) * (x - x0) + (y - y0) * (y - y0)) < 15)
			{
				x1 = i, y1 = j;
				return true;
			}
		}
	return false;
}

int CChessManager::GetColor()
{
	return m_Color;
}

void CChessManager::SetColor(int color)
{
	m_Color=(COLOR)color;
}

int CChessManager::Add(int x, int y)
{
	int x1, y1;
	if (!Xy2Xy(x, y, x1, y1))
		return 1;
	for(int i=0;i<m_nChess;i++)
		if(x1 == m_aChess[i].GetX() && y1 == m_aChess[i].GetY())
			return 2;
	m_aChess[m_nChess].Set(m_nChess, x1, y1, m_Color);
	Map[x1][y1] = (m_Color == WHITE) ? -1 : 1;
	m_nChess++;
	m_Color = (m_Color == WHITE ? BLACK:WHITE);
	return 0;
}

void CChessManager::Direct_Add(int x, int y)
{
	m_aChess[m_nChess].Set(m_nChess, x, y, m_Color);
	Map[x][y] = (m_Color == WHITE) ? -1 : 1;
	m_nChess++;
	m_Color = (m_Color == WHITE ? BLACK : WHITE);
}

//米字型搜索
//[—, | , / , \]四个移动方向
static const int inX[] = { 1,0,1,1 };
static const int inY[] = { 0,1,1,-1 };

//评分表
//Defence
static const int Score[3][6] = {
	{ 0, 0,  0,  0,   0,10000 },//防守2子
	{ 0, 0, 20,100, 500,10000 },//防守1子
	{ 0,20,100,500,2500,10000 } //防守0子
};

//x,y:棋盘位置
//color:落子颜色(黑色：1；白色：-1)
int CChessManager::getScore(const int x, const int y,const int color)
{
	//返回评分值
	int re = 0;

	//向 [—,|, /, \]四个方向搜索，对应inX，inY
	for (int i = 0; i < 4; ++i) {
		//k记录连子两侧空位的数目（非墙，非敌方落子）
		int k = 0;
		//记录连子的数目，初始值为1因为假设在当前位置落子
		int count = 1;

		//[—,|, /, \]四个方向的正负方向
		for (int j = -1; j < 2; j += 2) {
			int dx = x + j * inX[i];
			int dy = y + j * inY[i];

			//判断是否超出棋盘边界
			while (dx >= 0 && dx < MAX_COLS &&dy >= 0 && dy < MAX_ROWS)
			{
				//假如遇到颜色相同的子，count+1，反之则退出循环，并判断此时有无空位
				if (color * Map[dx][dy] > 0) {
					++count;
				}
				else {
					if (color * Map[dx][dy] == 0) ++k;
					break;
				}

				dx += j * inX[i];
				dy += j * inY[i];
			}
		}

		//假如连子大于5，使之等于5
		if (count > 5) count = 5;

		//加上该方向所得评分
		if (color == AI_Color)
			re += Score[k][count]*2;
		else
			re += Score[k][count];
	}
	return re;
}


void CChessManager::AI_Decision()
{
	int x=0, y=0,max = -1;
	//int X[100],Y[100],p=0;
	vector<int> X;
	vector<int> Y;
	if (m_nChess == 0)
	{
		x = 7;
		y = 7;
	}
	else if (m_nChess == 1)
	{
		if (m_aChess[0].GetX() == 7 && m_aChess[0].GetY() == 7)
		{
			x = 8;
			y = 6;
		}
		else
		{
			x = 7;
			y = 7;
		}
	}
	else
	{
		for (int i = 0; i < MAX_COLS; ++i) {
			for (int j = 0; j < MAX_ROWS; ++j) {
				//判断是否为空位
				if (Map[i][j] == 0) {
					//进攻还是防守，寻找利益最大值
					int score = (getScore(i, j, -1)>getScore(i, j, 1))? getScore(i, j, -1) : getScore(i, j, 1);

					//以防有多个相同的最大值
					if (score >= max) 
					{
						if (score > max) 
						{
							X.clear();
							Y.clear();
							max = score;
						}
						X.push_back(i);
						Y.push_back(j);
					}
				}
			}
		}

		//在最大值中任取一个返回,大部分情况只有一个
		int r = rand() % X.size();
		x = X[r];
		y = Y[r];
	}
	m_aChess[m_nChess].Set(m_nChess, x, y, m_Color);
	Map[x][y] = (m_Color == WHITE) ? -1 : 1;
	m_nChess++;
	m_Color = (m_Color == WHITE ? BLACK : WHITE);
	return;
}

void CChessManager::Regret()
{
	if (m_nChess == 0)
		return;
	Map[m_aChess[m_nChess].GetX()][m_aChess[m_nChess].GetY()] = 0;
	m_nChess--;
	m_Color = (m_Color == WHITE ? BLACK : WHITE);
	return;
}

void CChessManager::Load(int x, int y,COLOR color)
{
	m_aChess[m_nChess].Set(m_nChess, x, y, color);
	m_nChess++;
	return;
}

void CChessManager::Show(CDC* pDC)
{
	for (int i = 0; i < m_nChess-1; i++)
		m_aChess[i].Show(pDC);
	if(m_nChess)
		m_aChess[m_nChess - 1].Show_dot(pDC);
}

bool CChessManager::GameOver()
{
	if(CheckRows())
		return true;
	if (CheckCols())
		return true;
	if (CheckLSlash())
		return true;
	if (CheckRSlash())
		return true;
	return false;
}

CChess* CChessManager::GetQz(int x, int y)
{
	for(int i=0;i<m_nChess;i++)
		if(m_aChess[i].GetX()==x&& m_aChess[i].GetY() == y)
			return &m_aChess[i];
	return nullptr;
}

int CChessManager::GetMap(int x, int y)
{
	return Map[x][y];
}

bool CChessManager::CheckRows()
{
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = 0; i < MAX_ROWS; i++)
	{
		iCount = 0;
		for (int j = 0; j < MAX_COLS; j++)
			if (pQz = GetQz(j, i))
			{
				if (iCount == 0)
				{
					color = pQz->GetColor();
					iCount++;
				}
				else if (pQz->GetColor() == color)
				{
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else
				{
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}

bool CChessManager::CheckCols()
{
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = 0; i < MAX_COLS; i++)
	{
		iCount = 0;
		for (int j = 0; j < MAX_ROWS; j++)
			if (pQz = GetQz(i,j))
			{
				if (iCount == 0)
				{
					color = pQz->GetColor();
					iCount++;
				}
				else if (pQz->GetColor() == color)
				{
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else
				{
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}

bool CChessManager::CheckLSlash()
{
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = -14; i < MAX_COLS; i++)
	{
		iCount = 0;
		for (int j = 0; j < MAX_ROWS; j++)
			if (pQz = GetQz(i+j, j))
			{
				if (iCount == 0)
				{
					color = pQz->GetColor();
					iCount++;
				}
				else if (pQz->GetColor() == color)
				{
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else
				{
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}

void CChessManager::Color_Reverse()
{
	m_Color = (m_Color == WHITE ? BLACK : WHITE);
	return;
}

bool CChessManager::CheckRSlash()
{
	CChess* pQz;
	COLOR color;
	int iCount;
	for (int i = 0; i < MAX_COLS+14; i++)
	{
		iCount = 0;
		for (int j = 0; j < MAX_ROWS; j++)
			if (pQz = GetQz(i - j, j))
			{
				if (iCount == 0)
				{
					color = pQz->GetColor();
					iCount++;
				}
				else if (pQz->GetColor() == color)
				{
					iCount++;
					if (iCount == WIN_NUM)
						return true;
				}
				else
				{
					color = pQz->GetColor();
					iCount = 1;
				}
			}
			else
				iCount = 0;
	}
	return false;
}