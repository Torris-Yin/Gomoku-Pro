#pragma once
#include "afxdialogex.h"


// CONLINE dialog

class CONLINE : public CDialogEx
{
	DECLARE_DYNAMIC(CONLINE)

public:
	CONLINE(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CONLINE();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_ONLINE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCreate();
	afx_msg void OnBnClickedButtonJoin();
};
