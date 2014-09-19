// pageTasks.cpp : 实现文件
//

#include "stdafx.h"
#include "jetManager.h"
#include "pageTasks.h"
#include "GUI/DlgOperator.h"


// CPageTasks 对话框

IMPLEMENT_DYNAMIC(CPageTasks, CDialog)
CPageTasks::CPageTasks(CWnd* pParent /*=NULL*/)
	: CDialog(CPageTasks::IDD, pParent)
{
	m_nCurrentUserGroupID	= -1;
	m_nCurrentMachineID		= -1;
}

CPageTasks::~CPageTasks()
{
}

void CPageTasks::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TASKS, m_lstTasks);
	DDX_Control(pDX, IDC_LIST_JOB_MACHINE,m_lstJobMachine);
}


BEGIN_MESSAGE_MAP(CPageTasks, CDialog)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_LIST_TASKS,OnNMClickListGroup)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_JOB_MACHINE, OnNMRclickListGroupMember)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_TASKS, OnNMRclickListUserGroup)
END_MESSAGE_MAP()


// CPageTasks 消息处理程序
/********************************************************************/
/*																	*/
/* Function name : OnSize											*/		
/* Description   : Handle WM_SIZE message							*/
/*																	*/
/********************************************************************/
void CPageTasks::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST_TASKS)))
	{
		CRect rect;
		GetClientRect(rect);
		//rect.top	+= 10;
		rect.left	+= 10;

		rect.bottom =rect.top +rect.Height()/2-5;
		m_lstTasks.MoveWindow(rect);


		m_lstTasks.SetColumnWidth(0, rect.Width()/12-2);
		m_lstTasks.SetColumnWidth(1, rect.Width()/6-2);
		m_lstTasks.SetColumnWidth(2, rect.Width()/12-2);
		m_lstTasks.SetColumnWidth(3, rect.Width()/12-2);
		m_lstTasks.SetColumnWidth(4, rect.Width()/12-2);
		m_lstTasks.SetColumnWidth(5, rect.Width()/12-2);
	}
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST_JOB_MACHINE)))
	{
		CRect rect;
		GetClientRect(rect);
		//rect.top	+= 10;
		rect.left	+= 10;

		rect.top =rect.bottom -rect.Height()/2+5;
		m_lstJobMachine.MoveWindow(rect);

		m_lstJobMachine.SetColumnWidth(0, rect.Width()/8-2);
		m_lstJobMachine.SetColumnWidth(1, rect.Width()/8);
		m_lstJobMachine.SetColumnWidth(2, rect.Width()/8-2);
		m_lstJobMachine.SetColumnWidth(3, rect.Width()/8-2);
		m_lstJobMachine.SetColumnWidth(4, rect.Width()/8-2);
		m_lstJobMachine.SetColumnWidth(5, rect.Width()/8-2);
		m_lstJobMachine.SetColumnWidth(6, rect.Width()/8-2);
		m_lstJobMachine.SetColumnWidth(7, rect.Width()/8-2);
//		m_lstJobMachine.SetColumnWidth(2, rect.Width()/8-2);
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
BOOL CPageTasks::OnInitDialog() 
{
	CDialog::OnInitDialog();

    m_lstTasks.InsertColumn(0, "ID");	
    m_lstTasks.InsertColumn(1, "任务号");	
	m_lstTasks.InsertColumn(2, "颜色数");
	m_lstTasks.InsertColumn(3, "机器数");
    m_lstTasks.InsertColumn(4, "完成数");
	m_lstTasks.InsertColumn(5, "总数");
	
	m_lstJobMachine.InsertColumn(0,"机器ID");
	m_lstJobMachine.InsertColumn(1,"编号");
	m_lstJobMachine.InsertColumn(2,"名称");
	m_lstJobMachine.InsertColumn(3,"优先级");
	m_lstJobMachine.InsertColumn(4,"状态");
	m_lstJobMachine.InsertColumn(5,"完成数");
	m_lstJobMachine.InsertColumn(6,"正品数");
	m_lstJobMachine.InsertColumn(7,"作业剩余");

	DWORD dwStyle = m_lstTasks.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES ;
    m_lstTasks.SetExtendedStyle(dwStyle);   
    m_lstJobMachine.SetExtendedStyle(dwStyle);

//	m_menu.CreateMenu( );
//	m_menu.LoadMenu( IDR_POPUP_MENU );
//	m_menu.LoadMenu( IDR_MAINMENU );
	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST_TASKS)))
	{
		CRect rect;
		GetClientRect(rect);
		//rect.top	+= 10;
		rect.left	+= 10;

		rect.bottom = rect.top + rect.Height() / 4  - 1;
		m_lstTasks.MoveWindow(rect);

		m_lstTasks.SetColumnWidth(0, rect.Width()/6-2);
		m_lstTasks.SetColumnWidth(1, rect.Width()/6-2);
		m_lstTasks.SetColumnWidth(2, rect.Width()/6-2);
		m_lstTasks.SetColumnWidth(3, rect.Width()/6-2);
		m_lstTasks.SetColumnWidth(4, rect.Width()/6-2);
		m_lstTasks.SetColumnWidth(5, rect.Width()/6-2);
	}
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST_JOB_MACHINE)))
	{
		CRect rect;
		GetClientRect(rect);
		//rect.top	+= 10;
		rect.left	+= 10;

		rect.top = rect.top + rect.Height() /4 + 1;
//		rect.bottom = rect.top + rect.Height()	;
		m_lstJobMachine.MoveWindow(rect);

		m_lstJobMachine.SetColumnWidth(0, rect.Width()/9-2);
		m_lstJobMachine.SetColumnWidth(1, rect.Width()/9);
		m_lstJobMachine.SetColumnWidth(2, rect.Width()/9-2);
		m_lstJobMachine.SetColumnWidth(3, rect.Width()/9-2);
		m_lstJobMachine.SetColumnWidth(4, rect.Width()/9-2);
		m_lstJobMachine.SetColumnWidth(5, rect.Width()/9-2);
		m_lstJobMachine.SetColumnWidth(6, rect.Width()/9-2);
		m_lstJobMachine.SetColumnWidth(7, rect.Width()/9-2);
		m_lstJobMachine.SetColumnWidth(8, rect.Width()/9-2);
//		m_lstJobMachine.SetColumnWidth(2, rect.Width()/8-2);
	}
	this->Invalidate();

	return TRUE;
}

