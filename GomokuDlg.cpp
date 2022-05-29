
// GomokuDlg.cpp : implementation file
//
#include "afx.h"
#include "pch.h"
#include "windows.h"
#include "framework.h"
#include "Gomoku.h"
#include "GomokuDlg.h"
#include "afxdialogex.h"
#include <mmsystem.h>
#include <winsock2.h>
#pragma comment(lib, "WINMM.LIB")

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CGomokuDlg dialog


CGomokuDlg::CGomokuDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GOMOKU_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	mute = 1;
	music = 0;
	m_iTime1 = 0;
	m_iTime2 = 0;
	m_bState = false;
	Online_State = false;
	IS_USER = false;
	IS_CREATER = false;
	AI_State = false;
}

void CGomokuDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CGomokuDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_COMMAND(ID_GAME_NEWGAME, &CGomokuDlg::OnGameNewgame)
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CGomokuDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_READ, &CGomokuDlg::OnBnClickedButtonRead)
	ON_BN_CLICKED(IDC_BUTTON_REGRET, &CGomokuDlg::OnBnClickedButtonRegret)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CGomokuDlg::OnBnClickedButtonExit)
	ON_BN_CLICKED(IDC_BUTTON_MUSIC, &CGomokuDlg::OnBnClickedButtonMusic)
	ON_BN_CLICKED(IDC_BUTTON_AI, &CGomokuDlg::OnBnClickedButtonAi)
	ON_BN_CLICKED(IDC_RADIO_Human_First, &CGomokuDlg::OnBnClickedRadioHumanFirst)
	ON_BN_CLICKED(IDC_RADIO_AI_First, &CGomokuDlg::OnBnClickedRadioAiFirst)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CGomokuDlg::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_JOIN, &CGomokuDlg::OnBnClickedButtonJoin)
END_MESSAGE_MAP()


// CGomokuDlg message handlers

