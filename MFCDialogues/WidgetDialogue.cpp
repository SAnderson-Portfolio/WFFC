// WidgetDialogue.cpp : implementation file
//

#include "WidgetDialogue.h"


// WidgetDialogue dialog

IMPLEMENT_DYNAMIC(WidgetDialogue, CDialogEx)

WidgetDialogue::WidgetDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG4, pParent)
{

}

WidgetDialogue::~WidgetDialogue()
{
}

void WidgetDialogue::SetObjectData(TransformWidgets * widgetObject)
{
	m_transformWidgets = widgetObject;
	//Tolerance
	m_toleranceSpin.SetBuddy(&m_toleranceEdit);
	m_toleranceSpin.SetRange(1, 20);
	m_toleranceSpin.SetPos(m_transformWidgets->GetTolerance());
	//Rotate
	m_rotateSpin.SetBuddy(&m_rotateEdit);
	m_rotateSpin.SetRange(1, 20);
	m_rotateSpin.SetPos(m_transformWidgets->GetRotationSpeed());
	//Translation
	m_translateSpin.SetBuddy(&m_translateEdit);
	m_translateSpin.SetRange(1, 20);
	m_translateSpin.SetPos(m_transformWidgets->GetTranslateDistance());
	//Scaling
	m_scaleSpin.SetBuddy(&m_scaleEdit);
	m_scaleSpin.SetRange(1, 20);
	m_scaleSpin.SetPos(m_transformWidgets->GetScaleAmount());
}

BOOL WidgetDialogue::OnInitDialog()
{

	CDialogEx::OnInitDialog();

	// Set the icon for this dialog. The framework does this automatically
	// when the application's main window is not a dialog
	return TRUE;
}

void WidgetDialogue::PostNcDestroy()
{
}

void WidgetDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
	DestroyWindow();
}

void WidgetDialogue::End()
{
	DestroyWindow();
}

void WidgetDialogue::OnClose()
{
	// TODO: Add your command handler code here
	CDialog::OnClose();
	DestroyWindow();
}

void WidgetDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SPIN2, m_rotateSpin);
	DDX_Control(pDX, IDC_RotateEdit, m_rotateEdit);
	DDX_Control(pDX, IDC_TOLERANCEEDIT, m_toleranceEdit);
	DDX_Control(pDX, IDC_TOLERANCESPIN, m_toleranceSpin);
	DDX_Control(pDX, IDC_TranslateSpin, m_translateSpin);
	DDX_Control(pDX, IDC_TranslateEdit, m_translateEdit);
	DDX_Control(pDX, IDC_ScaleEdit, m_scaleEdit);
	DDX_Control(pDX, IDC_ScaleSpin, m_scaleSpin);
}


BEGIN_MESSAGE_MAP(WidgetDialogue, CDialogEx)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN2, &WidgetDialogue::OnDeltaposSpin2)
	ON_COMMAND(IDOK, &WidgetDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &WidgetDialogue::OnBnClickedOk)
	ON_COMMAND(IDCLOSE, &WidgetDialogue::OnClose)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TOLERANCESPIN, &WidgetDialogue::OnDeltaposTolerancespin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_TranslateSpin, &WidgetDialogue::OnDeltaposTranslatespin)
	ON_NOTIFY(UDN_DELTAPOS, IDC_ScaleSpin, &WidgetDialogue::OnDeltaposScalespin)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// WidgetDialogue message handlers


void WidgetDialogue::OnDeltaposSpin2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	int value = 0;
	CString currentSelectionValue;

	m_rotateEdit.GetWindowText(currentSelectionValue);

	value = _ttoi(currentSelectionValue);
	if (m_transformWidgets)
	{
		m_transformWidgets->SetRotationSpeed(value);
	}
	*pResult = 0;
}


void WidgetDialogue::OnDeltaposTolerancespin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	int value = 0;
	CString currentSelectionValue;

	m_toleranceEdit.GetWindowText(currentSelectionValue);

	value = _ttoi(currentSelectionValue);
	if (m_transformWidgets)
	{
		m_transformWidgets->SetTolerance(value);
	}

	*pResult = 0;
}


void WidgetDialogue::OnDeltaposTranslatespin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	int value = 0;
	CString currentSelectionValue;

	m_translateEdit.GetWindowText(currentSelectionValue);

	value = _ttoi(currentSelectionValue);
	if (m_transformWidgets)
	{
		m_transformWidgets->SetTranslateDistance(value);
	}
	*pResult = 0;
}


void WidgetDialogue::OnDeltaposScalespin(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: Add your control notification handler code here
	int value = 0;
	CString currentSelectionValue;

	m_scaleEdit.GetWindowText(currentSelectionValue);

	value = _ttoi(currentSelectionValue);
	if (m_transformWidgets)
	{
		m_transformWidgets->SetScaleAmount(value);
	}
	*pResult = 0;
}
