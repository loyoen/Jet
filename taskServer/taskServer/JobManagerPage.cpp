// JobManagerPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "JobManagerPage.h"


// CJobManagerPage �Ի���

IMPLEMENT_DYNAMIC(CJobManagerPage, CDialogResize)
CJobManagerPage::CJobManagerPage(CWnd* pParent /*=NULL*/)
	: CDialogResize(CJobManagerPage::IDD, pParent)
{
	m_nCurrentJobGroupID	=-1;
	m_nCurrentJobID		=-1;
}

CJobManagerPage::~CJobManagerPage()
{
}

void CJobManagerPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogResize::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_JOBGROUP, m_lstJobGroup);
	DDX_Control(pDX, IDC_LIST_JOB, m_lstJob);
	DDX_Control(pDX, IDC_LIST_CUTJOB, m_lstSubJob);
}


BEGIN_MESSAGE_MAP(CJobManagerPage, CDialogResize)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_JOBGROUP,OnNMClickListJobgroup)
	ON_NOTIFY(NM_CLICK, IDC_LIST_JOB,OnNMClickListJob)	
	ON_NOTIFY(NM_CLICK, IDC_LIST_CUTJOB,OnNMClickListCutJob)
END_MESSAGE_MAP()


BEGIN_DLGRESIZE_MAP(CJobManagerPage)
	DLGRESIZE_CONTROL(IDC_LIST_JOBGROUP, DLSZ_REPAINT)
	DLGRESIZE_CONTROL(IDC_LIST_JOB,DLSZ_REPAINT)
	DLGRESIZE_CONTROL(IDC_LIST_CUTJOB, DLSZ_REPAINT)
END_DLGRESIZE_MAP()
// CJobManagerPage ��Ϣ�������
/********************************************************************/
/*																	*/
/* Function name : OnSize											*/		
/* Description   : Handle WM_SIZE message							*/
/*																	*/
/********************************************************************/
void CJobManagerPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialogResize::OnSize(nType, cx, cy);
	
	CRect rect;
	GetClientRect(rect);
	int nHeight=rect.Height();

	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST_JOBGROUP)))
	{
		rect.bottom=rect.top +rect.Height()/3;
		m_lstJobGroup.MoveWindow(rect);

		m_lstJobGroup.SetColumnWidth(0, rect.Width()/4-2);
		m_lstJobGroup.SetColumnWidth(1, rect.Width()/4);
		m_lstJobGroup.SetColumnWidth(2, rect.Width()/4);
		m_lstJobGroup.SetColumnWidth(3, rect.Width()/4-2);

	}	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST_JOB)))
	{
        rect.top=rect.bottom +3;
		rect.bottom=rect.top +nHeight/3;
		m_lstJob.MoveWindow(rect);

		m_lstJob.SetColumnWidth(0, rect.Width()/4-2);
		m_lstJob.SetColumnWidth(1, rect.Width()/4);
		m_lstJob.SetColumnWidth(2, rect.Width()/4);

	}	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST_CUTJOB)))
	{
        rect.top=rect.bottom +3;
		rect.bottom=rect.top +nHeight/3;
		m_lstSubJob.MoveWindow(rect);

		m_lstSubJob.SetColumnWidth(0, rect.Width()/6-2);
		m_lstSubJob.SetColumnWidth(1, rect.Width()/5);
		m_lstSubJob.SetColumnWidth(2, rect.Width()/5);
		m_lstSubJob.SetColumnWidth(3, rect.Width()/5);
	    m_lstSubJob.SetColumnWidth(4, rect.Width()/5-2);

	}	
	this->Invalidate();
}

BOOL CJobManagerPage::OnInitDialog()
{
	CDialogResize::OnInitDialog();
  	InitResizing(FALSE, FALSE, WS_CLIPCHILDREN);
	UpdateDialogControls(this, FALSE);
	//�����б���
/*	m_lstJobGroup.InsertColumn(0, "����ID");
	m_lstJobGroup.InsertColumn(1, "״̬");
	m_lstJobGroup.InsertColumn(2, "������");
	m_lstJobGroup.InsertColumn(3, "�Ѵ�ӡ������");

	//Job�б���
	m_lstJob.InsertColumn(0,"����ID");
	m_lstJob.InsertColumn(1,"״̬");
	m_lstJob.InsertColumn(2,"δ��ӡ������");

	//CutJob�б���
	m_lstSubJob.InsertColumn(0,"��ҵID");
	m_lstSubJob.InsertColumn(1,"״̬");
	m_lstSubJob.InsertColumn(2,"��ӡ����1");
	m_lstSubJob.InsertColumn(3,"��ӡ����2");
	m_lstSubJob.InsertColumn(4,"��ӡ����3");
*/
	DWORD dwStyle = m_lstJobGroup.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES ;
	m_lstJobGroup.SetExtendedStyle(dwStyle);   
	m_lstJob.SetExtendedStyle(dwStyle);   
	m_lstSubJob.SetExtendedStyle(dwStyle);   

	return TRUE;
}

