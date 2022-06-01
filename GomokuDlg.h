
// GomokuDlg.h : header file
//

#pragma once
#include "CChessManager.h"
#include <windows.h>


// CGomokuDlg dialog
class CGomokuDlg : public CDialogEx
{
	
	CChessManager m_Manager;
	CFont m_FontTimer;
	CFont m_FontOver;
	int m_iTime1,m_iTime2;
	int music;
	bool m_bState;
	bool AI_State;
	bool Online_State;
	bool IS_USER;
	int WHO_FIRST=0;
	int IS_CREATER;
	int Trans_Data(int Data_1);
	bool Text_Display=false;
	bool mute;
	bool Online_Enabled = false;
// Construction
public:
	CGomokuDlg(CWnd* pParent = nullptr);	// standard constructor
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	bool NewGame(int x, int y);
	bool About(int x, int y);
	void AI_First();
	void Human_First();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GOMOKU_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnGameNewgame();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonRead();
	afx_msg void OnBnClickedButtonRegret();
	afx_msg void OnBnClickedButtonExit();
	afx_msg void OnBnClickedButtonMusic();
	afx_msg void OnBnClickedButtonAi();
	afx_msg void OnBnClickedRadioHumanFirst();
	afx_msg void OnBnClickedRadioAiFirst();
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonJoin();
};
