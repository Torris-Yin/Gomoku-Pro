// CONLINE.cpp : implementation file
//

#include "pch.h"
#include "Gomoku.h"
#include "afxdialogex.h"
#include "CONLINE.h"


// CONLINE dialog

IMPLEMENT_DYNAMIC(CONLINE, CDialogEx)

CONLINE::CONLINE(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_ONLINE, pParent)
{

}

CONLINE::~CONLINE()
{
}

void CONLINE::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CONLINE, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CONLINE::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON_CREATE, &CONLINE::OnBnClickedButtonCreate)
	ON_BN_CLICKED(IDC_BUTTON_JOIN, &CONLINE::OnBnClickedButtonJoin)
END_MESSAGE_MAP()


// CONLINE message handlers


void CONLINE::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void CONLINE::OnBnClickedButtonCreate()
{
	// TODO: Add your control notification handler code here
	
}

void CONLINE::OnBnClickedButtonJoin()
{
	// TODO: Add your control notification handler code here
}
