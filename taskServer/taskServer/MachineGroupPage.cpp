// MachineGroupPage.cpp : 实现文件
//

#include "stdafx.h"
#include "MachineGroupPage.h"
#include ".\MachineGrouppage.h"


// CMachineGroupPage 对话框

IMPLEMENT_DYNAMIC(CMachineGroupPage, CDialog)
CMachineGroupPage::CMachineGroupPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMachineGroupPage::IDD, pParent)
{
	m_nCurrentMachineGroupID	= -1;
	m_nCurrentMachineID		= -1;
}

CMachineGroupPage::~CMachineGroupPage()
{
}

void CMachineGroupPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_USER_GROUP, m_MachineGroup);
	DDX_Control(pDX,IDC_LIST_GROUP_MEMBER,m_lstGroupMember);
}


BEGIN_MESSAGE_MAP(CMachineGroupPage, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_USER_GROUP,OnNMClickListGroup)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_GROUP_MEMBER, OnNMRclickListGroupMember)
END_MESSAGE_MAP()


// CMachineGroupPage 消息处理程序
/********************************************************************/
/*																	*/
/* Function name : OnSize											*/		
/* Description   : Handle WM_SIZE message							*/
/*																	*/
/********************************************************************/
void CMachineGroupPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST_USER_GROUP)))
	{
		CRect rect;
		GetClientRect(rect);
		rect.bottom =rect.top +rect.Height()/2-5;
		m_MachineGroup.MoveWindow(rect);


		m_MachineGroup.SetColumnWidth(0, rect.Width()/12-2);
		m_MachineGroup.SetColumnWidth(1, rect.Width()/6-2);
		m_MachineGroup.SetColumnWidth(2, rect.Width()/12-2);
		m_MachineGroup.SetColumnWidth(3, rect.Width()/12-2);
		m_MachineGroup.SetColumnWidth(4, rect.Width()/12-2);
		m_MachineGroup.SetColumnWidth(5, rect.Width()/12-2);
	}
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST_GROUP_MEMBER)))
	{
		CRect rect;
		GetClientRect(rect);
		rect.top =rect.bottom -rect.Height()/2+5;
		m_lstGroupMember.MoveWindow(rect);


		m_lstGroupMember.SetColumnWidth(0, rect.Width()/8-2);
		m_lstGroupMember.SetColumnWidth(1, rect.Width()/8);
		m_lstGroupMember.SetColumnWidth(2, rect.Width()/8-2);
		m_lstGroupMember.SetColumnWidth(3, rect.Width()/8-2);
		m_lstGroupMember.SetColumnWidth(4, rect.Width()/8-2);
		m_lstGroupMember.SetColumnWidth(5, rect.Width()/8-2);
		m_lstGroupMember.SetColumnWidth(6, rect.Width()/8-2);
		m_lstGroupMember.SetColumnWidth(7, rect.Width()/8-2);
//		m_lstGroupMember.SetColumnWidth(2, rect.Width()/8-2);
	}
	this->Invalidate();
}

/********************************************************************/
/*																	*/
/* Function name : OnInitDialog										*/		
/* Description   : Called by the framework in response to the		*/
/*				   WM_INITDIALOG message.							*/
/*																	*/
/********************************************************************/
BOOL CMachineGroupPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

/*    m_MachineGroup.InsertColumn(0, "ID");	
    m_MachineGroup.InsertColumn(1, "任务");	
	m_MachineGroup.InsertColumn(2, "颜色");
	m_MachineGroup.InsertColumn(3, "机器数");
    m_MachineGroup.InsertColumn(4, "完成数");
	m_MachineGroup.InsertColumn(5, "总数");
	
	m_lstGroupMember.InsertColumn(0,"机器ID");
	m_lstGroupMember.InsertColumn(1,"名称");
	m_lstGroupMember.InsertColumn(2,"优先级");
	m_lstGroupMember.InsertColumn(3,"状态");
	m_lstGroupMember.InsertColumn(4,"完成数");
	m_lstGroupMember.InsertColumn(5,"有效数");
	m_lstGroupMember.InsertColumn(6,"剩余");

	DWORD dwStyle = m_MachineGroup.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES ;
    m_MachineGroup.SetExtendedStyle(dwStyle);   
    m_lstGroupMember.SetExtendedStyle(dwStyle);
*/
//	m_menu.CreateMenu( );
//	m_menu.LoadMenu( IDR_POPUP_MENU );
//	m_menu.LoadMenu( IDR_MAINMENU );

	return TRUE;
}

