// DlgPrePrinting.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "../jetManager.h"
#include "DlgPrePrinting.h"
#include "dlgManager.h"

#include "DlgPassword.h"

// CDlgPrePrinting �Ի���

IMPLEMENT_DYNAMIC(CDlgPrePrinting, CDialog)
CDlgPrePrinting::CDlgPrePrinting(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPrePrinting::IDD, pParent)
	, m_strEdit1(_T(""))
{
	m_pParent	= (CDlgManager*) pParent	;

	m_iShift	= 0;

	m_iPreMarkKind	= 0;

	m_lpPrePrintingEx	= NULL;
}

CDlgPrePrinting::~CDlgPrePrinting()
{
	if ( NULL != m_lpPrePrintingEx )
		delete	m_lpPrePrintingEx;
}

void CDlgPrePrinting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_MSG1, m_editMsg1);
	DDX_Control(pDX, IDC_EDIT_MSG2, m_editMsg2);
	DDX_Text(pDX, IDC_EDIT1, m_strEdit1);
}


BEGIN_MESSAGE_MAP(CDlgPrePrinting, CDialog)
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_BTN_LEFT, OnBnClickedBtnLeft)
	ON_BN_CLICKED(IDC_BTN_RIGHT, OnBnClickedBtnRight)
//	ON_WM_NCHITTEST()
	ON_BN_CLICKED(IDC_NORMAL, OnBnClickedNormal)
END_MESSAGE_MAP()


// CDlgPrePrinting ��Ϣ�������
void CDlgPrePrinting::ShowInfo ( )
{
	CString	strTitle;
	CString	strData;
	WORD	wCnt;
	char	cName[11];
	int		i;

	if ( NULL == m_lpPrePrintingEx )
		return;

	// ��ʾ��ǰ���ύ��ҵ�Ļ���������ҵ������
	strTitle.Format ( "��ǰ����%d�����ύ��ҵ���ϼ���%d�ֲ�ͬ��ҵ", 
		m_lpPrePrintingEx->iPreMarkDone,
		m_lpPrePrintingEx->iPreMarkKind );
	m_editMsg1.SetWindowText ( strTitle );

	// ��ʾ��ϸ���嵥
	LPBYTE			lpyPtr;
	LPWORD			lpwPtr;
	lpyPtr	= (LPBYTE) m_lpPrePrintingEx->cInfo;
	for ( i=0; (i<m_lpPrePrintingEx->iPreMarkKind) && (i<m_iShift+8); i++ )
	{
		lpwPtr	= (LPWORD) lpyPtr;
		wCnt	= *lpwPtr++;
		strData.Format ( "%d����\r\n======", wCnt );
		strTitle	+= strData;

		lpyPtr	= (LPBYTE) lpwPtr;
		for ( WORD w=0; w<wCnt; w++ )
		{
			::memcpy ( cName, lpyPtr, 11 );
			lpyPtr	+= 11;

			strData.Format ( "\r\n%s", cName );
			strTitle	+= strData;
		}

		if ( (0 <= i-m_iShift) && (8 > i-m_iShift) )
			GetDlgItem ( IDC_EDIT1 + i - m_iShift )->SetWindowText ( strTitle );
	}

	for ( ; i<(m_iShift/8) * 8 + 8; i++ )
	{
		if ( (0 <= i-m_iShift) && (8 > i-m_iShift) )
			GetDlgItem ( IDC_EDIT1 + i - m_iShift )->SetWindowText ( " " );
	}

	m_editMsg2.SetWindowText ( strTitle );

	strTitle.Format ( "��ǰ�ǵ�%d����ҵ", m_iShift / 8 + 1 );
	GetDlgItem ( IDC_EDIT_POS )->SetWindowText ( strTitle );
}

