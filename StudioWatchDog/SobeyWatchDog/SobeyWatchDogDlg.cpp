
// SobeyWatchDogDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SobeyWatchDog.h"
#include "SobeyWatchDogDlg.h"
#include <windows.h>  
#include <TlHelp32.h>  
#include "DlgSubInfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

static CString FolderFromPath(CString& strFullPath)
{
	CString strFolder = _T("");
	int nPos = strFullPath.ReverseFind('\\');
	if (nPos == -1)
		return strFolder;
	strFolder = strFullPath.Left(nPos);//��ȡ�ļ���

	return strFolder;
}

DWORD WINAPI THREAD_WORK(LPVOID lp)
{
	CSobeyWatchDogDlg* pDog = (CSobeyWatchDogDlg*)lp;
	return pDog->WatchProcess();
}

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

CSobeyWatchDogDlg::CSobeyWatchDogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSobeyWatchDogDlg::IDD, pParent)
{
	m_hIcon		= AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_hThread	= NULL;
	m_bWork		= FALSE;
	INIT_LOGER(LogerWatchDog, _T("LogPath"), _T("StudioWatchDog"));
	WRITE_LOG(LogerWatchDog, 0, FALSE, _T("******************** StudioWatchDog Initialized ********************"));
	ForbidSystemErrorUI();
}

void CSobeyWatchDogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_ListProcess);
	DDX_Control(pDX, IDOK, m_btnWatch);
	DDX_Control(pDX, IDC_PROGRESS, m_Progress);
	DDX_Control(pDX, IDC_EDIT_IMAGE, m_edtImage);
}

BEGIN_MESSAGE_MAP(CSobeyWatchDogDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_ADD, &CSobeyWatchDogDlg::OnBnClickedButtonAdd)
	ON_BN_CLICKED(IDOK, &CSobeyWatchDogDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CSobeyWatchDogDlg::OnBnClickedCancel)
	ON_WM_DESTROY()
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROCESS, &CSobeyWatchDogDlg::OnNMRClickListProcess)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_DELETESELECTEDITEM, &CSobeyWatchDogDlg::OnDeleteselecteditem)
	ON_EN_KILLFOCUS(IDC_EDIT_IMAGE, &CSobeyWatchDogDlg::OnEnKillfocusEditImage)
END_MESSAGE_MAP()


// CSobeyWatchDogDlg ��Ϣ�������

BOOL CSobeyWatchDogDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	m_Menu.LoadMenu(IDR_MENU_RIGHT);

	m_ListProcess.InsertColumn(0, _T("ID"), LVCFMT_CENTER, 55);
	m_ListProcess.InsertColumn(1, _T("Process"), LVCFMT_CENTER, 195);
	m_ListProcess.InsertColumn(2, _T("Status"), LVCFMT_CENTER, 90);
	m_ListProcess.InsertColumn(3, _T("Abnormal count"), LVCFMT_CENTER, 100);
	m_ListProcess.InsertColumn(4, _T("Location"), LVCFMT_CENTER, 320);
	m_ListProcess.SetExtendedStyle(m_ListProcess.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_SINGLEROW);

	LoadProcesses();
	m_Progress.SetRange(0, m_nInterval);
	m_edtImage.SetWindowText(m_strImage);
	if(m_mapProcess.size() > 0)
	{
		OnBnClickedOk();
	}
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

BOOL CSobeyWatchDogDlg::PreTranslateMessage(MSG* pMsg)
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

void CSobeyWatchDogDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CSobeyWatchDogDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CSobeyWatchDogDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CString CSobeyWatchDogDlg::GetProcessLocal(CString strPath, CString strName)
{
	strPath.Replace(strName, _T(""));
	return strPath;
}

CString CSobeyWatchDogDlg::GetPorcessStatus(int nStatus)
{
	CString strStatus = _T("");
	switch (nStatus)
	{
	case STATUS_PROCESS_NOWATCH:
		strStatus = _T("NO WATCH");
		break;
	case STATUS_PROCESS_IDLE:
		strStatus = _T("IDLE");
		break;
	case STATUS_PROCESS_STARTING:
		strStatus = _T("STARTING");
		break;
	case STATUS_PROCESS_RUNNING:
		strStatus = _T("RUNNING");
		break;
	}
	return strStatus;
}

