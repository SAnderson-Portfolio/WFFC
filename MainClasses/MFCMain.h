#pragma once

#include <afxwin.h> 
#include <afxext.h>
#include <afx.h>
#include "pch.h"
#include "Game.h"
#include "ToolMain.h"
#include "resource.h"
#include "MFCFrame.h"
#include "SelectDialogue.h"
#include "InspectorDialogue.h"
#include "ObjectDialogue.h"
#include "WidgetDialogue.h"
#include "TerrainDialogue.h"
#include "HelpDialogue.h"
class MFCMain : public CWinApp 
{
public:
	MFCMain();
	~MFCMain();
	BOOL InitInstance();
	int  Run();
	void AddChanges() { m_ToolSystem.AddChanges(); }
private:

	CMyFrame * m_frame;	//handle to the frame where all our UI is
	HWND m_toolHandle;	//Handle to the MFC window
	ToolMain m_ToolSystem;	//Instance of Tool System that we interface to. 
	CRect WindowRECT;	//Window area rectangle. 
	SelectDialogue m_ToolSelectDialogue;			//for modeless dialogue, declare it here
	InspectorDialogue	m_ToolInspectorDialogue;
	ObjectDialogue	m_ToolObjectDialogue;
	WidgetDialogue m_ToolWidgetDialogue;
	TerrainDialogue m_ToolTerrainDialogue;
	HelpDialogue m_ToolHelpDialogue;
	int m_width;		
	int m_height;
	bool m_redoActive;
	bool m_undoActive;
	//Interface funtions for menu and toolbar etc requires
	afx_msg void MenuFileQuit();
	afx_msg void MenuFileSaveTerrain();
	afx_msg void MenuFileHelp();
	afx_msg	void ToolBarButton1();
	afx_msg void ToolBarFreeCamButton();
	//View
	afx_msg void MenuViewInspector();
	afx_msg void MenuViewObject();
	afx_msg void MenuViewWidget();
	afx_msg void MenuEditSelect();
	//Edit
	afx_msg void MenuEditDeselect();
	afx_msg void MenuEditDuplicate();
	//Terrain
	afx_msg void MenuTerrainGenerate();
	afx_msg void MenuTerrainUndo();
	afx_msg void MenuTerrainRedo();
	afx_msg void MenuTerrainPanel();
	//Picking messages
	afx_msg void SetTransformPicking() { m_ToolSystem.SetTransformPicking(); }
	afx_msg void SetFreeMovePicking() { m_ToolSystem.SetFreeMovePicking(); }
	//Terrain Picking Messages
	afx_msg void SetTerrainSculptingPicking() { m_ToolSystem.SetTerrainSculptingPicking(); }
	afx_msg void SetTerrainPaintPicking() { m_ToolSystem.SetTerrainPaintPicking(); }
	afx_msg void SetTerrainFlattenPicking() { m_ToolSystem.SetTerrainFlattenPicking(); }
	afx_msg void SetTerrainScalingPicking() { m_ToolSystem.SetTerrainScalingPicking(); }
	//Widget messages
	afx_msg void SetWidgetsToTranslate() { m_ToolSystem.SetWidgetsToTranslate(); }
	afx_msg void SetWidgetsToRotate() { m_ToolSystem.SetWidgetsToRotate(); }
	afx_msg void SetWidgetsToScale() { m_ToolSystem.SetWidgetsToScale(); }
	//State changing messages
	afx_msg void MenuEditUndo();
	afx_msg void MenuEditRedo();

	DECLARE_MESSAGE_MAP()	// required macro for message map functionality  One per class
public:
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
};