float CDlgPrePrinting::CalculateAverage ( )
{
	float		fItem;
	float		fTotal;
	LPBYTE		lpyPtr;
	LPWORD		lpwPtr;
	WORD		wCnt;
	WORD		wTotal;

	if ( NULL == m_lpPrePrintingEx )
		return 0;

	// ���
	fTotal	= 0;
	wTotal	= 0;
	lpyPtr	= (LPBYTE) m_lpPrePrintingEx->cInfo;
	for ( int i=0; i<m_lpPrePrintingEx->iPreMarkKind; i++ )
	{
		fItem	= *lpyPtr++;

		lpwPtr	= (LPWORD) lpyPtr;
		wCnt	= *lpwPtr++;

		fItem	= fItem * wCnt;

		wTotal	+= wCnt;
		fTotal	+= fItem;

		lpyPtr	= (LPBYTE) lpwPtr;
		for ( WORD w=0; w<wCnt; w++ )
		{
			lpyPtr	+= 11;
		}
	}

	// ����ƽ����ҵ
	if ( 0 == wTotal )
		return	0;
	else
		return	fTotal / wTotal;
}

void CDlgPrePrinting::OnServerPreGradedMarkEx ( LPPACK_PREGRADED_MARK_EX lpPreGradedMarkEx )
{
	int		iSize;
	char *	pBuffer;

	iSize	= lpPreGradedMarkEx->iSize;

	pBuffer	= new char[iSize];

	::memcpy ( pBuffer, lpPreGradedMarkEx, iSize );

	if ( m_lpPrePrintingEx )
		delete	m_lpPrePrintingEx;

	m_lpPrePrintingEx	= (LPPACK_PREGRADED_MARK_EX) pBuffer;

	m_iPreMarkKind	= m_lpPrePrintingEx->iPreMarkKind;
	m_fAverage	= CalculateAverage ( );

	m_iShift	= 0;
	ShowInfo ();
	GetDlgItem ( IDC_BTN_LEFT )->EnableWindow( FALSE );
	GetDlgItem ( IDC_BTN_RIGHT )->EnableWindow( m_iPreMarkKind > 8 );
}

void CDlgPrePrinting::OnBnClickedCancel()
{
	this->ShowWindow ( SW_HIDE );
	//OnCancel();
}

void CDlgPrePrinting::OnBnClickedOk()
{
	this->ShowWindow ( SW_HIDE );
	//OnOK();
}

void CDlgPrePrinting::OnBnClickedBtnLeft()
{
	if ( 0 == m_iShift )
		return;

	m_iShift	-= 8;
	ShowInfo ( );

	if ( m_iShift <= 0 )
	{
		GetDlgItem ( IDC_BTN_LEFT )->EnableWindow( FALSE );
	}

	GetDlgItem ( IDC_BTN_RIGHT )->EnableWindow( );
}

void CDlgPrePrinting::OnBnClickedBtnRight()
{
	if ( m_iPreMarkKind < m_iShift )
		return;

	m_iShift	+= 8;
	ShowInfo ( );

	if ( m_iShift >= m_iPreMarkKind - 8 )
	{
		GetDlgItem ( IDC_BTN_RIGHT )->EnableWindow( FALSE );
	}

	GetDlgItem ( IDC_BTN_LEFT )->EnableWindow( );
}

UINT CDlgPrePrinting::OnNcHitTest(CPoint point)
{
//	return	0;

	return CDialog::OnNcHitTest(point);
}

void CDlgPrePrinting::OnBnClickedNormal()
{
	CDlgPassword	dlg;

	dlg.Set_dlg_title ( "�鳤׼��������ʽ��ӡ" );
	dlg.Set_message_string ( "������ò���������:" );
	dlg.Set_verify_psw ( "84817422" );

	if ( IDOK != dlg.DoModal() )
		return;

	GetDlgItem ( IDC_NORMAL )->EnableWindow ( FALSE );
	GetDlgItem ( IDC_NORMAL )->ShowWindow ( SW_HIDE );

//	m_pParent->Apply_for_normal_process ( );
}