void CSobeyWatchDogDlg::UpdateProcessList(const PROCESSINFO& Process)
{
	int nIndex = -1;
	for (int i = 0; i < m_ListProcess.GetItemCount(); i++)
	{
		CString strName  = m_ListProcess.GetItemText(i, 1);
		CString strLocal = m_ListProcess.GetItemText(i, 4);
		if (0 == Process.strName.Compare(strName) && 0 == Process.strLocation.Compare(strLocal))
		{
			nIndex = i;
		}
	}
	if (-1 == nIndex)
	{
		nIndex = 0;
		m_ListProcess.InsertItem(nIndex, NULL);
	}
	CString strValue = _T("");
	strValue.Format(_T("%d"), Process.dwProcessID);
	m_ListProcess.SetItemText(nIndex, 0, strValue);
	m_ListProcess.SetItemText(nIndex, 1, Process.strName);
	if(Process.nAbnormalCount > 0)
	{
		strValue = _T("ABNORMAL");
	}
	else
	{
		strValue = GetPorcessStatus(Process.nStatus);
	}
	m_ListProcess.SetItemText(nIndex, 2, strValue);
	strValue.Format(_T("%d"), Process.nAbnormalCount);
	m_ListProcess.SetItemText(nIndex, 3, strValue);
	m_ListProcess.SetItemText(nIndex, 4, Process.strLocation);
}

void CSobeyWatchDogDlg::SaveProcesses()
{
	CString strPath  = _T("");
	CString strKey   = _T("");
	CString strValue = _T("");
	strPath.Format(_T("%s\\config.dat"), GetAppFolder());
	DeleteFile(strPath);
	m_edtImage.GetWindowText(m_strImage);
	CString strImage = m_strImage;
	if(strImage.IsEmpty())
	{
		strImage = _T("\\");
	}
	::WritePrivateProfileString(_T("SET"), _T("IMAGE"), m_strImage, strPath);
	strValue.Format(_T("%d"), m_nInterval);
	::WritePrivateProfileString(_T("SET"), _T("INTERVAL"), strValue, strPath);
	strValue.Format(_T("%d"), m_nTolerateCount);
	::WritePrivateProfileString(_T("SET"), _T("TOLERATE"), strValue, strPath);

	std::map<CString, PROCESSINFO>::iterator iter = m_mapProcess.begin();
	for (int i = 1; iter != m_mapProcess.end(); iter++, i++)
	{
		strKey.Format(_T("NODE%03d"), i);
		::WritePrivateProfileString(strKey, _T("NAME"), iter->second.strName, strPath);
		::WritePrivateProfileString(strKey, _T("LOCATION"), iter->second.strLocation, strPath);
		WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::SaveProcesses Process[%s] Location[%s]"), iter->second.strName, iter->second.strLocation);
	}
}

void CSobeyWatchDogDlg::LoadProcesses()
{
	CString strPath = _T("");
	CString strKey = _T("");
	strPath.Format(_T("%s\\config.dat"), GetAppFolder());
	m_nInterval			= ::GetPrivateProfileInt(_T("SET"), _T("INTERVAL"), 10, strPath);
	if(m_nInterval < 3)
	{
		m_nInterval = 3;
	}
	m_nTolerateCount	= ::GetPrivateProfileInt(_T("SET"), _T("TOLERATE"), 2, strPath);
	if(m_nTolerateCount < 1)
	{
		m_nTolerateCount = 1;
	}
	TCHAR tImage[256] = {0};
	::GetPrivateProfileString(_T("SET"), _T("IMAGE"), _T("\\Sobey"), tImage, 256, strPath);
	m_strImage = tImage;
	WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::LoadProcesses Image path[%s] Interval[%d]"), m_strImage, m_nInterval);
	for (int i = 1; i < 1001; i++)
	{
		TCHAR tBuf[256] = { 0 };
		strKey.Format(_T("NODE%03d"), i);
		PROCESSINFO Info;
		::GetPrivateProfileString(strKey, _T("NAME"), _T(""), tBuf, 256, strPath);
		Info.strName = tBuf;
		if (Info.strName.IsEmpty())
		{
			return;
		}
		::GetPrivateProfileString(strKey, _T("LOCATION"), _T(""), tBuf, 256, strPath);
		Info.strLocation = tBuf;
		if (Info.strLocation.IsEmpty())
		{
			return;
		}
		CString strDirectory = _T("");
		strDirectory.Format(_T("%s%s"), Info.strLocation, Info.strName);
		m_mapProcess[strDirectory] = Info;
		WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::LoadProcesses Process[%s] Location[%s]"), Info.strName, Info.strLocation);
		UpdateProcessList(Info);
	}
}

