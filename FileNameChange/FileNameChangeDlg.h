
// FileNameChangeDlg.h: 头文件
//

#pragma once


// CFileNameChangeDlg 对话框
class CFileNameChangeDlg : public CDialogEx
{
// 构造
public:
	CFileNameChangeDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILENAMECHANGE_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_ComboBox;
	afx_msg void OnBnClickedFilesearch();
	//afx_msg void OnBnClickedDirectorychange();
	afx_msg void OnCbnSelchangeDealtype();
	CEdit m_FilenameShow;

	int FindFiles(CString sPath, const CString sMatchContent, const int iDealType);
};
