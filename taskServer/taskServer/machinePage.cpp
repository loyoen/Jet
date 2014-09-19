// OnlineUserPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MachinePage.h"


// CMachinePage �Ի���

IMPLEMENT_DYNAMIC(CMachinePage, CDialog)
CMachinePage::CMachinePage(CWnd* pParent /*=NULL*/)
	: CDialog(CMachinePage::IDD, pParent)
{
}

CMachinePage::~CMachinePage()
{
}

void CMachinePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_Machines);
}


BEGIN_MESSAGE_MAP(CMachinePage, CDialog)
   ON_WM_SIZE()
END_MESSAGE_MAP()


// CMachinePage ��Ϣ�������
/********************************************************************/
/*																	*/
/* Function name : OnSize											*/		
/* Description   : Handle WM_SIZE message							*/
/*																	*/
/********************************************************************/
void CMachinePage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST1)))
	{
		CRect rect;
		GetClientRect(rect);
		m_Machines.MoveWindow(rect);


		m_Machines.SetColumnWidth(0, rect.Width()/7-2);
		m_Machines.SetColumnWidth(1, rect.Width()/7-2);
		m_Machines.SetColumnWidth(2, rect.Width()/7-2);
		m_Machines.SetColumnWidth(3, rect.Width()/7-2);
		m_Machines.SetColumnWidth(4, rect.Width()/7-2);
		m_Machines.SetColumnWidth(5, rect.Width()/7-2);
		m_Machines.SetColumnWidth(6, rect.Width()/7-2);
	}	
}
/********************************************************************/
/*																	*/
/* Function name : OnInitDialog										*/		
/* Description   : Called by the framework in response to the		*/
/*				   WM_INITDIALOG message.							*/
/*																	*/
/********************************************************************/
BOOL CMachinePage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	m_Machines.InsertColumn(0, _T("ID"));		
    m_Machines.InsertColumn(1, _T("���"));	
	m_Machines.InsertColumn(2, _T("ά����Ա"));
	m_Machines.InsertColumn(3, _T("�������"));
	m_Machines.InsertColumn(4, _T("ά��ʱ��"));	
	m_Machines.InsertColumn(5, _T("��ǰ��ҵ��"));
	m_Machines.InsertColumn(6, _T("�ۼ���ҵ��"));
	
	DWORD dwStyle = m_Machines.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES ;
    m_Machines.SetExtendedStyle(dwStyle);
    

	return TRUE;
}

BOOL CMachinePage::AddMachine(
		int         nMachineID,
		const char* strAccount,
		const char* strName,
		//��ӡ��Ϣ
		int         nSubmitToday,
		int         nSubmitTotal,
		//��ǰ��ӡ��ҵ
		unsigned char nCurrentQuestionNum
		)
{
	CString strMachineID;
//	strMachineID.Format("%d", nMachineID);

	LVFINDINFO info;
	
	info.flags = LVFI_STRING;
	info.psz = (LPCTSTR)strMachineID;

	int nIndex = m_Machines.FindItem(&info);
	if (nIndex == -1)
	{
		nIndex = m_Machines.InsertItem(0, strMachineID);
	}
	//����
//	m_Machines.SetItemText(nIndex, 1,strName);
	//�˺�
//	m_Machines.SetItemText(nIndex, 2,strAccount);
	//�������
	CString temp;
//	temp.Format("%d",nCurrentQuestionNum);
    m_Machines.SetItemText(nIndex, 3,temp);
    //��½ʱ��
//	m_Machines.SetItemText(nIndex, 4,CTime::GetCurrentTime().Format("%H:%M:%S"));
	//�����ӡ��������
	temp.Empty();
//	temp.Format("%d",nSubmitToday);
	m_Machines.SetItemText(nIndex, 5,temp);
    //�ۼƴ�ӡ��������
	temp.Empty();
//	temp.Format("%d",nSubmitTotal);
	m_Machines.SetItemText(nIndex, 6,temp);

	return FALSE;
}

BOOL  CMachinePage::RemoveMachine(
		int         nMachineID
		)
{
	//CString strMachineID;
	//strMachineID.Format("%d", nMachineID);

	//LVFINDINFO info;
	//
	//info.flags = LVFI_STRING;
	//info.psz = (LPCTSTR)strMachineID;

	//int nIndex = m_Machines.FindItem(&info);
	//if (nIndex == -1)
	//{
	//	return FALSE;
	//}
	//else
	//{
	//	m_Machines.DeleteItem(nIndex);
	//	return TRUE;
	//}
	return FALSE;
}