void CPageTasks::Set_channel_state ( LPPACK_CHANNEL_STATE lpChannelState )
{
	DWORD					dwSizePackage;
	int						i, iSize;
	POSITION				pos;
	LPCHANNELATTRIB			lpAttrib;
	LPCHANNEL_STAT			lpState;
	LPMACHINE_ATTRIB		lpMachineAttrib;
	LPPACK_CHANNEL_STATE	lpNew_pack;

	lpAttrib	= &lpChannelState->attribChannel;
	lpState		= &lpChannelState->statisChannel;

	iSize	= 0	;
	iSize	+= sizeof ( CHANNELATTRIB ) + sizeof ( CHANNEL_STAT );
	iSize	+= lpState->m_iCntMachine * sizeof ( MACHINE_ATTRIB );

	// 计算包的大小
	dwSizePackage	= sizeof( PACK_CHANNEL_STATE ) + iSize - sizeof(MACHINE_ATTRIB);

	// 寻找是否有该channel的前期信息
	pos	= m_ptrState_list.GetHeadPosition();
	for ( i=0; i<m_ptrState_list.GetCount(); i++ )
	{
		lpNew_pack	= (LPPACK_CHANNEL_STATE) m_ptrState_list.GetNext(pos);

		if ( lpNew_pack->attribChannel.m_iTaskID == lpAttrib->m_iTaskID )
			break;
	}

	//m_lstTasks.GetHotItem();
	if ( i == m_ptrState_list.GetCount() )
	{
		lpNew_pack	= (LPPACK_CHANNEL_STATE) new BYTE [dwSizePackage];
		m_ptrState_list.AddTail ( lpNew_pack );
	}

	::memcpy ( lpNew_pack, lpChannelState, dwSizePackage );

	// 添加或更新信息
	Update_user_group ( lpNew_pack );
}