BOOL CGomokuDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	SetWindowPos(NULL, 0, 0, 1024, 768, SWP_NOZORDER | SWP_NOMOVE);
	m_FontTimer.CreatePointFont(250, _T("Segoe UI Semibold"), NULL);
	m_FontOver.CreatePointFont(1000, _T("微软雅黑"), NULL);
	m_bState = false;
	mute = 1;
	//PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT | SND_LOOP);
	if (__argc == 2)
	{
		m_Manager.NewGame();
		Invalidate();
		m_iTime1 = 0;
		m_iTime2 = 0;
		music = 0;
		SetTimer(1, 1000, NULL);
		SetTimer(2, 1000, NULL);
		m_bState = true;
		FILE* fp;
		fopen_s(&fp, __argv[1], "rt");
		COLOR color{};
		int x, y, num;
		m_Manager.SetColor(0);
		if (fp == NULL) return TRUE;
		fscanf_s(fp, "%d ", &num);
		for (int i = 0; i < num; i++)
		{
			fscanf_s(fp, "%d %d", &x, &y);
			m_Manager.Direct_Add(x, y);
		}
		if (fp == NULL) return TRUE;
		fclose(fp);
	}
	srand((unsigned int)(time(NULL)));
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CGomokuDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CGomokuDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC dc(this);
		CBitmap bmp;
		BITMAP bm{};
		if(mute == 1)
			bmp.LoadBitmap(IDB_BITMAP_BK_MUTE);
		else
			bmp.LoadBitmap(IDB_BITMAP_BK_UNMUTE);
		bmp.GetObject(sizeof(BITMAP), &bm);
		CDC MemDC;
		MemDC.CreateCompatibleDC(&dc);
		CBitmap* pOldBitmap = MemDC.SelectObject(&bmp);
		dc.BitBlt(0, 0, bm.bmWidth, bm.bmHeight, &MemDC, 0, 0, SRCCOPY);
		MemDC.SelectObject(pOldBitmap);
		m_Manager.Show(&dc);
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CGomokuDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CGomokuDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (Text_Display)
	{
		Text_Display = false;
		Invalidate();
		return;
	}
	if (NewGame(point.x, point.y))
	{
		KillTimer(4);
		Online_State = false;
		return;
	}
	if (About(point.x, point.y))
		return;
	if ((point.x >= 30 && point.y >= 220) && (point.x <= 205 && point.y <= 270))
	{
		if (!m_bState)
		{
			AfxMessageBox(_T("当前不处于游戏状态，无法保存"));
			return;
		}
		if (Online_State)
		{
			AfxMessageBox(_T("在线模式无法保存棋局"));
			return;
		}
		char szFilters[] =
			"Gomoku Record(*.gmk)\0*.gmk\0"\
			"\0";

		SYSTEMTIME st;
		CString strTime;
		GetLocalTime(&st);
		strTime.Format(_T("%d_%d_%d_%d_%d_%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		CFileDialog FileDlg(FALSE, _T("gmk"), strTime);
		FileDlg.m_ofn.lpstrTitle = _T("Save Current Game");
		FileDlg.m_ofn.lpstrFilter = szFilters;


		if (IDOK == FileDlg.DoModal())
		{
			FILE* fp;
			CChess* chess;
			fopen_s(&fp, FileDlg.GetPathName(), "wt");
			//for (int i = 0; i < MAX_ROWS; i++)
			//{
			//	for (int j = 0; j < MAX_COLS; j++)
			//	{
			//			fprintf(fp, "%d ",m_Manager.GetMap(i,j));
			//	}
			//	//fprintf(fp, "\n");
			//}
			//fprintf(fp, "%d ", m_Manager.GetColor());
			fprintf(fp, "%d ", m_Manager.GetNum());
			for (int i = 0; i < m_Manager.GetNum(); i++)
			{
				chess = m_Manager.GetChess(i);
				fprintf(fp, "%d %d ",chess->GetX(), chess->GetY());
			}
			fclose(fp);
		}
		return;
	}
	if ((point.x >= 30 && point.y >= 290) && (point.x <= 205 && point.y <= 340))
	{
		char szFilters[] =
			"Gomoku Record(*.gmk)\0*.gmk\0"\
			"\0";

		CFileDialog OpenDlg(TRUE);
		OpenDlg.m_ofn.lpstrTitle = _T("Open Gomoku File");
		OpenDlg.m_ofn.lpstrFilter = szFilters;

		if (IDOK == OpenDlg.DoModal())
		{
			m_Manager.NewGame();
			Invalidate();
			m_iTime1 = 0;
			m_iTime2 = 0;
			music = 0;
			SetTimer(1, 1000, NULL);
			SetTimer(2, 1000, NULL);
			m_bState = true;
			FILE* fp;
			fopen_s(&fp, OpenDlg.GetPathName(), "rt");
			COLOR color{};
			int x,y,num;
			m_Manager.SetColor(0);
			fscanf_s(fp, "%d ", &num);
			for (int i = 0; i < num; i++)
			{
				fscanf_s(fp, "%d %d", &x,&y);
				m_Manager.Direct_Add(x, y);
			}
			fclose(fp);
		}
		return;
	}
	if ((point.x >= 30 && point.y >= 360) && (point.x <= 205 && point.y <= 410))
	{
		if (!m_bState)
		{
			AfxMessageBox(_T("游戏已结束，无法悔棋"));
			return;
		}
		else if (AI_State)
		{
			m_Manager.Regret();
			m_Manager.Regret();
		}
		else if (Online_State)
		{
			AfxMessageBox(_T("在线模式不能悔棋哦"));
			return;
		}
		else
			m_Manager.Regret();
		Invalidate();
		return;
	}
	if ((point.x >= 30 && point.y >= 430) && (point.x <= 205 && point.y <= 480))
	{
		mute ^= 1;
		if (mute)
			PlaySound(NULL, NULL, SND_PURGE);
		else
			PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT | SND_LOOP);
		Invalidate();
		return;
	}
	if ((point.x >= 30 && point.y >= 500) && (point.x <= 205 && point.y <= 550))
	{
		int ans = Trans_Data(2);
		if (ans == 1)
		{
			IS_CREATER = 0;
			AfxMessageBox(_T("加入成功！对局已开始。"));
			SetTimer(4, 1000, NULL);
			//AfxMessageBox("对局开始！");
			m_Manager.NewGame();
			Invalidate();
			m_iTime1 = 0;
			m_iTime2 = 0;
			music = 0;
			SetTimer(1, 1000, NULL);
			SetTimer(2, 1000, NULL);
			m_bState = true;
			AI_State = false;
			Online_State = true;
			IS_USER = false;
			m_Manager.Map_Reset();
			//m_Manager.Color_Reverse();
		}
		else
		{
			Trans_Data(1);
			IS_CREATER = 1;
			AfxMessageBox(_T("房间创建成功！请等待对手加入房间。"));
			KillTimer(1);
			KillTimer(2);
			m_bState = false;
			m_iTime1 = 0;
			m_iTime2 = 0;
			m_Manager.NewGame();
			Invalidate();
			CClientDC dc(this);
			m_Manager.Show(&dc);
			SetTimer(3, 1000, NULL);
		}
		return;
	}
	if ((point.x >= 30 && point.y >= 570) && (point.x <= 205 && point.y <= 620))
	{
		DestroyWindow();
		return;
	}
	if ((point.x >= 844 && point.y >= 241) && (point.x <= 958 && point.y <= 291))
	{
		m_Manager.Set_AI_Color(-1);
		Human_First();
		return;
	}
	if ((point.x >= 844 && point.y >= 314) && (point.x <= 958 && point.y <= 364))
	{
		m_Manager.Set_AI_Color(1);
		AI_First();
		return;
	}
	if (!m_bState)
	{
		AfxMessageBox(_T("请选择“开始”按钮开始新的游戏，按Esc键退出游戏！"));
		return;
	}
	if (AI_State && !IS_USER)
	{
		AfxMessageBox(_T("请等待AI棋手落子！"));
		return;
	}
	if (Online_State && !IS_USER)
	{
		AfxMessageBox(_T("请等待对手落子！"));
		return;
	}
	int r = m_Manager.Add(point.x, point.y);
	if (r == 0)
	{
		CClientDC dc(this);
		m_Manager.Show(&dc);
		if (AI_State)
		{
			IS_USER = false;
			CClientDC dc(this);
			m_Manager.Show(&dc);
			if (m_Manager.GameOver())
			{
				KillTimer(1);
				KillTimer(2);
				CString csTemp;
				if (m_Manager.GetWinner() == WHITE)
					/*if (AI_State)
						if (WHO_FIRST == 1)
						{
							csTemp.Format(_T("玩家胜！"));
							Text_Display = true;
						}
						else
						{
							csTemp.Format(_T("电脑胜！"));
							Text_Display = true;
						}
					else*/
					{
						csTemp.Format(_T("白方胜！"));
						Text_Display = true;
					}
				else
					/*if (AI_State)
						if (WHO_FIRST == 1)
						{
							csTemp.Format(_T("电脑胜！"));
							Text_Display = true;
						}
						else
						{
							csTemp.Format(_T("玩家胜！"));
							Text_Display = true;
						}
					else*/
					{
						csTemp.Format(_T("黑方胜！"));
						Text_Display = true;
					}
				m_bState = false;
				CClientDC dc(this);
				CFont* pOldFont = dc.SelectObject(&m_FontOver);
				int OldBkMode = dc.GetBkMode();
				COLORREF OldColor, NewColor1 = RGB(60, 60, 60), NewColor2 = RGB(250, 50, 50);
				dc.SetBkMode(TRANSPARENT);
				OldColor = dc.SetTextColor(NewColor1);
				dc.TextOut(158, 208, csTemp);
				dc.SetTextColor(NewColor2);
				dc.TextOut(150, 200, csTemp);
				dc.SetTextColor(OldColor);
				dc.SetBkMode(OldBkMode);
				dc.SelectObject(pOldFont);
				return;
			}
			m_Manager.AI_Decision();
			m_Manager.Show(&dc);
			if (m_Manager.GameOver())
			{
				KillTimer(1);
				KillTimer(2);
				CString csTemp;
				if (m_Manager.GetWinner() == WHITE)
				{
					csTemp.Format(_T("白方胜！"));
					Text_Display = true;
				}
				else
				{
					csTemp.Format(_T("黑方胜！"));
					Text_Display = true;
				}
				m_bState = false;
				CClientDC dc(this);
				CFont* pOldFont = dc.SelectObject(&m_FontOver);
				int OldBkMode = dc.GetBkMode();
				COLORREF OldColor, NewColor1 = RGB(60, 60, 60), NewColor2 = RGB(250, 50, 50);
				dc.SetBkMode(TRANSPARENT);
				OldColor = dc.SetTextColor(NewColor1);
				dc.TextOut(158, 208, csTemp);
				dc.SetTextColor(NewColor2);
				dc.TextOut(150, 200, csTemp);
				dc.SetTextColor(OldColor);
				dc.SetBkMode(OldBkMode);
				dc.SelectObject(pOldFont);
			}
			IS_USER = true;
			return;
		}
		
		if (Online_State)
		{
			int x1, y1;
			m_Manager.Xy2Xy(point.x, point.y,x1,y1);
			if(IS_CREATER)
				Trans_Data(x1*100+y1+40000);
			else
				Trans_Data(x1 * 100 + y1 + 60000);
			IS_USER = false;
			SetTimer(4, 1000, NULL);
		}

		if (m_Manager.GameOver())
		{
			KillTimer(1);
			KillTimer(2);
			if(Online_State)
				KillTimer(4);
			CString csTemp;
			if (m_Manager.GetWinner() == WHITE)
			{
				csTemp.Format(_T("白方胜！"));
				Text_Display = true;
			}
			else
			{
				csTemp.Format(_T("黑方胜！"));
				Text_Display = true;
			}
			m_bState = false;
			CClientDC dc(this);
			CFont* pOldFont = dc.SelectObject(&m_FontOver);
			int OldBkMode = dc.GetBkMode();
			COLORREF OldColor, NewColor1 = RGB(60, 60, 60), NewColor2 = RGB(250, 50, 50);
			dc.SetBkMode(TRANSPARENT);
			OldColor = dc.SetTextColor(NewColor1);
			dc.TextOut(158, 208, csTemp);
			dc.SetTextColor(NewColor2);
			dc.TextOut(150,200, csTemp);
			dc.SetTextColor(OldColor);
			dc.SetBkMode(OldBkMode);
			dc.SelectObject(pOldFont);
		}
	}
	if (r == 1)
		AfxMessageBox(_T("请在棋盘交叉点落子！"));
	else if(r==2)
		AfxMessageBox(_T("不可以重复落子！"));
	CDialogEx::OnLButtonUp(nFlags, point);
}

