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

//����������
//[��, | , / , \]�ĸ��ƶ�����
static const int inX[] = { 1,0,1,1 };
static const int inY[] = { 0,1,1,-1 };

//���ֱ�
//Defence
static const int Score[3][6] = {
	{ 0, 0,  0,  0,   0,10000 },//����2��
	{ 0, 0, 20,100, 500,10000 },//����1��
	{ 0,20,100,500,2500,10000 } //����0��
};

//x,y:����λ��
//color:������ɫ(��ɫ��1����ɫ��-1)
int CChessManager::getScore(const int x, const int y,const int color)
{
	//��������ֵ
	int re = 0;

	//�� [��,|, /, \]�ĸ�������������ӦinX��inY
	for (int i = 0; i < 4; ++i) {
		//k��¼���������λ����Ŀ����ǽ���ǵз����ӣ�
		int k = 0;
		//��¼���ӵ���Ŀ����ʼֵΪ1��Ϊ�����ڵ�ǰλ������
		int count = 1;

		//[��,|, /, \]�ĸ��������������
		for (int j = -1; j < 2; j += 2) {
			int dx = x + j * inX[i];
			int dy = y + j * inY[i];

			//�ж��Ƿ񳬳����̱߽�
			while (dx >= 0 && dx < MAX_COLS &&dy >= 0 && dy < MAX_ROWS)
			{
				//����������ɫ��ͬ���ӣ�count+1����֮���˳�ѭ�������жϴ�ʱ���޿�λ
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

		//�������Ӵ���5��ʹ֮����5
		if (count > 5) count = 5;

		//���ϸ÷�����������
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
				//�ж��Ƿ�Ϊ��λ
				if (Map[i][j] == 0) {
					//�������Ƿ��أ�Ѱ���������ֵ
					int score = (getScore(i, j, -1)>getScore(i, j, 1))? getScore(i, j, -1) : getScore(i, j, 1);

					//�Է��ж����ͬ�����ֵ
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

		//�����ֵ����ȡһ������,�󲿷����ֻ��һ��
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