
// SobeyWatchDogDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CSobeyWatchDogDlg 对话框
class CSobeyWatchDogDlg : public CDialog
{
// 构造
public:
	CSobeyWatchDogDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SOBEYWATCHDOG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);

// 实现
protected:
	HICON m_hIcon;
	std::map<CString, PROCESSINFO>	m_mapProcess;
	BOOL							m_bWork;
	HANDLE							m_hThread;
	CMenu							m_Menu;
	CListCtrl						m_ListProcess;
	CProgressCtrl					m_Progress;
	CButton							m_btnWatch;
	int								m_nInterval;
	int								m_nTolerateCount;
	CString							m_strImage;
	CWriteMsgToFile					LogerWatchDog;
	CEdit							m_edtImage;
	CString							GetProcessLocal(CString strPath, CString strName);
	CString							GetPorcessStatus(int nStatus);
	void							UpdateProcessList(const PROCESSINFO& Process);
	void							SaveProcesses();
	void							LoadProcesses();
	void							EndWatch();
	HWND							GetWindowFromProcessID(DWORD dwProcessID);
	BOOL							IsExit();
	void							ForbidSystemErrorUI(BOOL bForbid = TRUE);

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	DWORD IsExistProcess(CString strName, CString strPath);
	DWORD WatchProcess();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDestroy();
	afx_msg void OnNMRClickListProcess(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeleteselecteditem();
	afx_msg void OnEnKillfocusEditImage();
};
