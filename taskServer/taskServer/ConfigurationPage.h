#pragma once
#include "DialogResize.h"

#include "Resource.h"

// CConfigurationPage �Ի���

class CConfigurationPage : public CDialogResize
{
	DECLARE_DYNAMIC(CConfigurationPage)

public:
	CConfigurationPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CConfigurationPage();

public:
	//{{AFX_DATA(CConfigurationPage)
	enum { IDD = IDD_PROPPAGE_CONFIGURATION };
	UINT m_nPort;
	CString m_strWorkProgressFilePath;
	UINT m_nTaskID;
	UINT m_nHeartBeatInterval;
	UINT m_nClientTimeout;
	UINT m_nJobGroupExpected;
	UINT m_nJobGroupMax;
    //}}AFX_DATA
// �Ի�������
protected:
	BOOL m_bModified;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	DECLARE_DLGRESIZE_MAP;
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnApply();
	afx_msg void OnBnClickedButtonBrowse();
	afx_msg void OnSomethingChanged();
	afx_msg void OnUpdateApply(CCmdUI* pCmdUI);
};