BOOL CSobeyWatchDogDlg::IsExit()
{
	if (m_bWork)
	{
		return FALSE;
	}
	if (IDYES == MessageBox(_T("Do you want exit?"), _T("Warnning"), MB_YESNO))
	{
		m_bWork = FALSE;
		if (NULL != m_hThread)
		{
			CloseHandle(m_hThread);
			m_hThread = NULL;
		}
		SaveProcesses();
		ForbidSystemErrorUI(FALSE);
		WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::IsExit User sure exit"));
		exit(0);
	}
	else
	{
		return FALSE;
	}
}

DWORD CSobeyWatchDogDlg::WatchProcess()
{
	int nCounter = m_nInterval;
	m_Progress.SetPos(0);
	BOOL bStart = TRUE;
	while (m_bWork)
	{
		if(nCounter < m_nInterval)
		{
			nCounter++;
			m_Progress.SetPos(nCounter);
		}
		else
		{
			std::map<CString, PROCESSINFO>::iterator iter = m_mapProcess.begin();
			for (; iter != m_mapProcess.end(); iter++)
			{
				DWORD dwProcessID = IsExistProcess(iter->second.strName, iter->first);
				WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::WatchProcess process[%s] ID[%d]"), iter->first, dwProcessID);
				if (dwProcessID > 0)
				{
					iter->second.dwProcessID = dwProcessID;
					if(iter->second.nAbnormalCount >= m_nTolerateCount)
					{
						HANDLE hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessID);
						if (NULL != hTargetProcess)
						{
							if (TerminateProcess(hTargetProcess, 0))
							{
								WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::WatchProcess terminal process[%s]"), iter->first);
								iter->second.dwProcessID	= 0;
								iter->second.nStatus		= STATUS_PROCESS_IDLE;
								iter->second.nAbnormalCount = 0;
								UpdateProcessList(iter->second);
								continue;
							}
							else
							{
								WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::WatchProcess process[%s] ID[%d] TerminateProcess failed[%d]"), iter->first, dwProcessID, GetLastError());
							}
						}
						else
						{
							WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::WatchProcess process[%s] ID[%d] OpenProcess failed[%d]"), iter->first, dwProcessID, GetLastError());
						}
					}
					if(NULL == GetWindowFromProcessID(dwProcessID))
					{
						iter->second.nAbnormalCount++;
						WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::WatchProcess Process[%s] is abnormal count[%d]"), iter->first, iter->second.nAbnormalCount);
					}
					else
					{
						iter->second.nStatus		= STATUS_PROCESS_RUNNING;
						iter->second.nAbnormalCount = 0;
					}
				}
				else
				{
					if(iter->second.nAbnormalCount < m_nTolerateCount && !bStart)
					{
						iter->second.nAbnormalCount++;
					}
					else
					{
						CString strEXEPath = iter->first;
						CString strNewWorkFolder = FolderFromPath(strEXEPath);
						WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg:: WorkExe=[%s], WorkFolder=[%s]"), strEXEPath, strNewWorkFolder);

						TCHAR szOldWorkFolder[MAX_PATH] = { 0 };
						GetCurrentDirectory(MAX_PATH, szOldWorkFolder);
						CString strOldWorkFolder; strOldWorkFolder = szOldWorkFolder;

						SetCurrentDirectory(strNewWorkFolder);
						ShellExecute(NULL, _T("open"), iter->first, _T(""), strNewWorkFolder, SW_SHOW);
						SetCurrentDirectory(strOldWorkFolder);

						iter->second.nStatus		= STATUS_PROCESS_STARTING;
						iter->second.nAbnormalCount = 0;
						WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::WatchProcess Start Process[%s]"), iter->first);
					}
				}
				UpdateProcessList(iter->second);
			}
			nCounter = 0;
		}
		Sleep(1000);
		bStart = FALSE;
	}
	return 0;
}
void CSobeyWatchDogDlg::StartProcess(const CString& strExePath)
{
	// ��CStringת��ΪLPCTSTR����
	LPCTSTR lpParam = (LPCTSTR)strExePath;

	// �������̵���ز���
	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	// ��ʼ��STARTUPINFO�ṹ
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);

	// ��������
	if (CreateProcess(NULL, (LPTSTR)lpParam, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi))
	{
		// �ȴ����̽���
		WaitForSingleObject(pi.hProcess, INFINITE);

		// �رս��̺��̵߳ľ��
		CloseHandle(pi.hProcess);
		CloseHandle(pi.hThread);
	}
	else
	{
		// ��������ʧ��
		DWORD error = GetLastError();
		WRITE_LOG(LogerWatchDog, 0, FALSE, _T("Failed to create process. Error code: %d"), error);
	}
}