bool CGomokuDlg::NewGame(int x, int y)
{
	int x0 = 35, y0 = 150, x1 = 200, y1 = 185;
	if ((x >= x0 && x <= x1) && (y >= y0 && y <= y1))
	{
		m_Manager.NewGame();
		Invalidate();
		m_iTime1 = 0;
		m_iTime2 = 0;
		music = 0;
		SetTimer(1, 1000, NULL);
		SetTimer(2, 1000, NULL);
		m_bState = true;
		AI_State = false;
		m_Manager.Map_Reset();
		return true;
	}
	return false;
}

bool CGomokuDlg::About(int x, int y)
{
	int x0 = 35, y0 = 70, x1 = 200, y1 = 95;
	if ((x >= x0 && x <= x1) && (y >= y0 && y <= y1))
	{
		CAboutDlg dlg;
		dlg.DoModal();
		return true;
	}
	return false;
}



void CGomokuDlg::OnTimer(UINT_PTR nIDEvent)
{
	switch (nIDEvent)
	{
		case 1:
		{
			CClientDC dc(this);
			CFont* pOldFont;
			pOldFont = dc.SelectObject(&m_FontTimer);
			if(m_Manager.GetColor() ==0 )
				m_iTime1++;
			CString csTemp;
			csTemp.Format(_T("黑：%04d "), m_iTime1);
			COLORREF OldColor, NewColor = RGB(150, 50, 50);
			OldColor = dc.SetTextColor(NewColor);
			//dc.SetBkMode(TRANSPARENT);
			dc.TextOut(400, 7, csTemp);
			dc.SetTextColor(OldColor);
			dc.SelectObject(pOldFont);
			break;
		}
		case 2:
		{
			CClientDC dc(this);
			CFont* pOldFont;
			pOldFont = dc.SelectObject(&m_FontTimer);
			if (m_Manager.GetColor() == 1)
				m_iTime2++;
			CString csTemp;
			csTemp.Format(_T("白：%04d "), m_iTime2);
			COLORREF OldColor, NewColor = RGB(150, 50, 50);
			OldColor = dc.SetTextColor(NewColor);
			//dc.SetBkMode(TRANSPARENT);
			dc.TextOut(725, 7, csTemp);
			dc.SetTextColor(OldColor);
			dc.SelectObject(pOldFont);
			break;
		}
		case 3:
		{
			int ans=Trans_Data(3);
			if (ans == 1)
			{
				KillTimer(3);
				AfxMessageBox(_T("对局开始！"));
				m_Manager.NewGame();
				Invalidate();
				m_iTime1 = 0;
				m_iTime2 = 0;
				music = 0;
				SetTimer(1, 1000, NULL);
				SetTimer(2, 1000, NULL);
				m_bState = true;
				AI_State = false;
				Online_State = true;
				IS_USER = true;
				m_Manager.Map_Reset();
			}
			break;
		}
		case 4:
		{
			int ans;
			if(IS_CREATER)
				ans = Trans_Data(51);
			else
				ans = Trans_Data(50);
			if (ans)
			{
				m_Manager.Direct_Add((ans-10000) / 100, (ans - 10000) % 100);
				IS_USER = true;
				CClientDC dc(this);
				m_Manager.Show(&dc);
				KillTimer(4);
				if (m_Manager.GameOver())
				{
					KillTimer(1);
					KillTimer(2);
					if (Online_State)
						KillTimer(4);
					CString csTemp;
					if (m_Manager.GetWinner() == WHITE)
					{
						csTemp.Format(_T("白方胜！"));
						Text_Display = true;
					}
					else
					{
						csTemp.Format(_T("黑方胜！"));
						Text_Display = true;
					}
					m_bState = false;
					CClientDC dc(this);
					CFont* pOldFont = dc.SelectObject(&m_FontOver);
					int OldBkMode = dc.GetBkMode();
					COLORREF OldColor, NewColor1 = RGB(60, 60, 60), NewColor2 = RGB(250, 50, 50);
					dc.SetBkMode(TRANSPARENT);
					OldColor = dc.SetTextColor(NewColor1);
					dc.TextOut(158, 208, csTemp);
					dc.SetTextColor(NewColor2);
					dc.TextOut(150, 200, csTemp);
					dc.SetTextColor(OldColor);
					dc.SetBkMode(OldBkMode);
					dc.SelectObject(pOldFont);
				}
			}
			break;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}

void CGomokuDlg::OnGameNewgame()
{
	// TODO: Add your command handler code here
	NewGame(50, 150);
}

void CGomokuDlg::OnBnClickedButtonSave()
{
	char szFilters[] =
		"Gomoku Record(*.gmk)\0*.gmk\0"\
		"\0";

	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st);
	strTime.Format(_T("%d_%d_%d_%d_%d_%d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
	CFileDialog FileDlg(FALSE, "gmk", strTime);
	FileDlg.m_ofn.lpstrTitle = _T("Save Current Game");
	FileDlg.m_ofn.lpstrFilter = szFilters;


	if (IDOK == FileDlg.DoModal())
	{
		FILE* fp;
		fopen_s(&fp, FileDlg.GetPathName(), "wt");
		CChess* color;
		for (int i = 0; i < MAX_ROWS; i++)
		{
			for (int j = 0; j < MAX_COLS; j++)
			{
				color = m_Manager.GetQz(j, i);
				if (color == NULL)
				{
					if (fp == NULL) return;
					fprintf(fp, "-1 ");
				}
				else
				{
					if (fp == NULL) return;
					fprintf(fp, "%d ", color->GetColor());
				}
			}
			//fprintf(fp, "\n");
		}
		fprintf(fp, "%d ", m_Manager.GetColor());
		if (fp == NULL) return;
		fclose(fp);
	}
}


void CGomokuDlg::OnBnClickedButtonRead()
{
	// TODO: Add your control notification handler code here
	
	char szFilters[] =
		"Gomoku Record(*.gmk)\0*.gmk\0"\
		"\0";

	CFileDialog OpenDlg(TRUE);
	OpenDlg.m_ofn.lpstrTitle = _T("Open Gomoku File");
	OpenDlg.m_ofn.lpstrFilter = szFilters;

	if (IDOK == OpenDlg.DoModal())
	{
		m_Manager.NewGame();
		Invalidate();
		m_iTime1 = 0;
		m_iTime2 = 0;
		music = 0;
		SetTimer(1, 1000, NULL);
		SetTimer(2, 1000, NULL);
		m_bState = true;
		FILE* fp;
		fopen_s(&fp, OpenDlg.GetPathName(), "rt");
		COLOR color;
		int colordata;
		for (int i = 0; i < MAX_ROWS; i++)
		{
			for (int j = 0; j < MAX_COLS; j++)
			{
				if (fp == NULL) return;
				fscanf_s(fp,"%d ", &colordata);
				if (colordata == -1)
					continue;
				color = (COLOR)colordata;
				m_Manager.Load(j, i, color);
			}
		}
		fscanf_s(fp, "%d ", &colordata);
		m_Manager.SetColor(colordata);
		if (fp == NULL) return;
		fclose(fp);
	}
}

void CGomokuDlg::OnBnClickedButtonRegret()
{
	// TODO: Add your control notification handler code here
	if (!m_bState)
	{
		AfxMessageBox(_T("游戏已结束，无法悔棋"));
		return;
	}
	else if (AI_State)
	{
		m_Manager.Regret();
		m_Manager.Regret();
	}
	else if (Online_State)
	{
		AfxMessageBox(_T("在线模式不能悔棋哦"));
		return;
	}
	else
		m_Manager.Regret();
	Invalidate();
}


void CGomokuDlg::OnBnClickedButtonExit()
{
	// TODO: Add your control notification handler code here
	DestroyWindow();
}


void CGomokuDlg::OnBnClickedButtonMusic()
{
	// TODO: Add your control notification handler code here
	if (music == 0)
	{
		music = 1;
		GetDlgItem(IDC_BUTTON_MUSIC)->SetWindowText(_T("关闭音乐"));
		PlaySound(MAKEINTRESOURCE(IDR_WAVE1), AfxGetResourceHandle(), SND_ASYNC | SND_RESOURCE | SND_NODEFAULT | SND_LOOP);
	}
	else
	{
		music = 0;
		GetDlgItem(IDC_BUTTON_MUSIC)->SetWindowText(_T("播放音乐"));
		PlaySound(NULL, NULL, SND_PURGE);
	}
	return;
}

void CGomokuDlg::AI_First()
{
	m_Manager.NewGame();
	Invalidate();
	m_iTime1 = 0;
	m_iTime2 = 0;
	music = 0;
	SetTimer(1, 1000, NULL);
	SetTimer(2, 1000, NULL);
	m_bState = true;
	AI_State = true;
	Online_State = false;
	IS_USER = false;
	m_Manager.Map_Reset();
	m_Manager.AI_Decision();
	CClientDC dc(this);
	m_Manager.Show(&dc);
	IS_USER = true;
}

void CGomokuDlg::Human_First()
{
	m_Manager.NewGame();
	Invalidate();
	m_iTime1 = 0;
	m_iTime2 = 0;
	music = 0;
	SetTimer(1, 1000, NULL);
	SetTimer(2, 1000, NULL);
	m_bState = true;
	AI_State = true;
	Online_State = false;
	IS_USER = true;
	m_Manager.Map_Reset();
}

void CGomokuDlg::OnBnClickedButtonAi()
{
	//TODO: Add your control notification handler code here
	if (WHO_FIRST == -1)
		Human_First();
	else if (WHO_FIRST == 1)
		AI_First();
	else
		AfxMessageBox(_T("请选择先手模式"));
}


void CGomokuDlg::OnBnClickedRadioHumanFirst()
{
	// TODO: Add your control notification handler code here
	WHO_FIRST = -1;
}


void CGomokuDlg::OnBnClickedRadioAiFirst()
{
	// TODO: Add your control notification handler code here
	WHO_FIRST = 1;
}

int CGomokuDlg::Trans_Data(int Data_1)
{

	int ans = 0, p = 0;
	WORD sockVersion = MAKEWORD(2, 2);
	WSADATA data;
	if (WSAStartup(sockVersion, &data) != 0)
	{
		return -1;
	}
	while (true)
	{
		SOCKET sclient = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if (sclient == INVALID_SOCKET)
		{
			return -1;
		}

		sockaddr_in serAddr{};
		serAddr.sin_family = AF_INET;
		serAddr.sin_port = htons(8888);
		inet_pton(AF_INET, "152.136.97.214", &serAddr.sin_addr.S_un.S_addr);
		if (connect(sclient, (sockaddr*)&serAddr, sizeof(serAddr)) == SOCKET_ERROR)
		{
			//连接失败
			closesocket(sclient);
			return -1;
		}

		char sendData[255];
		//_itoa_s(Data_1, sendData, 10);
		sprintf_s(sendData, "%d", Data_1);
		//char * sendData = "你好，TCP服务端，我是客户端\n";
		send(sclient, sendData, (int)strlen(sendData), (0));
		//send()用来将数据由指定的socket传给对方主机
		//int send(int s, const void * msg, int len, unsigned int flags)
		//s为已建立好连接的socket，msg指向数据内容，len则为数据长度，参数flags一般设0
		//成功则返回实际传送出去的字符数，失败返回-1，错误原因存于error

		char recData[4096];
		int ret = recv(sclient, recData, 255, 0);
		if (ret > 0)
		{
			recData[ret] = 0x00;
			while (recData[p] != 0)
				ans = ans * 10 + (recData[p++] - '0');
		}
		closesocket(sclient);
		WSACleanup();
		return ans;
	}
}

void CGomokuDlg::OnBnClickedButtonCreate()
{
	// TODO: Add your control notification handler code here
	int ans = Trans_Data(1);
	if (ans == 1)
	{
		IS_CREATER = 1;
		AfxMessageBox(_T("创建成功！"));
		KillTimer(1);
		KillTimer(2);
		m_bState = false;
		m_iTime1 = 0;
		m_iTime2 = 0;
		m_Manager.NewGame();
		Invalidate();
		CClientDC dc(this);
		m_Manager.Show(&dc);
		SetTimer(3, 1000, NULL);
	}
	else
		AfxMessageBox(_T("创建失败！"));

}


void CGomokuDlg::OnBnClickedButtonJoin()
{
	// TODO: Add your control notification handler code here
	int ans = Trans_Data(2);
	if (ans == 1)
	{
		IS_CREATER = 0;
		AfxMessageBox(_T("加入成功！"));
		SetTimer(4, 1000, NULL);
		//AfxMessageBox("对局开始！");
		m_Manager.NewGame();
		Invalidate();
		m_iTime1 = 0;
		m_iTime2 = 0;
		music = 0;
		SetTimer(1, 1000, NULL);
		SetTimer(2, 1000, NULL);
		m_bState = true;
		AI_State = false;
		Online_State = true;
		IS_USER = false;
		m_Manager.Map_Reset();
		//m_Manager.Color_Reverse();
	}
	else
		AfxMessageBox(_T("加入失败！"));

}