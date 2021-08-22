#include "MFCMain.h"
#include "resource.h"


BEGIN_MESSAGE_MAP(MFCMain, CWinApp)
	//Menu File
	ON_COMMAND(ID_FILE_QUIT, &MFCMain::MenuFileQuit)
	ON_COMMAND(ID_FILE_SAVETERRAIN, &MFCMain::MenuFileSaveTerrain)
	ON_COMMAND(ID_FILE_HELP, &MFCMain::MenuFileHelp)
	//Menu Edit
	ON_COMMAND(ID_EDIT_SELECT, &MFCMain::MenuEditSelect)
	ON_COMMAND(ID_EDIT_UNDO, &MFCMain::MenuEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &MFCMain::MenuEditRedo)
	ON_COMMAND(ID_EDIT_DESELECT, &MFCMain::MenuEditDeselect)
	ON_COMMAND(ID_EDIT_DUPLICATE, &MFCMain::MenuEditDuplicate)
	//Menu View
	ON_COMMAND(ID_VIEW_INSPECTOR, &MFCMain::MenuViewInspector)
	ON_COMMAND(ID_VIEW_OBJECTLOADER, &MFCMain::MenuViewObject)
	ON_COMMAND(ID_VIEW_WIDGETVALUES, &MFCMain::MenuViewWidget)
	//Menu Terrain
	ON_COMMAND(ID_TERRAIN_GENERATE, &MFCMain::MenuTerrainGenerate)
	ON_COMMAND(ID_TERRAIN_UNDO, &MFCMain::MenuTerrainUndo)
	ON_COMMAND(ID_TERRAIN_REDO, &MFCMain::MenuTerrainRedo)
	ON_COMMAND(ID_TERRAIN_VALUES, &MFCMain::MenuTerrainPanel)
	//Toolbar
	ON_COMMAND(ID_BUTTON40001, &MFCMain::ToolBarButton1)
	ON_COMMAND(ID_TranslateButton, &MFCMain::SetWidgetsToTranslate)
	ON_COMMAND(ID_RotateButton, &MFCMain::SetWidgetsToRotate)
	ON_COMMAND(ID_ScaleButton, &MFCMain::SetWidgetsToScale)
	ON_COMMAND(ID_SculptTerrainButton, &MFCMain::SetTerrainSculptingPicking)
	ON_COMMAND(ID_FlattenTerrainButton, &MFCMain::SetTerrainFlattenPicking)
	ON_COMMAND(ID_ScaleTerrainButton, &MFCMain::SetTerrainScalingPicking)
	ON_COMMAND(ID_PaintTerrainButton, &MFCMain::SetTerrainPaintPicking)
	ON_COMMAND(ID_FREE_MOVE, &MFCMain::SetFreeMovePicking)
	//ON_COMMAND(ID_PaintTerrainButton, &MFCMain::SetTerrainPaintingPicking)
	ON_UPDATE_COMMAND_UI(ID_INDICATOR_TOOL, &CMyFrame::OnUpdatePage)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &MFCMain::OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &MFCMain::OnUpdateEditRedo)
END_MESSAGE_MAP()

BOOL MFCMain::InitInstance()
{
	//instanciate the mfc frame
	m_frame = new CMyFrame();
	m_pMainWnd = m_frame;

	m_frame->Create(	NULL,
					_T("World Of Flim-Flam Craft Editor"),
					WS_OVERLAPPEDWINDOW,
					CRect(100, 100, 1024, 768),
					NULL,
					NULL,
					0,
					NULL
				);
	//show and set the window to run and update. 
	HICON windowIcon = static_cast<HICON>(LoadImage(m_hInstance, LPWSTR("database/data/WFFCIcon.bmp"), IMAGE_ICON, GetSystemMetrics(SM_CXSMICON),	GetSystemMetrics(SM_CYSMICON),	LR_DEFAULTCOLOR));
	m_frame->SetIcon(windowIcon, false);
	m_frame->ShowWindow(SW_SHOW);
	m_frame->UpdateWindow();

	//get the rect from the MFC window so we can get its dimensions
	m_toolHandle = m_frame->m_DirXView.GetSafeHwnd();				//handle of directX child window
	m_frame->m_DirXView.GetClientRect(&WindowRECT);
	m_width		= WindowRECT.Width();
	m_height	= WindowRECT.Height();

	m_ToolSystem.onActionInitialise(m_toolHandle, m_pMainWnd->GetSafeHwnd(), m_width, m_height);

	m_redoActive = false;
	m_undoActive = false;
	return TRUE;
}

int MFCMain::Run()
{
	MSG msg;
	BOOL bGotMsg;

	PeekMessage(&msg, NULL, 0U, 0U, PM_NOREMOVE);

	while (WM_QUIT != msg.message)
	{
		if (true)
		{
			bGotMsg = (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE) != 0);
		}
		else
		{
			bGotMsg = (GetMessage(&msg, NULL, 0U, 0U) != 0);
		}

		if (bGotMsg)
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			m_ToolSystem.UpdateInput(&msg);
		}
		else
		{	
			int ID = m_ToolSystem.getCurrentSelectionID();
			std::wstring statusString;
			if (ID == -1)
			{
				statusString = L"Selected Object: None";
				if (m_ToolInspectorDialogue)
					m_ToolInspectorDialogue.ClearSelection();
			}
			else
				statusString = L"Selected Object: " + std::to_wstring(ID);

			m_ToolSystem.Tick(&msg);


			//send current object ID to status bar in The main frame
			m_frame->m_wndStatusBar.SetPaneText(1, statusString.c_str(), 1);
			if (m_ToolInspectorDialogue)
			{
				if(m_ToolInspectorDialogue.GetSelectedID() != ID || m_ToolSystem.InspectorRequiresUpdate())
					m_ToolInspectorDialogue.RedrawWindow();

				if (m_ToolInspectorDialogue.GetDisplayListUpdateRequired())
				{
					if (m_ToolInspectorDialogue.ChangesAdded())
					{
						m_ToolSystem.ResetObjectTransform();
						m_ToolInspectorDialogue.ConfirmChanges(false);
						m_ToolInspectorDialogue.RedrawWindow();
					}
					m_ToolSystem.RebuildDisplayList();
					m_ToolInspectorDialogue.SetDisplayListUpdateRequired(false);
				}
			}

			m_ToolSystem.StacksHaveElements(m_undoActive, m_redoActive);

			if (m_ToolTerrainDialogue)
			{
				if (m_ToolSystem.GetDisplayChunk()->IntersectedPointUpdated())
				{
					m_ToolTerrainDialogue.RedrawWindow();
					m_ToolSystem.GetDisplayChunk()->SetIntersectedPointUpdated(false);
				}
			}
		}
	}

	

	return (int)msg.wParam;
}