void CMachineGroupPage::OnMachineGroupAction(int nAction,int nQuestionNum,
									   float fScore,int nCntOfMembers,
				                       int nSubmitPapersInTotal, int nTotalCnt, char strTitle[13])
{
	//CString str;
	//str.Format("%d", nQuestionNum);

	//LVFINDINFO info;

	//info.flags =LVFI_STRING;
	//info.psz = (LPCTSTR)str;

	//int nIndex = m_MachineGroup.FindItem(&info);
	//if (nIndex == -1)
	//{
	//	nIndex	= m_MachineGroup.InsertItem( 0, str );

	//	str.Format("%1.0f",fScore);
	//	m_MachineGroup.SetItemText( nIndex, 2, str );
	//}

	//str.Format("%s", strTitle);
	//m_MachineGroup.SetItemText( nIndex, 1, str );

	////Update info
	//str.Format("%d",nCntOfMembers);
	//m_MachineGroup.SetItemText( nIndex, 3, str );

	//str.Format("%d",nSubmitPapersInTotal);
	//m_MachineGroup.SetItemText( nIndex, 4, str);

	//str.Format("%d",nTotalCnt);
	//m_MachineGroup.SetItemText( nIndex, 5, str);

/*	if(MACHINEGROUP_ACTION_ADD==nAction)
	{
		CString str;
		str.Format("%d",nQuestionNum);
		int nIndex=m_MachineGroup.InsertItem(0,str);

		str.Format("%1.1f",fScore);
		m_MachineGroup.SetItemText(nIndex,1,str);

		str.Format("%d",nCntOfMembers);
		m_MachineGroup.SetItemText(nIndex,2,str);

		str.Format("%d",nSubmitPapersInTotal);
		m_MachineGroup.SetItemText(nIndex,3,str);
	}
	if(MACHINEGROUP_ACTION_DEL==nAction)
	{
		CString str;
		str.Format("%d", nQuestionNum);

		LVFINDINFO info;

		info.flags =LVFI_STRING;
		info.psz = (LPCTSTR)str;

		int nIndex = m_MachineGroup.FindItem(&info);
		if (nIndex == -1)
		{
			return;
		}
		m_MachineGroup.DeleteItem(nIndex); 
	}
	if(MACHINEGROUP_ACTION_UPDATE==nAction)
	{
		CString str;
		str.Format("%d", nQuestionNum);

		LVFINDINFO info;

		info.flags =LVFI_STRING;
		info.psz = (LPCTSTR)str;

		int nIndex = m_MachineGroup.FindItem(&info);
		if (nIndex == -1)
		{
			return;
		}
		//Update info
		str.Format("%d",nCntOfMembers);
		m_MachineGroup.SetItemText(nIndex,2,str);

		str.Format("%d",nSubmitPapersInTotal);
		m_MachineGroup.SetItemText(nIndex,3,str);
	}
	return;
*/
}


void CMachineGroupPage::ResetAllList()
{
	m_MachineGroup.DeleteAllItems();
	m_lstGroupMember.DeleteAllItems();
}

//============================================
//
//     Browse the Group memebers of a single group
//
//============================================
void CMachineGroupPage::OnNMClickListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	//LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
	//
	//m_lstGroupMember.DeleteAllItems(); 
	//
	////Get the Jobgroup ID
	//int		iItem	= lpnmitem->iItem;
	//
	//CString str;
	//m_MachineGroup.GetItemText(iItem,0,str.GetBuffer(20),20);
	//int		nGroupID= (int)atoi(str);
	//m_nCurrentMachineGroupID=nGroupID;

	//m_pParentDlg->UpdateChannelState( nGroupID );

	////m_lstGroupMember.DeleteAllItems(); 
	////CMessageManager::PostMessageToNetWork(WM_GUI_NETWORK_GET_GROUP_MEMBERS,nGroupID,(char*)NULL);
	//*pResult = 0;
}

void CMachineGroupPage::OnNMRclickListGroupMember(NMHDR *pNMHDR, LRESULT *pResult)
{
	//CString	str;
	//CRect	r;

	//LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
	//
	//// Get the corresponding machineID
	//int		iItem	= lpnmitem->iItem;
	//m_lstGroupMember.GetItemText(iItem,0,str.GetBuffer(20),20);
	//m_nCurrentMachineID	= (int)atoi(str);

	//m_lstGroupMember.GetWindowRect( &r );

	//int iSel	= 0;
	//POSITION	pos;
	//pos	= m_lstGroupMember.GetFirstSelectedItemPosition();
	//while ( NULL != pos )
	//	iSel	= m_lstGroupMember.GetNextSelectedItem( pos );

	//int	iY	= r.top + 18 + iSel*13;

	//CMenu* menu_bar		= AfxGetMainWnd()->GetMenu();
	//CMenu* action_menu	= menu_bar->GetSubMenu(2);    
	//ASSERT(action_menu);

	//action_menu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, r.left + 30, 	iY, this);

	//*pResult = 0;
}


BOOL CMachineGroupPage::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	m_pParentDlg		= (CtaskServerDlg*)pParentWnd;

	return CDialog::Create(nIDTemplate, pParentWnd);
}
