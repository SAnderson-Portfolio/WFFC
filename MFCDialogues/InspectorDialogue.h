#pragma once
#include <afxdialogex.h>
#include "resource.h"
#include <afxwin.h>
#include <vector>
#include "SceneObject.h"
class InspectorDialogue :	public CDialogEx
{
	DECLARE_DYNAMIC(InspectorDialogue)

public:
	InspectorDialogue(CWnd* pParent, std::vector<SceneObject>* sceneGraph);
	InspectorDialogue(CWnd* pParent = NULL);
	virtual ~InspectorDialogue();
	void SetObjectData(std::vector<SceneObject>* sceneGraph, int* selection);
	void SetImage(HBITMAP image);
	int GetSelectedID() { return *m_currentSelection; }
	void ClearSelection();
	bool GetDisplayListUpdateRequired() { return m_displayListUpdateRequired; }
	void SetDisplayListUpdateRequired(bool newState) { m_displayListUpdateRequired = newState; }
	bool ChangesAdded() { return m_changesRequireAdded; }
	void ConfirmChanges(bool confirm) { m_changesRequireAdded = confirm; }
	virtual BOOL OnInitDialog();
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnLBnSelectChange();
	afx_msg void OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()
	afx_msg void OnPaint();

	//Dialog Data
#ifdef AFX_DESIGN_TIME
	enum {IDD = IDD_DIALOG2};
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void End(); //Kill the dialogue
	afx_msg void Select(); //Item has been selected

	std::vector<SceneObject>* m_sceneGraph;
	int* m_currentSelection;
	HICON m_hIcon;

private:
	CTreeCtrl m_treeCtrl;
	CStatic m_IDText;
	CStatic m_InspectorImage;
	CBitmap bmp1;
	HBITMAP m_inspectorBitmap;
	CButton m_wireframeControl;
	BOOL m_wireframeCheck;
	BOOL m_renderCheck;
	CButton m_renderControl;
	void SetSelectionTree();
	void UpdateSectionTree();
	bool treeSet;
	int previousSelectionID;
	afx_msg void OnStnClickedStaticText();
	afx_msg void OnStnClickedPictureStatic();
	afx_msg void OnBnClickedWireframe();
	afx_msg void OnBnClickedRendercheck();
	bool m_displayListUpdateRequired;
	bool m_changesRequireAdded;
public:
	CButton m_transformResetButton;
	afx_msg void OnBnClickedResetTransformbutton();
	afx_msg void OnIdclose();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();
};