void CJobManagerPage::AddJobGroup(int nAction,  int nJobGroupID,int nStatus,
									 int nJobCnt,int nSubmitCnt)
{
/*	if(JOBGROUP_ACTION_ADD==nAction)
	{
		CString str;
		str.Format("%d", nJobGroupID);

		LVFINDINFO info;

		info.flags = LVFI_STRING;
		info.psz = (LPCTSTR)str;

		int nIndex = m_lstJobGroup.FindItem(&info);
		if (nIndex == -1)
		{ 
			nIndex = m_lstJobGroup.InsertItem(0, str);

			//״̬
			if(nStatus==JOBGROUP_STATUS_FRESH_NEW)
				str="��δ��ӡ";
			if(nStatus==JOBGROUP_STATUS_IN_PROGRESS)
				str="���ڴ�ӡ";
			if(nStatus==JOBGROUP_STATUS_FINISHED)
				str="���";
			m_lstJobGroup.SetItemText(nIndex,1,str);

			//��������
			str.Format("%d",nJobCnt);
			m_lstJobGroup.SetItemText(nIndex,2,str);

			//�Ѵ�ӡ������
			str.Format("%d",nSubmitCnt);
			m_lstJobGroup.SetItemText(nIndex,3,str);
		}
	}
	if(JOBGROUP_ACTION_DEL==nAction)
	{
		CString str;
		str.Format("%d", nJobGroupID);

		LVFINDINFO info;

		info.flags = LVFI_PARTIAL|LVFI_STRING;
		info.psz = (LPCTSTR)str;

		int nIndex = m_lstJobGroup.FindItem(&info);
		if (nIndex == -1)
		{
			return;
		}
        m_lstJobGroup.DeleteItem(nIndex);
	}
	if(JOBGROUP_ACTION_UPDATE==nAction)
	{
		CString str;
		str.Format("%d", nJobGroupID);

		LVFINDINFO info;

		info.flags = LVFI_PARTIAL|LVFI_STRING;
		info.psz = (LPCTSTR)str;

		int nIndex = m_lstJobGroup.FindItem(&info);
		if (nIndex == -1)
		{
			return;
		}
		//״̬
		if(nStatus==JOBGROUP_STATUS_FRESH_NEW)
			str="��δ��ӡ";
		if(nStatus==JOBGROUP_STATUS_IN_PROGRESS)
			str="���ڴ�ӡ";
		if(nStatus==JOBGROUP_STATUS_FINISHED)
			str="���";
		m_lstJobGroup.SetItemText(nIndex,1,str);

		//�Ѵ�ӡ������
		str.Format("%d",nSubmitCnt);
		m_lstJobGroup.SetItemText(nIndex,3,str);
	}
	return;*/
}

void CJobManagerPage::OnNMClickListJobgroup(NMHDR *pNMHDR, LRESULT *pResult)
{
 //   LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
	//
	////Get the Jobgroup ID
	//int iItem=lpnmitem->iItem;
	//CString str;
 //   m_lstJobGroup.GetItemText(iItem,0,str.GetBuffer(20),20);
	//str.ReleaseBuffer();
	//int nJobGroupID=(int)atoi(str);
	//m_nCurrentJobGroupID=nJobGroupID;

 //   m_lstJob.DeleteAllItems(); 
	//CMessageManager::PostMessageToNetWork(WM_GUI_NETWORK_GET_JOB_IN_JOBGROUP,(int)nJobGroupID,(char*)NULL); 
	//*pResult = 0;
}


//==========================================
//
//      Get CutJob state
//
//==========================================
void CJobManagerPage::OnNMClickListJob(NMHDR *pNMHDR, LRESULT *pResult)
{
//    LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
//	
//	//Get the Jobgroup ID
//	int iItem=lpnmitem->iItem;
//	CString str;
//    m_lstJob.GetItemText(iItem,0,str.GetBuffer(20),20);
//	str.ReleaseBuffer();
//	int nJobID=(int)atoi(str);
//	m_nCurrentJobID=nJobID;
//
//	m_lstSubJob.DeleteAllItems(); 
////	CMessageManager::PostMessageToNetWork(WM_GUI_NETWORK_GET_CUTJOB_IN_JOB,(int)m_nCurrentJobGroupID,(char*)m_nCurrentJobID); 
//	*pResult = 0;
}

void CJobManagerPage::OnNMClickListCutJob(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;

	////Get the Jobgroup ID
	//int iItem=lpnmitem->iItem;
	//CString str;
	//m_lstSubJob.GetItemText(iItem,1,str.GetBuffer(20),20);
	//str.ReleaseBuffer();
	//BYTE nState=(BYTE)(int)atoi(str);


	//if(m_lstSubJob.GetItemCount()>0)
	//{
	//	CString str;
	//	str="-  ��ʾ��ʼ״̬���ȴ�����\nX  ��ʾ�Ѿ��ύ����һ������\nY  ��ʾ�Ѿ��ɹ����͵��˴�ӡ����\nZ  ��ʾ�Ѿ��ɹ��ύ\n";
	//	if(nState&0x80)
	//		str+="�ڴ滹û�ͷ�\n";
	//	::MessageBox(NULL,str,"״̬���ŵ�����",MB_OK|MB_ICONINFORMATION);
	//}
}

void CJobManagerPage::ResetAllList()
{
	m_lstSubJob.DeleteAllItems();
	m_lstJob.DeleteAllItems();
	m_lstJobGroup.DeleteAllItems();
}