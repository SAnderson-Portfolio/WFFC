#pragma once


// TerrainDialogue dialog

#include "afxdialogex.h"
#include "resource.h"
#include "DisplayChunk.h"
class TerrainDialogue : public CDialogEx
{
	DECLARE_DYNAMIC(TerrainDialogue)

public:
	TerrainDialogue(CWnd* pParent = nullptr);   // standard constructor
	virtual ~TerrainDialogue();
	void SetObjectData(DisplayChunk* chunkObject);
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG5 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

private:
	DisplayChunk*	m_displayChunk;
public:
	CEdit m_sculptEdit;
	CEdit m_flattenEdit;
	CEdit m_scaleEdit;
	CSpinButtonCtrl m_sculptSpin;
	CSpinButtonCtrl m_flattenSpin;
	CSpinButtonCtrl m_scaleSpin;
	CButton m_wireframeButton;
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSculptSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposFlattenSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposScaleSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedWireframeCheck();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	CEdit m_currentHeight;
	CEdit m_currentScale;
	afx_msg void OnPaint();
};
