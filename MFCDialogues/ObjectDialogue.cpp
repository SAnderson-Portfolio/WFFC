// ObjectDialogue.cpp : implementation file
//
#include "ObjectDialogue.h"

// ObjectDialogue dialog

IMPLEMENT_DYNAMIC(ObjectDialogue, CDialogEx)

ObjectDialogue::ObjectDialogue(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG3, pParent)

{

}

ObjectDialogue::~ObjectDialogue()
{
}

void ObjectDialogue::SetObjectData(ObjectLoader* objectLoader, Game* gameObject)
{
		//easily possible to make the data string presented more complex. showing other columns.
	for (int i = 0; i < m_numberOfObjects; i++)
	{
		std::wstring listBoxEntry = m_objectListNames[i];
		m_ObjectList.AddString(listBoxEntry.c_str());
	}
	m_objectLoader = objectLoader;
	m_gameObject = gameObject;
}

BOOL ObjectDialogue::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Set the icon for this dialog. The framework does this automatically
	// when the application's main window is not a dialog
	return TRUE;
}

void ObjectDialogue::PostNcDestroy()
{
}

void ObjectDialogue::OnBnClickedOk()
{
	CDialogEx::OnOK();
}

void ObjectDialogue::End()
{
	DestroyWindow();
}

void ObjectDialogue::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ObjectList);
}


BEGIN_MESSAGE_MAP(ObjectDialogue, CDialogEx)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &ObjectDialogue::OnLvnItemchangedList1)
	ON_COMMAND(IDOK, &ObjectDialogue::End)					//ok button
	ON_BN_CLICKED(IDOK, &ObjectDialogue::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDBLCLK()
	ON_LBN_DBLCLK(IDC_LIST1, &ObjectDialogue::ListDoubleClicked)
END_MESSAGE_MAP()


// ObjectDialogue message handlers


void ObjectDialogue::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: Add your control notification handler code here
	*pResult = 0;
}


void ObjectDialogue::OnClose()
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnClose();
	DestroyWindow();
}


void ObjectDialogue::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default

	CDialogEx::OnLButtonDblClk(nFlags, point);

	
}


void ObjectDialogue::ListDoubleClicked()
{
	// TODO: Add your control notification handler code here
	int index = m_ObjectList.GetCurSel();

	if (m_objectLoader && m_gameObject)
	{
		m_gameObject->AddChanges();
		std::string tempString(m_objectListNames[index].begin(), m_objectListNames[index].end());
		std::string modelFilePath = "database/data/" + tempString + ".cmo";
		std::string textureFilePath = "database/data/" + tempString + ".dds";
		m_objectLoader->CreateDisplayObjectAndAddToSceneGraph(modelFilePath.c_str(), tempString.c_str(), textureFilePath.c_str());
	}
}
