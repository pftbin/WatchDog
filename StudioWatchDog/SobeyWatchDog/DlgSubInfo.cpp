#include "stdafx.h"
#include "SobeyWatchDog.h"
#include "DlgSubInfo.h"

IMPLEMENT_DYNAMIC(CDlgSubInfo, CDialog)

CDlgSubInfo::CDlgSubInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSubInfo::IDD, pParent)
{

}

CDlgSubInfo::~CDlgSubInfo()
{
}

void CDlgSubInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_SUBNAME, m_edtSubName);
}


BEGIN_MESSAGE_MAP(CDlgSubInfo, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgSubInfo::OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CDlgSubInfo::PreTranslateMessage(MSG* pMsg)
{
	if (WM_KEYDOWN == pMsg->message)
	{
		if (VK_ESCAPE == pMsg->wParam || VK_RETURN == pMsg->wParam)
		{
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CDlgSubInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	m_edtSubName.SetWindowText(m_strSubName);
	return TRUE;
}

void CDlgSubInfo::OnBnClickedOk()
{
	CDialog::UpdateData(TRUE);
	m_edtSubName.GetWindowText(m_strSubName);
	CDialog::OnOK();
}