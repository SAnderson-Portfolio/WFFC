// HelpDialogue.cpp : implementation file
//



// HelpDialogue dialog
#include "HelpDialogue.h"

IMPLEMENT_DYNAMIC(HelpDialogue, CDialogEx)

HelpDialogue::HelpDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG6, pParent)
{

}

HelpDialogue::~HelpDialogue()
{
}

void HelpDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(HelpDialogue, CDialogEx)
	ON_BN_CLICKED(IDOK, &HelpDialogue::OnBnClickedOk)
	ON_COMMAND(IDCLOSE, &HelpDialogue::OnClose)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// HelpDialogue message handlers


void HelpDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	DestroyWindow();
}


void HelpDialogue::OnClose()
{
	// TODO: Add your command handler code here
	CDialog::OnClose();
	DestroyWindow();
}
