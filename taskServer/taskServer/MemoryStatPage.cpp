// MemoryStatPage.cpp : 实现文件
//

#include "stdafx.h"
#include "MemoryStatPage.h"

//#include "packprotocol.h"

// CMemoryStatPage 对话框

IMPLEMENT_DYNAMIC(CMemoryStatPage, CDialog)
CMemoryStatPage::CMemoryStatPage(CWnd* pParent /*=NULL*/)
	: CDialog(CMemoryStatPage::IDD, pParent)
{
}

CMemoryStatPage::~CMemoryStatPage()
{
}

void CMemoryStatPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MEMORY, m_MemoryUsageStat);
}


BEGIN_MESSAGE_MAP(CMemoryStatPage, CDialog)
ON_WM_SIZE()
END_MESSAGE_MAP()


// CMemoryStatPage 消息处理程序
/********************************************************************/
/*																	*/
/* Function name : OnSize											*/		
/* Description   : Handle WM_SIZE message							*/
/*																	*/
/********************************************************************/
void CMemoryStatPage::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	if (IsWindow(::GetDlgItem(m_hWnd, IDC_LIST_MEMORY)))
	{
		CRect rect;
		GetClientRect(rect);
		m_MemoryUsageStat.MoveWindow(rect);

		m_MemoryUsageStat.SetColumnWidth(0, 0);
		m_MemoryUsageStat.SetColumnWidth(1, 100);
		m_MemoryUsageStat.SetColumnWidth(2, 100);
		m_MemoryUsageStat.SetColumnWidth(3, 60);
		m_MemoryUsageStat.SetColumnWidth(4, 60);
		m_MemoryUsageStat.SetColumnWidth(5, 80);
	}	
}