DWORD CSobeyWatchDogDlg::IsExistProcess(CString strName, CString strPath)
{
	PROCESSENTRY32 pe;
	DWORD dwProcessID = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pe.dwSize = sizeof(PROCESSENTRY32);
	if (!Process32First(hSnapshot, &pe))
	{
		WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::IsExistProcess Get proccess32first failed name[%s] path[%s]"), strName, strPath);
		if(NULL != hSnapshot)
		{
			CloseHandle(hSnapshot);
		}
		return 0;
	}
	while (TRUE)
	{
		pe.dwSize = sizeof(PROCESSENTRY32);
		if (!Process32Next(hSnapshot, &pe))
		{
			WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::IsExistProcess Get proccess32next end. name[%s] path[%s]"), strName, strPath);
			break;
		}
		CString strTemp = pe.szExeFile;
		strTemp = strTemp.MakeLower();
		strName = strName.MakeLower();
		if (0 == strTemp.Compare(strName))
		{
			HANDLE hTargetProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe.th32ProcessID);
			if(NULL != hTargetProcess)
			{
				TCHAR tBuf[256] = {0};
				::GetProcessImageFileName(hTargetProcess, tBuf, 256);
				CString strImagePath = tBuf;
				strImagePath = strImagePath.MakeLower();
				WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::IsExistProcess Get Process location[%s]"), strImagePath);
				int nLen = m_strImage.GetLength();
				if(m_strImage.Find(_T("\\")) >= 0)
				{
					nLen++;
				}
				strTemp = strPath.Right(strPath.GetLength() - 2 - nLen);
				strTemp = strTemp.MakeLower();
				if(strImagePath.Find(strTemp) >= 0)
				{
					dwProcessID = pe.th32ProcessID;
					break;
				}
				else
				{
					WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::IsExistProcess identical Process[%s] but imagepath[%s] path[%s]"), strName, strImagePath, strPath);
					continue;
				}
			}
		}
	}
	CloseHandle(hSnapshot);
	return dwProcessID;
}

void CSobeyWatchDogDlg::OnBnClickedButtonAdd()
{
	CFileDialog FileDlg(TRUE, _T(""), _T(""), OFN_HIDEREADONLY | OFN_READONLY, _T("(*.exe)|*.exe||"), NULL);
	if(IDOK == FileDlg.DoModal())
	{
		CString strPath = FileDlg.GetPathName();
		PROCESSINFO Process;
		Process.strName		= FileDlg.GetFileName();
		Process.strLocation = GetProcessLocal(strPath, Process.strName);
		if (Process.strLocation.IsEmpty())
		{
			AfxMessageBox(_T("The path that selected is invalid!"));
			return;
		}
		std::map<CString, PROCESSINFO>::iterator iter = m_mapProcess.find(strPath);
		if (iter != m_mapProcess.end())
		{
			return;
		}
		m_mapProcess[strPath] = Process;
		UpdateProcessList(Process);
		SaveProcesses();
	}
}

void CSobeyWatchDogDlg::OnBnClickedOk()
{
	if (!m_bWork && 0 >= m_mapProcess.size())
	{
		AfxMessageBox(_T("No any proccess need to watched!"));
		return;
	}
	m_bWork = !m_bWork;
	m_btnWatch.SetWindowText(m_bWork ? _T("Stop Watch") : _T("Watch"));
	((CButton*)(GetDlgItem(IDC_BUTTON_ADD)))->EnableWindow(!m_bWork);
	((CButton*)(GetDlgItem(IDCANCEL)))->EnableWindow(!m_bWork);
	m_edtImage.SetReadOnly(m_bWork);
	if (m_bWork)
	{
		m_hThread = CreateThread(NULL, 0, THREAD_WORK, this, 0, NULL);
	}
	else
	{
		EndWatch();
	}
}

void CSobeyWatchDogDlg::EndWatch()
{
	if(NULL != m_hThread)
	{
		CloseHandle(m_hThread);
		m_hThread = NULL;
	}
	m_Progress.SetPos(0);
	std::map<CString, PROCESSINFO>::iterator iter = m_mapProcess.begin();
	for (; iter != m_mapProcess.end(); iter++)
	{
		iter->second.dwProcessID	= 0;
		iter->second.nAbnormalCount = 0;
		iter->second.nStatus		= STATUS_PROCESS_NOWATCH;
		UpdateProcessList(iter->second);
	}
}

void CSobeyWatchDogDlg::OnBnClickedCancel()
{
	IsExit();
}

void CSobeyWatchDogDlg::OnDestroy()
{
	IsExit();
}

