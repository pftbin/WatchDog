#pragma once
#include "afxwin.h"


// CDlgSubInfo �Ի���

class CDlgSubInfo : public CDialog
{
	DECLARE_DYNAMIC(CDlgSubInfo)

public:
	CDlgSubInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgSubInfo();
	CString GetSubName() { return m_strSubName; }
	void SetSubName(CString strSubName) { m_strSubName = strSubName; }

// �Ի�������
	enum { IDD = IDD_DIALOG_SUBINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
private:
	CString		m_strSubName;

public:
	CEdit		m_edtSubName;
	afx_msg void OnBnClickedOk();
};