/********************************************************************/
/*																	*/
/* Function name : OnInitDialog										*/		
/* Description   : Called by the framework in response to the		*/
/*				   WM_INITDIALOG message.							*/
/*																	*/
/********************************************************************/
BOOL CMemoryStatPage::OnInitDialog() 
{
	CString strItem	;
//	int		nIndex	;

	CDialog::OnInitDialog();

	m_MemoryUsageStat.InsertColumn(0, _T("ID"));		
    m_MemoryUsageStat.InsertColumn(1, _T("内存类型"));	
    m_MemoryUsageStat.InsertColumn(2, _T("使用内存位置"));	
	m_MemoryUsageStat.InsertColumn(3, _T("当前个数"));
	m_MemoryUsageStat.InsertColumn(4, _T("峰值个数"));
	m_MemoryUsageStat.InsertColumn(5, _T("额定值"));
	
	DWORD dwStyle = m_MemoryUsageStat.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES ;
    m_MemoryUsageStat.SetExtendedStyle(dwStyle);   

	//Insert Item,Refer to GUINetworkInterfac.h

/*	nIndex	= m_MemoryUsageStat.InsertItem( 0, "0" )			;
	m_MemoryUsageStat.SetItemText( nIndex,1, "_EXLARGE_400K" )	;
	m_MemoryUsageStat.SetItemText( nIndex,2, "CNT_SERVER" )		;
	m_MemoryUsageStat.SetItemText( nIndex,3, "0" )				;
	m_MemoryUsageStat.SetItemText( nIndex,4, "0" )				;
	strItem.Format( "%d", CNT_SERVER_EXLARGE_MEM )				;
	m_MemoryUsageStat.SetItemText( nIndex,5, strItem )			;

	nIndex	= m_MemoryUsageStat.InsertItem( 0, "1" )			;
	m_MemoryUsageStat.SetItemText( nIndex,1, "_LARGE_100K" )	;
	m_MemoryUsageStat.SetItemText( nIndex,2, "CNT_SERVER" )		;
	m_MemoryUsageStat.SetItemText( nIndex,3, "0" )				;
	m_MemoryUsageStat.SetItemText( nIndex,4, "0" )				;
	strItem.Format( "%d", CNT_SERVER_LARGE_MEM )				;
	m_MemoryUsageStat.SetItemText( nIndex,5, strItem )			;

	nIndex	= m_MemoryUsageStat.InsertItem( 0, "2" )			;
	m_MemoryUsageStat.SetItemText( nIndex,1, "_NORMAL_40K" )	;
	m_MemoryUsageStat.SetItemText( nIndex,2, "CNT_SERVER" )		;
	m_MemoryUsageStat.SetItemText( nIndex,3, "0" )				;
	m_MemoryUsageStat.SetItemText( nIndex,4, "0" )				;
	strItem.Format( "%d", CNT_SERVER_NORMAL_MEM )				;
	m_MemoryUsageStat.SetItemText( nIndex,5, strItem )			;

	nIndex	= m_MemoryUsageStat.InsertItem( 0, "3" )			;
	m_MemoryUsageStat.SetItemText( nIndex,1, "_SMALL_8K" )		;
	m_MemoryUsageStat.SetItemText( nIndex,2, "CNT_SERVER" )		;
	m_MemoryUsageStat.SetItemText( nIndex,3, "0" )				;
	m_MemoryUsageStat.SetItemText( nIndex,4, "0" )				;
	strItem.Format( "%d", CNT_SERVER_SMALL_MEM )				;
	m_MemoryUsageStat.SetItemText( nIndex,5, strItem )			;

	nIndex	= m_MemoryUsageStat.InsertItem( 0, "4" )			;
	m_MemoryUsageStat.SetItemText( nIndex,1, "_COMMAND_1K" )	;
	m_MemoryUsageStat.SetItemText( nIndex,2, "CNT_SERVER" )		;
	m_MemoryUsageStat.SetItemText( nIndex,3, "0" )				;
	m_MemoryUsageStat.SetItemText( nIndex,4, "0" )				;
	strItem.Format( "%d", CNT_SERVER_COMMAND_MEM )				;
	m_MemoryUsageStat.SetItemText( nIndex,5, strItem )			;


	nIndex	= m_MemoryUsageStat.InsertItem( 0, "99" )			;


	nIndex	= m_MemoryUsageStat.InsertItem( 0, "5" )			;
	m_MemoryUsageStat.SetItemText( nIndex,1, "_EXLARGE_400K" )	;
	m_MemoryUsageStat.SetItemText( nIndex,2, "CNT_NET" )		;
	m_MemoryUsageStat.SetItemText( nIndex,3, "0" )				;
	m_MemoryUsageStat.SetItemText( nIndex,4, "0" )				;
	strItem.Format( "%d", CNT_NET_EXLARGE_MEM );
	m_MemoryUsageStat.SetItemText( nIndex,5, strItem )			;

	nIndex	= m_MemoryUsageStat.InsertItem( 0, "6" )			;
	m_MemoryUsageStat.SetItemText( nIndex,1, "_LARGE_100K" )	;
	m_MemoryUsageStat.SetItemText( nIndex,2, "CNT_NET" )		;
	m_MemoryUsageStat.SetItemText( nIndex,3, "0" )				;
	m_MemoryUsageStat.SetItemText( nIndex,4, "0" )				;
	strItem.Format( "%d", CNT_NET_LARGE_MEM )					;
	m_MemoryUsageStat.SetItemText( nIndex,5, strItem )			;

	nIndex	= m_MemoryUsageStat.InsertItem( 0, "7" )			;
	m_MemoryUsageStat.SetItemText( nIndex,1, "_NORMAL_40K" )	;
	m_MemoryUsageStat.SetItemText( nIndex,2, "CNT_NET" )		;
	m_MemoryUsageStat.SetItemText( nIndex,3, "0" )				;
	m_MemoryUsageStat.SetItemText( nIndex,4, "0" )				;
	strItem.Format( "%d", CNT_NET_NORMAL_MEM )					;
	m_MemoryUsageStat.SetItemText( nIndex,5, strItem )			;

	nIndex	= m_MemoryUsageStat.InsertItem( 0, "8" )			;
	m_MemoryUsageStat.SetItemText( nIndex,1, "_SMALL_8K" )		;
	m_MemoryUsageStat.SetItemText( nIndex,2, "CNT_NET" )		;
	m_MemoryUsageStat.SetItemText( nIndex,3, "0" )				;
	m_MemoryUsageStat.SetItemText( nIndex,4, "0" )				;
	strItem.Format( "%d", CNT_NET_SMALL_MEM )					;
	m_MemoryUsageStat.SetItemText( nIndex,5, strItem )			;

	nIndex	= m_MemoryUsageStat.InsertItem( 0, "9" )			;
	m_MemoryUsageStat.SetItemText( nIndex,1, "_COMMAND_1K" )	;
	m_MemoryUsageStat.SetItemText( nIndex,2, "CNT_NET" )		;
	m_MemoryUsageStat.SetItemText( nIndex,3, "0" )				;
	m_MemoryUsageStat.SetItemText( nIndex,4, "0" )				;
	strItem.Format( "%d", CNT_NET_COMMAND_MEM )					;
	m_MemoryUsageStat.SetItemText( nIndex,5, strItem )			;
*/
	//str.Format("%d",MEMORY_TYPE_RAM_DB);
	//nIndex=m_MemoryUsageStat.InsertItem(0, str);
	//m_MemoryUsageStat.SetItemText(nIndex,1, "RAM DB管理器");
	//str.Format("%1.1f",0.0f);
 //   m_MemoryUsageStat.SetItemText(nIndex,2,str);
	//
	//str.Format("%d",MEMORY_TYPE_USER_MANAGER);
	//nIndex=m_MemoryUsageStat.InsertItem(0,str);
	//m_MemoryUsageStat.SetItemText(nIndex,1,"用户管理器");
	//str.Format("%1.1f",0.0f);
 //   m_MemoryUsageStat.SetItemText(nIndex,2,str);

	//str.Format("%d",MEMORY_TYPE_PAPER_MANAGER);
	//nIndex=m_MemoryUsageStat.InsertItem(0,str);
	//m_MemoryUsageStat.SetItemText(nIndex,1, "任务管理器");
	//str.Format("%1.1f",0.0f);
 //   m_MemoryUsageStat.SetItemText(nIndex,2,str);

	//str.Format("%d",MEMORY_TYPE_FTP_MANAGER);	
	//nIndex=m_MemoryUsageStat.InsertItem(0,str);
	//m_MemoryUsageStat.SetItemText(nIndex,1,"文件传输管理器");
	//str.Format("%1.1f",0.0f);
 //   m_MemoryUsageStat.SetItemText(nIndex,2,str);

	//str.Format("%d",MEMORY_TYPE_NETWORK);	
	//nIndex=m_MemoryUsageStat.InsertItem(0,str);
 //   m_MemoryUsageStat.SetItemText(nIndex,1,"网络协议管理器");
	//str.Format("%1.1f",0.0f);
 //   m_MemoryUsageStat.SetItemText(nIndex,2,str);

	//str.Format("%d",MEMORY_TYPE_MESSAGE);	
	//nIndex=m_MemoryUsageStat.InsertItem(0,str);
 //   m_MemoryUsageStat.SetItemText(nIndex,1,"消息管理器");
	//str.Format("%1.1f",0.0f);
 //   m_MemoryUsageStat.SetItemText(nIndex,2,str);

	return TRUE;
}

