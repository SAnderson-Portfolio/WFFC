#pragma once

#include <afxdialogex.h>
#include "resource.h"
#include <afxwin.h>
#include "DisplayObject.h"
#include "ObjectLoader.h"
#include "Game.h"
// ObjectDialogue dialog

class ObjectDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(ObjectDialogue)

public:
	ObjectDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~ObjectDialogue();
	void SetObjectData(ObjectLoader* objectLoader, Game* gameObject);	//passing in pointers to the data the class will operate on.
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void End(); //Kill the dialogue
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG3 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListBox m_ObjectList;

private:
	const int m_numberOfObjects = 5;
	const std::wstring m_objectListNames[5] = { L"Placeholder",  L"Stairs", L"Wall", L"Door",  L"Arch" };
	ObjectLoader* m_objectLoader;
	Game*		m_gameObject;
public:
	afx_msg void OnClose();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void ListDoubleClicked();
};