void CPageTasks::SetChannelInfo( LPMACHINE_ATTRIB lpMachineAttrib )
{
	int		iIndex;
	CString	str;
	str.Format( "%d", lpMachineAttrib->m_iMachineID );

	LVFINDINFO info;

	info.flags	= LVFI_STRING;
	info.psz	= (LPCTSTR)str;

	iIndex	= m_lstJobMachine.FindItem( &info );
	if ( iIndex == -1 )
	{
		iIndex	= m_lstJobMachine.InsertItem( 0, str );
	}

	str.Format( "%s", lpMachineAttrib->m_strAccount );
	m_lstJobMachine.SetItemText( iIndex, 1, str );

	str.Format( "%s", lpMachineAttrib->m_strName );
	m_lstJobMachine.SetItemText( iIndex, 2, str );

	str.Format( "%d", lpMachineAttrib->m_iPriority );
	m_lstJobMachine.SetItemText( iIndex, 3, str );

	BYTE	yState	= lpMachineAttrib->m_enumStatus;
	if ( yState & Machine_OffLine )
		str.Format( ".." );
	//else if ( yState & Machine_DropLine )
	//	str.Format( "掉线" );
	else if ( yState & Machine_OnLine )
		str.Format( "在线" );
	else if ( yState & Machine_Logout )
		str.Format( "=>登出" );
	else if ( yState & Machine_Kickout )
		str.Format( "__\\被踢出" );
	else if ( yState & Machine_ReLogin )
		str.Format( "自动登陆" );
	else
		str.Format( "其他" );
	m_lstJobMachine.SetItemText( iIndex, 4, str );

	str.Format( "%d", lpMachineAttrib->m_iCntPrinted );
	m_lstJobMachine.SetItemText( iIndex, 5, str );

	str.Format( "%d", lpMachineAttrib->m_iAcceptCnt );
	m_lstJobMachine.SetItemText( iIndex, 6, str );

	if ( lpMachineAttrib->m_iSentExCnt > lpMachineAttrib->m_iSentCnt )
		str.Format( "%d", lpMachineAttrib->m_iSentExCnt );
	else
		str.Format( "%d", lpMachineAttrib->m_iSentCnt );
	m_lstJobMachine.SetItemText( iIndex, 7, str );
}

//============================================
//
//     Browse the Group memebers of a single group
//
//============================================
void CPageTasks::OnNMClickListGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;
	
	//Get the scangroup ID
	int		iItem	= lpnmitem->iItem;
	
	CString str;
	m_lstTasks.GetItemText(iItem,0,str.GetBuffer(20),20);
	int		nGroupID	= (int)atoi(str);

	m_pParentDlg->Set_current_questionID ( nGroupID );

	if ( m_nCurrentUserGroupID == nGroupID )
		return;

	m_nCurrentUserGroupID	= nGroupID;

	//m_pParentDlg->UpdateChannelState( nGroupID );
	
	m_lstJobMachine.DeleteAllItems(); 

	// 找出该 channel 的信息进行更新
	POSITION				pos;
	LPPACK_CHANNEL_STATE	lpNew_pack;

	pos	= m_ptrState_list.GetHeadPosition();
	for ( int i=0; i<m_ptrState_list.GetCount(); i++ )
	{
		lpNew_pack	= (LPPACK_CHANNEL_STATE) m_ptrState_list.GetNext(pos);

		if ( lpNew_pack->attribChannel.m_iTaskID == m_nCurrentUserGroupID )
		{
			Update_user_group ( lpNew_pack );

			break;
		}
	}

	//m_lstJobMachine.DeleteAllItems(); 
	//CMessageManager::PostMessageToNetWork(WM_GUI_NETWORK_GET_GROUP_MEMBERS,nGroupID,(char*)NULL);
	*pResult = 0;
}

void CPageTasks::OnNMRclickListGroupMember(NMHDR *pNMHDR, LRESULT *pResult)
{
	CString	str;
	CRect	r;

	LPNMITEMACTIVATE lpnmitem = (LPNMITEMACTIVATE) pNMHDR;

	// Get the corresponding MachineID
	int		iItem	= lpnmitem->iItem;
	m_lstJobMachine.GetItemText(iItem,0,str.GetBuffer(20),20);
	m_nCurrentMachineID	= (int)atoi(str);

	m_pParentDlg->Set_current_teacerID ( m_nCurrentMachineID );

	m_lstJobMachine.GetWindowRect( &r );

	int	iMinPos, iMaxPos, iCurPos, iCntItem;
	m_lstJobMachine.GetScrollRange ( SB_VERT, &iMinPos, &iMaxPos );
	iCurPos		= m_lstJobMachine.GetScrollPos ( SB_VERT );
	iCntItem	= m_lstJobMachine.GetItemCount ( );

	if ( 0 != iCurPos )
		iCurPos		= iCurPos * iCntItem / (iMaxPos - iMinPos);

	int iSel	= 0;
	POSITION	pos;
	pos	= m_lstJobMachine.GetFirstSelectedItemPosition();
	while ( NULL != pos )
		iSel	= m_lstJobMachine.GetNextSelectedItem( pos );

	int	iY	= r.top + 18 + (iSel - iCurPos)*13;

	CMenu* menu_bar		= AfxGetMainWnd()->GetMenu();
	CMenu* action_menu	= menu_bar->GetSubMenu(5);    
	ASSERT(action_menu);

	action_menu->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON, r.left + 30, iY, this);

	*pResult = 0;
}

