// TerrainDialogue.cpp : implementation file
//


#include "TerrainDialogue.h"

// TerrainDialogue dialog

IMPLEMENT_DYNAMIC(TerrainDialogue, CDialogEx)

TerrainDialogue::TerrainDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG5, pParent)
{

}

TerrainDialogue::~TerrainDialogue()
{
}

void TerrainDialogue::SetObjectData(DisplayChunk * chunkObject)
{
	m_displayChunk = chunkObject;
	//Flatten
	m_flattenSpin.SetBuddy(&m_flattenEdit);
	m_flattenSpin.SetRange(1, 100);
	m_flattenSpin.SetPos(m_displayChunk->GetFlattenHeight());
	//Sculpt
	m_sculptSpin.SetBuddy(&m_sculptEdit);
	m_sculptSpin.SetRange(1, 100);
	m_sculptSpin.SetPos(m_displayChunk->GetTerrainSculptValue());
	//Scale
	m_scaleSpin.SetBuddy(&m_scaleEdit);
	m_scaleSpin.SetRange(1, 100);
	m_scaleSpin.SetPos(m_displayChunk->GetTerrainScaling());

	std::wstring optionString;
	POINT previousPoint = chunkObject->GetPreviousIntersectPoint();
	bool previousTerrain = false;
	optionString = std::to_wstring(chunkObject->GetTerrainHeightAtIndex(previousPoint.x, previousPoint.y));
	m_currentHeight.SetWindowText(optionString.c_str());
	optionString = std::to_wstring(chunkObject->GetTerrainScaleAtIndex(previousPoint.x, previousPoint.y));
	m_currentScale.SetWindowText(optionString.c_str());
}

void TerrainDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SCULPT_EDIT, m_sculptEdit);
	DDX_Control(pDX, IDC_FLATTEN_EDIT, m_flattenEdit);
	DDX_Control(pDX, IDC_SCALE_EDIT, m_scaleEdit);
	DDX_Control(pDX, IDC_SCULPT_SPIN, m_sculptSpin);
	DDX_Control(pDX, IDC_FLATTEN_SPIN, m_flattenSpin);
	DDX_Control(pDX, IDC_SCALE_SPIN, m_scaleSpin);
	DDX_Control(pDX, IDC_WIREFRAME_CHECK, m_wireframeButton);
	DDX_Control(pDX, IDC_CURRENT_HEIGHT, m_currentHeight);
	DDX_Control(pDX, IDC_CURRENT_SCALE, m_currentScale);

	

}


BEGIN_MESSAGE_MAP(TerrainDialogue, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SCULPT_SPIN, &TerrainDialogue::OnDeltaposSculptSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_FLATTEN_SPIN, &TerrainDialogue::OnDeltaposFlattenSpin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SCALE_SPIN, &TerrainDialogue::OnDeltaposScaleSpin)
	ON_BN_CLICKED(IDC_WIREFRAME_CHECK, &TerrainDialogue::OnBnClickedWireframeCheck)
	ON_BN_CLICKED(IDOK, &TerrainDialogue::OnBnClickedOk)
	ON_COMMAND(IDOK, &TerrainDialogue::OnBnClickedOk)
	ON_COMMAND(IDCLOSE, &TerrainDialogue::OnClose)
	ON_WM_PAINT()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// TerrainDialogue message handlers


BOOL TerrainDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	return TRUE;
}

void TerrainDialogue::OnDeltaposSculptSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	int value = 0;
	CString currentSelectionValue;

	m_sculptEdit.GetWindowText(currentSelectionValue);

	value = _ttoi(currentSelectionValue);
	if (m_displayChunk)
	{
		m_displayChunk->SetTerrainSculptValue(value);
	}
	*pResult = 0;
}


void TerrainDialogue::OnDeltaposFlattenSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	int value = 0;
	CString currentSelectionValue;

	m_flattenEdit.GetWindowText(currentSelectionValue);

	value = _ttoi(currentSelectionValue);
	if (m_displayChunk)
	{
		m_displayChunk->SetFlattenHeight(value);
	}
	*pResult = 0;
}


void TerrainDialogue::OnDeltaposScaleSpin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	int value = 0;
	CString currentSelectionValue;

	m_scaleEdit.GetWindowText(currentSelectionValue);

	value = _ttoi(currentSelectionValue);
	if (m_displayChunk)
	{
		m_displayChunk->SetTerrainScaling(value);
	}
	*pResult = 0;
}


void TerrainDialogue::OnBnClickedWireframeCheck()
{
	// TODO: Add your control notification handler code here
	if (m_displayChunk)
		m_displayChunk->ToggleWireframe();
}


void TerrainDialogue::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();
	DestroyWindow();
}


void TerrainDialogue::OnClose()
{
	// TODO: Add your command handler code here
	CDialogEx::OnClose();
	DestroyWindow();
}


void TerrainDialogue::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: Add your message handler code here
					   // Do not call CDialogEx::OnPaint() for painting messages

	if (m_displayChunk)
	{
		std::wstring optionString;
		POINT previousPoint = m_displayChunk->GetPreviousIntersectPoint();
		bool previousTerrain = false;
		optionString = std::to_wstring(m_displayChunk->GetTerrainHeightAtIndex(previousPoint.x, previousPoint.y));
		m_currentHeight.SetWindowText(optionString.c_str());
		optionString = std::to_wstring(m_displayChunk->GetTerrainScaleAtIndex(previousPoint.x, previousPoint.y));
		m_currentScale.SetWindowText(optionString.c_str());
	}
}
