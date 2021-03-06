
// FileNameChangeDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "FileNameChange.h"
#include "FileNameChangeDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CFileNameChangeDlg 对话框



CFileNameChangeDlg::CFileNameChangeDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILENAMECHANGE_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CFileNameChangeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_DEALTYPE, m_ComboBox);
	DDX_Control(pDX, IDC_FILENAMESHOW, m_FilenameShow);
}

BEGIN_MESSAGE_MAP(CFileNameChangeDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_FILESEARCH, &CFileNameChangeDlg::OnBnClickedFilesearch)
	ON_CBN_SELCHANGE(IDC_DEALTYPE, &CFileNameChangeDlg::OnCbnSelchangeDealtype)
END_MESSAGE_MAP()


// CFileNameChangeDlg 消息处理程序

BOOL CFileNameChangeDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ComboBox.AddString(L"删除内容");
	m_ComboBox.AddString(L"添加内容");

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CFileNameChangeDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CFileNameChangeDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CFileNameChangeDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CFileNameChangeDlg::OnBnClickedFilesearch()
{
	// TODO: 在此添加控件通知处理程序代码
	CString sPath, sContent;
	GetDlgItemText(IDC_BROWSEEDIT, sPath);
	GetDlgItemText(IDC_MATCHCONTENT, sContent);
	if (sPath.IsEmpty())
	{
		MessageBox(L"文件路径不能为空，请选择！", L"提示",MB_ICONWARNING);
		return;
	}

	int iIndex = ((CComboBox*)GetDlgItem(IDC_DEALTYPE))->GetCurSel();
	int iChangeType = ((CButton *)GetDlgItem(IDC_SEARCHTYPE))->GetCheck();
	int iDealType = iChangeType * 10 + iIndex;

	GetDlgItem(IDC_FILENAMESHOW)->SetWindowText(L"");  //需要先清屏
	FindFiles(sPath, sContent, iDealType);
}


void CFileNameChangeDlg::OnCbnSelchangeDealtype()
{
}

int CFileNameChangeDlg::FindFiles(CString sPath, const CString sMatchContent, const int iDealType)
{
	CString sDataPath, sSearchPath, sOldFileName, sFileName;
	CString sOldDirectoryName, sDirectoryName;
	CFileFind m_find;
	sDataPath = sPath + L"\\*.*";
	BOOL bFind = m_find.FindFile(sDataPath);
	BOOL bChangeed = false;

	m_FilenameShow.SetSel(-1, -1);       //自动滚屏

	while (bFind)
	{
		bFind = m_find.FindNextFileW();
		bChangeed = false;

		if (!m_find.IsDots())
		{
			sSearchPath = m_find.GetFilePath();
			sOldDirectoryName = sSearchPath;
			sDirectoryName = sSearchPath;

			if (m_find.IsDirectory())
			{
				//删除空文件夹
				if (((CButton *)GetDlgItem(IDC_DELETEEMPDIR))->GetCheck() &&
					PathIsDirectoryEmpty(sSearchPath))
				{
					RemoveDirectory(sSearchPath);
					continue;
				}

				//如果是文件夹，先判断是否包含需要匹配的内容；
				//递归，继续往下找 
				char *cDirectoryName, *cOldDirectoryName;
				int len = 0;
				switch (iDealType)
				{
				case 10:
				{
					if (sMatchContent.IsEmpty() || !StrStr(sDirectoryName, sMatchContent)
						|| (sOldDirectoryName == sMatchContent))
					{
						break;
					}
					
					sDirectoryName.Replace(sMatchContent, L"");
					len = WideCharToMultiByte(CP_ACP, 0, sDirectoryName, -1, NULL, 0, NULL, NULL);
					cDirectoryName = new char[len + 1];
					WideCharToMultiByte(CP_ACP, 0, sDirectoryName, -1, cDirectoryName, len, NULL, NULL);

					len = WideCharToMultiByte(CP_ACP, 0, sOldDirectoryName, -1, NULL, 0, NULL, NULL);
					char *cOldDirectoryName = new char[len + 1];
					WideCharToMultiByte(CP_ACP, 0, sOldDirectoryName, -1, cOldDirectoryName, len, NULL, NULL);

					rename(cOldDirectoryName, cDirectoryName);
					bChangeed = true;
					break;
				}
				case 11:
				{
					sDirectoryName = sDirectoryName + sMatchContent;

					len = WideCharToMultiByte(CP_ACP, 0, sDirectoryName, -1, NULL, 0, NULL, NULL);
					cDirectoryName = new char[len + 1];
					WideCharToMultiByte(CP_ACP, 0, sDirectoryName, -1, cDirectoryName, len, NULL, NULL);

					len = WideCharToMultiByte(CP_ACP, 0, sOldDirectoryName, -1, NULL, 0, NULL, NULL);
					cOldDirectoryName = new char[len + 1];
					WideCharToMultiByte(CP_ACP, 0, sOldDirectoryName, -1, cOldDirectoryName, len, NULL, NULL);

					rename(cOldDirectoryName, cDirectoryName);
					bChangeed = true;
					break;
				}
				default:
					break;
				}
				if (sMatchContent.IsEmpty() || StrStr(sDirectoryName, sMatchContent))
				{
					m_FilenameShow.ReplaceSel(sOldDirectoryName + "\r\n");    //自动换行
				}
				if (bChangeed)
				{
					m_FilenameShow.ReplaceSel(L"-> " + sDirectoryName + "\r\n");    //自动换行
				}

				FindFiles(sDirectoryName, sMatchContent, iDealType);
			}
			else
			{
				sFileName = m_find.GetFilePath();
				sOldFileName = sFileName;
				int i;
				CString sSuffixPre = NULL, sSuffixAfter = NULL;
				
				switch (iDealType)
				{
				case 10:
				{
					if (sMatchContent.IsEmpty() || !StrStr(sOldFileName, sMatchContent))
					{
						break;
					}
					i = sOldFileName.GetLength() - sOldFileName.ReverseFind('.');
					sSuffixPre = sOldFileName.Right(i);

					CString stmpPath = sFileName.TrimRight(m_find.GetFileName());
					sFileName = m_find.GetFileTitle();
					sFileName.Replace(sMatchContent, L"");

					sFileName = stmpPath + sFileName + sSuffixPre;
					CFile::Rename(sOldFileName, sFileName);

					bChangeed = true;
					break;
				}
				case 11:
				{
					if (sMatchContent.IsEmpty())
					{
						break;
					}
					i = sOldFileName.GetLength() - sOldFileName.ReverseFind('.');
					sSuffixPre = sOldFileName.Right(i);
					sFileName.Replace(sSuffixPre, L"");
					sFileName = sFileName + sMatchContent + sSuffixPre;
					CFile::Rename(sOldFileName, sFileName);

					bChangeed = true;
					break;
				}
				default:
					break;
				}
				if (sMatchContent.IsEmpty() || StrStr(sOldFileName, sMatchContent))
				{
					m_FilenameShow.ReplaceSel(sOldFileName + "\r\n");    //自动换行
				}
				if (bChangeed)
				{
					m_FilenameShow.ReplaceSel(L"-> " + sFileName + "\r\n");    //自动换行
				}
			}
		}
	}
	
	m_find.Close();
	
	return true;
}