BOOL CPageTasks::Create(UINT nIDTemplate, CWnd* pParentWnd)
{
	m_pParentDlg		= (CDlgOperator*)pParentWnd;

	return CDialog::Create(nIDTemplate, pParentWnd);
}

void CPageTasks::Update_user_group ( LPPACK_CHANNEL_STATE lpChannelState )
{
	LPCHANNELATTRIB		lpAttrib		;
	LPCHANNEL_STAT		lpState			;
	LPMACHINE_ATTRIB	lpMachineAttrib	;

	lpAttrib		= &lpChannelState->attribChannel;
	lpState			= &lpChannelState->statisChannel;

	CString str;
	str.Format("%d", lpAttrib->m_iTaskID);

	LVFINDINFO info;

	info.flags =LVFI_STRING;
	info.psz = (LPCTSTR)str;

	int nIndex = m_lstTasks.FindItem(&info);
	if (nIndex == -1)
	{
		nIndex	= m_lstTasks.InsertItem( 0, str );

//		str.Format("%1.0f", lpAttrib->);
		m_lstTasks.SetItemText( nIndex, 2, str );

		str.Format("%s", lpAttrib->m_strSubTask);
		m_lstTasks.SetItemText( nIndex, 1, str );

		//Update info
		str.Format("%d",lpState->m_iCntMachine);
		m_lstTasks.SetItemText( nIndex, 3, str );

		str.Format("%d",lpState->m_iCntPrinted);
		m_lstTasks.SetItemText( nIndex, 4, str);

		str.Format("%d",lpState->m_iCntTotal);
		m_lstTasks.SetItemText( nIndex, 5, str);

		for ( int i=0; i<lpState->m_iCntMachine; i++ )
		{
			lpMachineAttrib	= &lpChannelState->attribMachine[i];

			SetChannelInfo ( lpMachineAttrib )	;
		}
	}
	else
	{
		POSITION pos = m_lstTasks.GetFirstSelectedItemPosition();

		if ( nIndex == m_lstTasks.GetNextSelectedItem(pos) )
		{
//			str.Format("%1.0f",lpAttrib->);
			m_lstTasks.SetItemText( nIndex, 2, str );

			str.Format("%s", lpAttrib->m_strSubTask);
			m_lstTasks.SetItemText( nIndex, 1, str );

			//Update info
			str.Format("%d",lpState->m_iCntMachine);
			m_lstTasks.SetItemText( nIndex, 3, str );

			str.Format("%d",lpState->m_iCntPrinted);
			m_lstTasks.SetItemText( nIndex, 4, str);

			str.Format("%d",lpState->m_iCntTotal);
			m_lstTasks.SetItemText( nIndex, 5, str);

			for ( int i=0; i<lpState->m_iCntMachine; i++ )
			{
				lpMachineAttrib	= &lpChannelState->attribMachine[i];

				SetChannelInfo ( lpMachineAttrib )	;
			}
		}
	}
}

void CPageTasks::OnUserGroupAction(int nAction,int nTaskNum,
									   float fScore,int nCntOfMembers,
				                       int nSubmitTasksInTotal, int nTotalCnt, char strTitle[13])
{
	CString str;
	str.Format("%d", nTaskNum);

	LVFINDINFO info;

	info.flags =LVFI_STRING;
	info.psz = (LPCTSTR)str;

	int nIndex = m_lstTasks.FindItem(&info);
	if (nIndex == -1)
	{
		nIndex	= m_lstTasks.InsertItem( 0, str );

		str.Format("%1.0f",fScore);
		m_lstTasks.SetItemText( nIndex, 2, str );
	}

	str.Format("%s", strTitle);
	m_lstTasks.SetItemText( nIndex, 1, str );

	//Update info
	str.Format("%d",nCntOfMembers);
	m_lstTasks.SetItemText( nIndex, 3, str );

	str.Format("%d",nSubmitTasksInTotal);
	m_lstTasks.SetItemText( nIndex, 4, str);

	str.Format("%d",nTotalCnt);
	m_lstTasks.SetItemText( nIndex, 5, str);
}


void CPageTasks::OnNMRclickListUserGroup(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}
