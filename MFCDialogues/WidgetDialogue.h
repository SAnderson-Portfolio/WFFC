#pragma once

#include "afxdialogex.h"
#include "resource.h"
#include "TransformWidgets.h"
#include "TransformModes.h"
// WidgetDialogue dialog

class WidgetDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(WidgetDialogue)

public:
	WidgetDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~WidgetDialogue();
	void SetObjectData(TransformWidgets* widgetObject);	//passing in pointers to the data the class will operate on.
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void End(); //Kill the dialogue
	afx_msg void OnClose();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG4 };
#endif
private:
	TransformWidgets* m_transformWidgets;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult);
	CSpinButtonCtrl m_rotateSpin;
	CEdit m_rotateEdit;
	CEdit m_toleranceEdit;
	CSpinButtonCtrl m_toleranceSpin;
	afx_msg void OnDeltaposTolerancespin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposTranslatespin(NMHDR *pNMHDR, LRESULT *pResult);
	CSpinButtonCtrl m_translateSpin;
	CEdit m_translateEdit;
	CEdit m_scaleEdit;
	CSpinButtonCtrl m_scaleSpin;
	afx_msg void OnDeltaposScalespin(NMHDR *pNMHDR, LRESULT *pResult);
};