void CSobeyWatchDogDlg::OnNMRClickListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nSel = m_ListProcess.GetSelectionMark();
	if(nSel < 0 || m_bWork)
	{
		return;
	}
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	CMenu *pSubMenu = m_Menu.GetSubMenu(0);
	if (pSubMenu)
	{
		POINT pt;
		GetCursorPos(&pt);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
	}
	*pResult = 0;
}

void CSobeyWatchDogDlg::OnDeleteselecteditem()
{
	int nSel = m_ListProcess.GetSelectionMark();
	if(nSel >= 0)
	{
		if(IDYES != MessageBox(_T("Do you want delete the item that selected?"), _T("Warnning"), MB_YESNO))
		{
			return;
		}
		CString strKey = _T("");
		strKey.Format(_T("%s%s"), m_ListProcess.GetItemText(nSel, 4), m_ListProcess.GetItemText(nSel, 1));
		WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::OnDeleteselecteditem Process[%s]"), strKey);
		std::map<CString, PROCESSINFO>::iterator iter = m_mapProcess.find(strKey);
		if(iter != m_mapProcess.end())
		{
			m_mapProcess.erase(iter);
		}
		m_ListProcess.DeleteItem(nSel);
		SaveProcesses();
	}
}

HWND CSobeyWatchDogDlg::GetWindowFromProcessID(DWORD dwProcessID)
{
	HWND hWnd = ::GetTopWindow(0);
	WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::GetWindowFromProcessID start"));
	while (NULL != hWnd)
	{
		DWORD dwPID = 0;
		DWORD dwTID = ::GetWindowThreadProcessId(hWnd, &dwPID);
		if(NULL != dwTID)
		{
			if(dwPID == dwProcessID)
			{
				BOOL bHanged = IsHungAppWindow(hWnd);
				WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::GetWindowFromProcessID hwnd[%d] PID[%d] ID[%d] Hang[%d]"), hWnd, dwPID, dwProcessID, bHanged);
				if(bHanged)
				{
					return NULL;
				}
				return hWnd;
			}
		}
		hWnd = ::GetNextWindow(hWnd , GW_HWNDNEXT);
	}
	return NULL;
}

void CSobeyWatchDogDlg::OnEnKillfocusEditImage()
{
	CString strImage = _T("");
	m_edtImage.GetWindowText(strImage);
	if(0 != m_strImage.Compare(strImage))
	{
		m_strImage = strImage;
	}
	SaveProcesses();
}

void CSobeyWatchDogDlg::ForbidSystemErrorUI(BOOL bForbid)
{
	HKEY	hKey			= NULL;
	DWORD	dwErr			= 0;
	DWORD	dwDisp			= 0;
	DWORD	dwWowFlags[]	= { KEY_WOW64_32KEY, KEY_WOW64_64KEY };
	DWORD	dwWowCount		= ARRAYSIZE(dwWowFlags);
	DWORD	dwValue			= 1;
	if(!bForbid)
	{
		dwValue = 0;
	}
	try
	{
		for(int i = 0; i < dwWowCount; i++)
		{
			LSTATUS lResult = ::RegCreateKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\Microsoft\\Windows\\Windows Error Reporting"), 0, REG_NONE,
				REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS | dwWowFlags[i], NULL, &hKey, &dwDisp);
			if(ERROR_SUCCESS != lResult || NULL == hKey)
			{
				dwErr = GetLastError();
				WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::ForbidSystemErrorUI CreateKey failed Err[%d] Key86[%08x] index[%d]"), dwErr, hKey, i);
				continue;
			}
			DWORD dwSize  = sizeof(DWORD);
			lResult = RegSetValueEx(hKey, _T("DontShowUI"), NULL, REG_DWORD, (LPBYTE) &dwValue, dwSize);
			if(ERROR_SUCCESS != lResult)
			{
				dwErr = GetLastError();
				WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::ForbidSystemErrorUI SetValue DtSUI failed Err[%d] index[%d]"), dwErr, i);
			}
			lResult = RegSetValueEx(hKey, _T("Disabled"), NULL, REG_DWORD, (LPBYTE) &dwValue, dwSize);
			if(ERROR_SUCCESS != lResult)
			{
				dwErr = GetLastError();
				WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::ForbidSystemErrorUI SetValue Disabled failed Err[%d] index[%d]"), dwErr, i);
			}
			if(NULL != hKey)
			{
				RegCloseKey(hKey);
			}
		}
	}
	catch(CException* e)
	{
		TCHAR szError[256] = {0};
		e->GetErrorMessage(szError, sizeof(szError));
		WRITE_LOG(LogerWatchDog, 0, FALSE, _T("CSobeyWatchDogDlg::ForbidSystemErrorUI catch Err[%s]"), szError);
	}
}