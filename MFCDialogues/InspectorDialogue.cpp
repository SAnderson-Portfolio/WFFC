#include "InspectorDialogue.h"

IMPLEMENT_DYNAMIC(InspectorDialogue, CDialogEx)

BEGIN_MESSAGE_MAP(InspectorDialogue, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE2, &InspectorDialogue::OnTvnSelchangedTree2)
	ON_COMMAND(IDOK, &InspectorDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &InspectorDialogue::OnBnClickedOk)
	ON_WM_PAINT()
	ON_STN_CLICKED(IDC_STATIC_TEXT, &InspectorDialogue::OnStnClickedStaticText)
	ON_STN_CLICKED(IDC_PICTURE_STATIC, &InspectorDialogue::OnStnClickedPictureStatic)
	ON_BN_CLICKED(IDC_Wireframe, &InspectorDialogue::OnBnClickedWireframe)
	ON_BN_CLICKED(IDC_RenderCheck, &InspectorDialogue::OnBnClickedRendercheck)
	ON_BN_CLICKED(IDC_ResetTransformButton, &InspectorDialogue::OnBnClickedResetTransformbutton)
	ON_COMMAND(IDCLOSE, &InspectorDialogue::OnIdclose)
	ON_BN_CLICKED(IDCANCEL, &InspectorDialogue::OnBnClickedCancel)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


void InspectorDialogue::ClearSelection()
{
	m_treeCtrl.DeleteAllItems();
	m_wireframeControl.EnableWindow(FALSE);
	m_renderControl.EnableWindow(FALSE);
	m_IDText.SetWindowText(L"None");
	//m_currentSelection = nullptr;
	treeSet = false;
}

BOOL InspectorDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog. The framework does this automatically
	// when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);            // Set big icon
	SetIcon(m_hIcon, FALSE);            // Set small icon
	m_renderCheck = true;
	m_wireframeCheck = false;
	m_changesRequireAdded = false;
	m_displayListUpdateRequired = false;
	m_renderControl.SetCheck(1);
	return TRUE;
}

void InspectorDialogue::PostNcDestroy()
{
	
}

void InspectorDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void InspectorDialogue::OnLBnSelectChange()
{
}

void InspectorDialogue::OnTvnSelchangedTree2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

InspectorDialogue::InspectorDialogue(CWnd* pParent, std::vector<SceneObject>* sceneGraph) : CDialogEx(IDD_DIALOG2, pParent)
, m_wireframeCheck(FALSE)
, m_renderCheck(FALSE)
{
	m_hIcon = LoadIcon(NULL, MAKEINTRESOURCE(460));
	m_sceneGraph = sceneGraph;
	
}

InspectorDialogue::InspectorDialogue(CWnd * pParent)
{
}

InspectorDialogue::~InspectorDialogue()
{
}

void InspectorDialogue::SetObjectData(std::vector<SceneObject>* sceneGraph, int* selection)
{

	m_sceneGraph = sceneGraph;
	m_currentSelection = selection;
	treeSet = false;
	previousSelectionID = *m_currentSelection;
	if (*m_currentSelection >= 0 && *m_currentSelection < m_sceneGraph->size())
	{
		m_wireframeControl.EnableWindow(TRUE);
		m_renderControl.EnableWindow(TRUE);
		m_wireframeCheck = sceneGraph->at(*m_currentSelection).editor_wireframe;
		m_renderCheck = sceneGraph->at(*m_currentSelection).editor_render;
	}
	else
	{
		m_wireframeControl.EnableWindow(FALSE);
		m_renderControl.EnableWindow(FALSE);
		m_IDText.SetWindowText(L"None");
	}

	SetSelectionTree();
	
	
}

void InspectorDialogue::SetImage(HBITMAP image)
{
}


void InspectorDialogue::DoDataExchange(CDataExchange * pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE2, m_treeCtrl);
	DDX_Control(pDX, IDC_STATIC_TEXT, m_IDText);
	DDX_Control(pDX, IDC_PICTURE_STATIC, m_InspectorImage);
	DDX_Control(pDX, IDC_Wireframe, m_wireframeControl);
	DDX_Check(pDX, IDC_Wireframe, m_wireframeCheck);
	DDX_Check(pDX, IDC_RenderCheck, m_renderCheck);
	DDX_Control(pDX, IDC_RenderCheck, m_renderControl);
	DDX_Control(pDX, IDC_ResetTransformButton, m_transformResetButton);
}

void InspectorDialogue::End()
{
	DestroyWindow();
}

void InspectorDialogue::Select()
{
}


void InspectorDialogue::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages
	if (*m_currentSelection == -1 || previousSelectionID == -1)
	{
		SetSelectionTree();
		m_wireframeControl.EnableWindow(TRUE);
		m_renderControl.EnableWindow(TRUE);
	}
	else
	{
		m_wireframeControl.EnableWindow(TRUE);
		m_renderControl.EnableWindow(TRUE);
		UpdateSectionTree();
	}
}