/*++
[Description]
       Update memory Usage statistics. 
[Parameters]
     [IN]BYTE nItem
	        see WM_NETWORK_MEMORY definition in GUINetworkInterface.h
     [IN]int nCntMemory
	         the memory usage changed,if positive,new memory allocated
			                          if negative,memory freed!
[Return Value]
      None!
--*/
/*
//	SKLSE 2005-08-18, jicheng, displaying memmory state, begin
void CMemoryStatPage::UpdateMemoryStatistics( t_network_server_memory* lpMemState )
{
	CString		strID	;
	CString		strItem	;
	int			nIndex	;
	LVFINDINFO	info	;

	// 准备查找的信息
	strID.Format( "%d", lpMemState->nType )	;
//	info.flags	= LVFI_PARTIAL|LVFI_STRING	;
	info.flags	= LVFI_STRING				;
	info.psz	= (LPCTSTR) strID			;

	// 找到响应的行
   	nIndex	= m_MemoryUsageStat.FindItem( &info )	;
	if ( -1 == nIndex )
	    return; 

	// 更新该行的相关值
	strItem.Format( "%d", lpMemState->iCntCurrent )		;
	m_MemoryUsageStat.SetItemText( nIndex, 3, strItem )	;
	strItem.Format( "%d", lpMemState->iCntMax )			;
	m_MemoryUsageStat.SetItemText( nIndex, 4, strItem )	;
	// 暂时不更新额定值, 故屏蔽下面的行
//	strItem.Format( "%d", lpMemState->iCntRating )		;
//	m_MemoryUsageStat.SetItemText( nIndex, 5, strItem )	;

	//int nFormerUsage=0;
	//CString strText;
	//m_MemoryUsageStat.GetItemText(nIndex,2,strText.GetBuffer(20),20);
	//strText.ReleaseBuffer();
	//nFormerUsage=atoi(strText);

	//nFormerUsage+=nCntMemory;
	//if(nFormerUsage<0)
	//	nFormerUsage=0;
	//strText.Format("%1.1f",(float)nFormerUsage/1024.0f);
	//m_MemoryUsageStat.SetItemText(nIndex,2,strText);
}
//	SKLSE 2005-08-18, jicheng, displaying memmory state, ends
*/