void MFCMain::MenuFileQuit()
{
	//will post message to the message thread that will exit the application normally
	PostQuitMessage(0);
}

void MFCMain::MenuFileSaveTerrain()
{
	m_ToolSystem.onActionSaveTerrain();
}

void MFCMain::MenuFileHelp()
{
	if (!m_ToolHelpDialogue)
	{
		m_ToolHelpDialogue.Create(IDD_DIALOG6);	//Start up modeless
		m_ToolHelpDialogue.ShowWindow(SW_SHOW);	//show modeless
	}
	else
	{
		m_ToolHelpDialogue.SetForegroundWindow();
	}
}

void MFCMain::MenuEditSelect()
{
	//SelectDialogue m_ToolSelectDialogue(NULL, &m_ToolSystem.m_sceneGraph);		//create our dialoguebox //modal constructor
	//m_ToolSelectDialogue.DoModal();	// start it up modal

	//modeless dialogue must be declared in the class.   If we do local it will go out of scope instantly and destroy itself
	if (!m_ToolSelectDialogue)
	{
		m_ToolSelectDialogue.Create(IDD_DIALOG1);	//Start up modeless
		m_ToolSelectDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_ToolSelectDialogue.SetObjectData(&m_ToolSystem.m_sceneGraph, &m_ToolSystem.getCurrentSelectionID());
	}
	else
	{
		m_ToolSelectDialogue.SetForegroundWindow();
	}
	
}

void MFCMain::ToolBarButton1()
{
	
	m_ToolSystem.onActionSave();
}

void MFCMain::ToolBarFreeCamButton()
{
	m_ToolSystem.onFreeCamToggle();
}

void MFCMain::MenuViewInspector()
{
	if (!m_ToolInspectorDialogue)
	{
		m_ToolInspectorDialogue.Create(IDD_DIALOG2);	//Start up modeless
		m_ToolInspectorDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_ToolInspectorDialogue.SetObjectData(&m_ToolSystem.GetSceneGraph(), &m_ToolSystem.getCurrentSelectionID());
	}
	else
	{
		m_ToolInspectorDialogue.SetForegroundWindow();
	}
	
}

void MFCMain::MenuViewObject()
{
	if (!m_ToolObjectDialogue)
	{
		m_ToolObjectDialogue.Create(IDD_DIALOG3);	//Start up modeless
		m_ToolObjectDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_ToolObjectDialogue.SetObjectData(m_ToolSystem.GetObjectLoader(), m_ToolSystem.GetGameObject());
	}
	else
	{
		m_ToolObjectDialogue.SetForegroundWindow();
	}
}

void MFCMain::MenuViewWidget()
{
	if (!m_ToolWidgetDialogue)
	{
		m_ToolWidgetDialogue.Create(IDD_DIALOG4);	//Start up modeless
		m_ToolWidgetDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_ToolWidgetDialogue.SetObjectData(m_ToolSystem.GetTransformWidgets());
	}
	else
	{
		m_ToolWidgetDialogue.SetForegroundWindow();
	}
}

void MFCMain::MenuEditDeselect()
{
	m_ToolSystem.setCurrentSelectionID(-1);
}

void MFCMain::MenuEditDuplicate()
{
	
	m_ToolSystem.DuplicateSelectedObject();
}

void MFCMain::MenuTerrainGenerate()
{
	m_ToolSystem.GetDisplayChunk()->GenerateHeightmap();
}

void MFCMain::MenuTerrainUndo()
{
	m_ToolSystem.GetDisplayChunk()->UndoChanges();
}

void MFCMain::MenuTerrainRedo()
{
	m_ToolSystem.GetDisplayChunk()->RedoChanges();
}

void MFCMain::MenuTerrainPanel()
{
	if (!m_ToolTerrainDialogue)
	{
		m_ToolTerrainDialogue.Create(IDD_DIALOG5);	//Start up modeless
		m_ToolTerrainDialogue.ShowWindow(SW_SHOW);	//show modeless
		m_ToolTerrainDialogue.SetObjectData(m_ToolSystem.GetDisplayChunk());
	}
	else
	{
		m_ToolTerrainDialogue.SetForegroundWindow();
	}
}


MFCMain::MFCMain()
{
}


MFCMain::~MFCMain()
{

}


void MFCMain::MenuEditUndo()
{
	m_ToolSystem.UndoChanges();
	if (m_ToolInspectorDialogue)
		m_ToolInspectorDialogue.RedrawWindow();
}

void MFCMain::MenuEditRedo()
{
	m_ToolSystem.RedoChanges();
	if (m_ToolInspectorDialogue)
		m_ToolInspectorDialogue.RedrawWindow();
}

void MFCMain::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_undoActive);
}


void MFCMain::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(m_redoActive);
}