void InspectorDialogue::SetSelectionTree()
{
	m_treeCtrl.DeleteAllItems();
	HTREEITEM hNodeName, hValueName;
	std::wstring optionString;
	if (*m_currentSelection >= 0 && *m_currentSelection < m_sceneGraph->size())
	{
		DeleteObject(m_inspectorBitmap);
		SceneObject selectedObject = m_sceneGraph->at(*m_currentSelection);
		std::wstring optionString;

		optionString = L"Item ID: " + std::to_wstring(*m_currentSelection);
		m_IDText.SetWindowText(optionString.c_str());

		//Transform options
		hNodeName = m_treeCtrl.InsertItem(L"Transform", TVI_ROOT);
		//Position
		hValueName = m_treeCtrl.InsertItem(L"Position", hNodeName);
		optionString = L"X: " + std::to_wstring(selectedObject.posX) + L" Y: " + std::to_wstring(selectedObject.posY) + L" Z: " + std::to_wstring(selectedObject.posX);
		m_treeCtrl.InsertItem(optionString.c_str(), hValueName);
		//Rotation
		hValueName = m_treeCtrl.InsertItem(L"Rotation", hNodeName);
		optionString = L"X: " + std::to_wstring(selectedObject.rotX) + L" Y: " + std::to_wstring(selectedObject.rotY) + L" Z: " + std::to_wstring(selectedObject.rotZ);
		m_treeCtrl.InsertItem(optionString.c_str(), hValueName);
		//Scale
		hValueName = m_treeCtrl.InsertItem(L"Scale", hNodeName);
		optionString = L"X: " + std::to_wstring(selectedObject.scaX) + L" Y: " + std::to_wstring(selectedObject.scaY) + L" Z: " + std::to_wstring(selectedObject.scaZ);
		m_treeCtrl.InsertItem(optionString.c_str(), hValueName);
		//Inspection window
		//TODO: Find out how to add an image to the tree and add here

		/*m_inspectorBitmap = CreateBitmap(100, 125, 1, 32, m_displayList->at(*m_currentSelection).m_texture_diffuse);
		m_InspectorImage.SetBitmap(m_inspectorBitmap);*/
		
	}
	else
	{
		/*optionString = L"Item ID: " + std::to_wstring(0);

		m_IDText.SetWindowText(optionString.c_str());*/
		optionString = L"None";
		m_IDText.SetWindowText(optionString.c_str());
		hNodeName = m_treeCtrl.InsertItem(L"Please select an object", TVI_ROOT);
	}
	previousSelectionID = *m_currentSelection;
}

void InspectorDialogue::UpdateSectionTree()
{
	
	if (*m_currentSelection >= 0 && *m_currentSelection < m_sceneGraph->size())
	{
		DeleteObject(m_inspectorBitmap);
		std::wstring optionString;
		HTREEITEM hNodeName, hValueName;
		HTREEITEM root = m_treeCtrl.GetRootItem();
		HTREEITEM transformItem = m_treeCtrl.GetChildItem(root);
		HTREEITEM nextItem = m_treeCtrl.GetNextSiblingItem(transformItem);
		HTREEITEM currentItem;
		optionString = L"Item ID: " + std::to_wstring(*m_currentSelection);
		SceneObject selectedObject = m_sceneGraph->at(*m_currentSelection);
		m_IDText.SetWindowText(optionString.c_str());
		//Position
		currentItem = m_treeCtrl.GetChildItem(transformItem);
		optionString = L"X: " + std::to_wstring(selectedObject.posX) + L" Y: " + std::to_wstring(selectedObject.posY) + L" Z: " + std::to_wstring(selectedObject.posZ);
		m_treeCtrl.SetItemText(currentItem, optionString.c_str());
		//Rotation
		currentItem = m_treeCtrl.GetChildItem(nextItem);
		optionString = L"X: " + std::to_wstring(selectedObject.rotX) + L" Y: " + std::to_wstring(selectedObject.rotY) + L" Z: " + std::to_wstring(selectedObject.rotZ);
		m_treeCtrl.SetItemText(currentItem, optionString.c_str());

		//Update nextItem to rotation
		nextItem = m_treeCtrl.GetNextSiblingItem(nextItem);

		//Scale
		currentItem = m_treeCtrl.GetChildItem(nextItem);
		optionString = L"X: " + std::to_wstring(selectedObject.scaX) + L" Y: " + std::to_wstring(selectedObject.scaY) + L" Z: " + std::to_wstring(selectedObject.scaZ);
		m_treeCtrl.SetItemText(currentItem, optionString.c_str());

		//m_inspectorBitmap = CreateBitmap(100, 125, 1, 32, m_displayList->at(*m_currentSelection).m_texture_diffuse);
		
		//m_InspectorImage.SetBitmap(m_inspectorBitmap);

	}
	previousSelectionID = *m_currentSelection;
}


void InspectorDialogue::OnStnClickedStaticText()
{
	// TODO: Add your control notification handler code here
}


void InspectorDialogue::OnStnClickedPictureStatic()
{
	// TODO: Add your control notification handler code here
}



void InspectorDialogue::OnBnClickedWireframe()
{
	UpdateData(TRUE);
	if (*m_currentSelection >= 0 && *m_currentSelection < m_sceneGraph->size())
	{
		m_sceneGraph->at(*m_currentSelection).editor_wireframe = m_wireframeCheck;
		m_displayListUpdateRequired = true;
	}
}


void InspectorDialogue::OnBnClickedRendercheck()
{
	UpdateData(TRUE);
	if (*m_currentSelection >= 0 && *m_currentSelection < m_sceneGraph->size())
	{
		m_sceneGraph->at(*m_currentSelection).editor_render = m_renderCheck;
		m_displayListUpdateRequired = true;
	}
}


void InspectorDialogue::OnBnClickedResetTransformbutton()
{
	if (*m_currentSelection >= 0 && *m_currentSelection < m_sceneGraph->size())
	{
		UpdateSectionTree();
		m_changesRequireAdded = true;
		m_displayListUpdateRequired = true;
	}
}


void InspectorDialogue::OnIdclose()
{
	// TODO: Add your command handler code here
	CDialog::OnClose();
	DestroyWindow();
}


void InspectorDialogue::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnCancel();
}


void InspectorDialogue::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
	DestroyWindow();
}
