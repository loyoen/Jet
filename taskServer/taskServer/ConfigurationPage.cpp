// ConfigurationPage.cpp : 实现文件
//

#include "stdafx.h"
#include "ConfigurationPage.h"
#include ".\configurationpage.h"
//#include "CIni.h"

// CConfigurationPage 对话框

IMPLEMENT_DYNAMIC(CConfigurationPage, CDialogResize)
CConfigurationPage::CConfigurationPage(CWnd* pParent /*=NULL*/)
	: CDialogResize(CConfigurationPage::IDD, pParent)
	, m_nPort(0)
	, m_strWorkProgressFilePath(_T(""))
	, m_nTaskID(0)
	, m_nHeartBeatInterval(0)
	, m_nClientTimeout(0)
	, m_nJobGroupMax(0)
	, m_nJobGroupExpected(0)
{
   	m_bModified = FALSE;
}

CConfigurationPage::~CConfigurationPage()
{
}

void CConfigurationPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_PORT, m_nPort);
	DDX_Text(pDX, IDC_EDIT_PATH, m_strWorkProgressFilePath);
	DDX_Text(pDX, IDC_EDIT_TaskID, m_nTaskID);
	DDX_Text(pDX, IDC_EDIT_HEART_BEAT_INTERVAL, m_nHeartBeatInterval);
	DDX_Text(pDX, IDC_EDIT_CLIENT_ACK_TIMEOUT, m_nClientTimeout);
	DDX_Text(pDX, IDC_EDIT_JOBGROUP_MAX, m_nJobGroupMax);
	DDX_Text(pDX, IDC_EDIT_JOBGROUP_EXPECTED, m_nJobGroupExpected);
}


BEGIN_MESSAGE_MAP(CConfigurationPage, CDialogResize)
    ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_APPLY, OnApply)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBnClickedButtonBrowse)
    ON_UPDATE_COMMAND_UI(IDC_BTN_APPLY, OnUpdateApply)
	ON_EN_CHANGE(IDC_EDIT_PORT, OnSomethingChanged)
	ON_EN_CHANGE(IDC_EDIT_PATH, OnSomethingChanged)
	ON_EN_CHANGE(IDC_EDIT_TaskID, OnSomethingChanged)
	ON_EN_CHANGE(IDC_EDIT_HEART_BEAT_INTERVAL, OnSomethingChanged)
	ON_EN_CHANGE(IDC_EDIT_CLIENT_ACK_TIMEOUT, OnSomethingChanged)
	ON_EN_CHANGE(IDC_EDIT_JOBGROUP_MAX, OnSomethingChanged)
	ON_EN_CHANGE(IDC_EDIT_JOBGROUP_EXPECTED, OnSomethingChanged)
END_MESSAGE_MAP()

BEGIN_DLGRESIZE_MAP(CConfigurationPage)
	//DLGRESIZE_CONTROL(IDC_EDIT_PORT, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_EDIT_PATH, DLSZ_SIZE_X )
    DLGRESIZE_CONTROL(IDC_BUTTON_BROWSE,DLSZ_MOVE_X )
	//DLGRESIZE_CONTROL(IDC_EDIT_TaskID, DLSZ_SIZE_X)
	//DLGRESIZE_CONTROL(IDC_EDIT_HEART_BEAT_INTERVAL, DLSZ_SIZE_X)
	//DLGRESIZE_CONTROL(IDC_EDIT_CLIENT_ACK_TIMEOUT, DLSZ_SIZE_X)
	DLGRESIZE_CONTROL(IDC_BTN_APPLY, DLSZ_MOVE_X | DLSZ_MOVE_Y)
END_DLGRESIZE_MAP()
// CConfigurationPage 消息处理程序

BOOL CConfigurationPage::OnInitDialog()
{
	CDialogResize::OnInitDialog();
  
	InitResizing(FALSE, FALSE, WS_CLIPCHILDREN);
	UpdateDialogControls(this, FALSE);

	//设置配置文件
	m_nPort					= AfxGetApp()->GetProfileInt( _T("Configuration"), _T("Port"), 8721 );
	m_nTaskID			= AfxGetApp()->GetProfileInt( _T("Configuration"), _T("TaskID"), 1 );
	m_nHeartBeatInterval	= AfxGetApp()->GetProfileInt( _T("Configuration"), _T("HeartBeatInterval"), 2 );
	m_nClientTimeout		= AfxGetApp()->GetProfileInt( _T("Configuration"), _T("ClientHeartBeatAckTimeout"), 2 );
	m_nJobGroupExpected	= AfxGetApp()->GetProfileInt( _T("Configuration"), _T("JobGroupExpected"), 10 );
	m_nJobGroupMax			= AfxGetApp()->GetProfileInt( _T("Configuration"), _T("JobGroupMax"), 20 );
	m_strWorkProgressFilePath	= AfxGetApp()->GetProfileString( _T("Configuration"), _T("WorkProgressFilePath"), _T("eMarkWorkLoad.ini") );

 //   CString str;
	//GetCurrentDirectory(MAX_PATH,str.GetBuffer(MAX_PATH));
	//str.ReleaseBuffer();
	//str+="\\eMarkConfig.ini";

 //   CIni ini(str.GetBuffer(MAX_PATH));
	//str.ReleaseBuffer();
	//m_nPort=ini.ReadInt("Configuration","Port");

 //   m_nTaskID=ini.ReadInt("Configuration","TaskID");
 //   m_nHeartBeatInterval=ini.ReadInt("Configuration","HeartBeatInterval");
 //   m_nClientTimeout=ini.ReadInt("Configuration","ClientHeartBeatAckTimeout");
 //   m_nJobGroupExpected=ini.ReadInt("Configuration","JobGroupExpected");
	//m_nJobGroupMax=ini.ReadInt("Configuration","JobGroupMax");
	////Take care when read text from ini file using this class!!!
	//char *pRet=ini.ReadText("Configuration","WorkProgressFilePath"); 
 //   m_strWorkProgressFilePath.Format("%s",pRet);
	//if(pRet!=NULL)
	//    delete[] pRet;
	//ini.Close();

	UpdateData(FALSE);

	return TRUE;  
}

