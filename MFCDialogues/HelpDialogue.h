#pragma once


// HelpDialogue dialog
#include "afxdialogex.h"
#include "resource.h"

class HelpDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(HelpDialogue)

public:
	HelpDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~HelpDialogue();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG6 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
};