void CConfigurationPage::OnApply()
{
    UpdateData(TRUE);
    m_bModified = FALSE;
	UpdateDialogControls(this, FALSE);	

	//写配置文件
	//CString str;
	//GetCurrentDirectory(MAX_PATH,str.GetBuffer(MAX_PATH));
	//str.ReleaseBuffer();
	//str+="\\eMarkConfig.ini";

	//CIni ini(str.GetBuffer(MAX_PATH));
	//str.ReleaseBuffer();

	AfxGetApp()->WriteProfileInt( _T("Configuration"), _T("Port"), m_nPort );
	AfxGetApp()->WriteProfileInt( _T("Configuration"), _T("TaskID"), m_nTaskID );
	AfxGetApp()->WriteProfileInt( _T("Configuration"), _T("HeartBeatInterval"), m_nHeartBeatInterval );
	AfxGetApp()->WriteProfileInt( _T("Configuration"), _T("ClientHeartBeatAckTimeout"), m_nClientTimeout );
	AfxGetApp()->WriteProfileInt( _T("Configuration"), _T("JobGroupExpected"), m_nJobGroupExpected );
	AfxGetApp()->WriteProfileInt( _T("Configuration"), _T("JobGroupMax"), m_nJobGroupMax );
	AfxGetApp()->WriteProfileString( _T("Configuration"), _T("WorkProgressFilePath"), m_strWorkProgressFilePath );
	//ini.Write("Configuration","Port",m_nPort);
	//ini.Write("Configuration","TaskID",m_nTaskID);
	//ini.Write("Configuration","HeartBeatInterval",m_nHeartBeatInterval);
	//ini.Write("Configuration","ClientHeartBeatAckTimeout",m_nClientTimeout);
	//ini.Write("Configuration","JobGroupExpected",m_nJobGroupExpected);
	//ini.Write("Configuration","JobGroupMax",m_nJobGroupMax);
	//ini.Write("Configuration","WorkProgressFilePath",m_strWorkProgressFilePath.GetBuffer(MAX_PATH));
	//m_strWorkProgressFilePath.ReleaseBuffer();
	//ini.Save();
	//ini.Close();
}

void CConfigurationPage::OnBnClickedButtonBrowse()
{
     CFileDialog dlg(TRUE, _T("ini"),NULL,OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
		             _T("Configuration Files (*.ini)|*.ini|All Files (*.*)|*.*||"));
	 dlg.DoModal();
	 m_strWorkProgressFilePath=dlg.GetPathName();
	 UpdateData(FALSE);
	 OnSomethingChanged();
	 
}
/********************************************************************/
/*																	*/
/* Function name : OnSomethingChanged								*/
/* Description   : Something has changed...							*/
/*																	*/
/********************************************************************/
void CConfigurationPage::OnSomethingChanged() 
{
	m_bModified = TRUE;	
	UpdateDialogControls(this, FALSE);	
}

/********************************************************************/
/*																	*/
/* Function name : OnUpdateApply									*/		
/* Description   : Update button state.								*/	
/*																	*/
/********************************************************************/
void CConfigurationPage::OnUpdateApply(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_bModified);			
}

/********************************************************************/
/*																	*/
/* Function name : OnDestroy										*/		
/* Description   : Called by the framework in response to the		*/
/*				   WM_DESTROY message.								*/
/*																	*/
/********************************************************************/
void CConfigurationPage::OnDestroy() 
{
	UpdateData(TRUE);

	//写配置文件
/*	CString str;
	GetCurrentDirectory(MAX_PATH,str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	str+="\\eMarkConfig.ini";

    CIni ini(str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();

	ini.Write("Configuration","Port",m_nPort);
	ini.Write("Configuration","TaskID",m_nTaskID);
	ini.Write("Configuration","HeartBeatInterval",m_nHeartBeatInterval);
	ini.Write("Configuration","ClientHeartBeatAckTimeout",m_nClientTimeout);
	ini.Write("Configuration","JobGroupExpected",m_nJobGroupExpected);
	ini.Write("Configuration","JobGroupMax",m_nJobGroupMax);
	ini.Write("Configuration","WorkProgressFilePath",m_strWorkProgressFilePath.GetBuffer(MAX_PATH));
	m_strWorkProgressFilePath.ReleaseBuffer();
    ini.Save();
	ini.Close();
	CDialogResize::OnDestroy();
*/
}