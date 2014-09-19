#include "StdAfx.h"
#include ".\quality.h"

//#include "DlgQuality.h"

void CQuality::Recv_quality_segment ( LPPACK_QUALITY_SEGMENT lpQuality_segment )
{
	// 找到该条目
	LPQLINE	lpQLine;

	lpQLine	= m_lpQLine_lst_head;
	while ( NULL != lpQLine )
	{
		if ( lpQLine->m_iMachineID == lpQuality_segment->iMachineID )
		{
			break;
		}

		lpQLine	= lpQLine->m_lpNext;
	}

	// 没找到的话新建该条目
	if ( NULL == lpQLine )
	{
		lpQLine	= new QLINE;

		lpQLine->m_iIndex	= m_iLine_cnt;	// 索引从 0 开始
		m_iLine_cnt++;

		lpQLine->m_iMachineID	= lpQuality_segment->iMachineID;
		::memcpy ( lpQLine->m_strName, lpQuality_segment->strName, 11 );

		// 加到链表的尾巴上
		if ( NULL == m_lpQLine_lst_head )
		{
			m_lpQLine_lst_head	= lpQLine;
			m_lpQLine_lst_tail	= lpQLine;
		}
		else
		{
			m_lpQLine_lst_tail->m_lpNext	= lpQLine;
			m_lpQLine_lst_tail				= lpQLine;
		}
	}

	// 拷贝数据
	lpQLine->Add_new_data ( lpQuality_segment->data, lpQuality_segment->dwLen );

	// 计算总平均
	m_fMean	= Calculate_average_n_max_sum_cnt ( );
}

void CQuality::Set_interface_dlg ( CDlgQuality *pDlg )
{
	m_pDlg	= pDlg;
}

double CQuality::Calculate_average_n_max_sum_cnt ( )
{
	LPQLINE	lpQLine;

	double	dSum	= 0.;
	DWORD	dwCnt	= 0;

	m_iMax_sum_cnt	= 0;

	lpQLine	= m_lpQLine_lst_head;
	while ( NULL != lpQLine )
	{
		dSum	+= lpQLine->m_dwSum;
		dwCnt	+= lpQLine->m_nSum_length;

		if ( m_iMax_sum_cnt < (int)lpQLine->m_nSum_cnt )
			m_iMax_sum_cnt	= lpQLine->m_nSum_cnt;

		lpQLine	= lpQLine->m_lpNext;
	}

	if ( 0 == dwCnt )
		return	0.;

	// 计算水平量化因子
	int		iWidth_rect;

	iWidth_rect	= m_rcRect.Width() - (2 * MARGIN_H);
	m_fFactor_H	= iWidth_rect / m_iMax_sum_cnt;

	// 计算纵向量化因子
	int		iHeight_rect;

	iHeight_rect	= m_rcRect.Height() - (2 * MARGIN_H);
	m_fFactor_V	= iHeight_rect / m_iMax_vertical;

	return	dSum / dwCnt;
}

CQuality::CQuality(void)
{
	m_lpQLine_lst_head	= NULL;
	m_lpQLine_lst_tail	= NULL;

	m_iLine_cnt		= 0;
	m_iMax_sum_cnt	= 0;

	BYTE	yR, yG, yB;
	for ( int i=0; i<=SHIFT_QUALITY; i++ )
	{
		yR	= 255;
		yG	= 255;
		yB	= 255;

		switch ( i%6 )
		{
		case	0:
			yR	-= (i*3);
			yG	= 27;
			yB	= 27;
			break;

		case	1:
			yG	-= (i*3);
			yR	= 27;
			yB	= 27;
			break;

		case	2:
			yB	-= (i*3);
			yR	= 27;
			yG	= 27;
			break;

		case	3:
			yR	-= (i*3);
			yG	-= (i*3);
			yB	= 27;
			break;

		case	4:
			yG	-= (i*3);
			yB	-= (i*3);
			yR	= 27;
			break;

		case	5:
			yB	-= (i*3);
			yR	-= (i*3);
			yG	= 27;
			break;

		default:
			break;
		}

		m_crLine_colors[i]	= RGB(yR, yG, yB);
	
		m_penLine[i].CreatePen (PS_SOLID, 1, RGB(yR, yG, yB));
	}
}

CQuality::~CQuality(void)
{
	LPQLINE	lpQLine;

	lpQLine	= m_lpQLine_lst_head;
	while ( NULL != lpQLine )
	{
		m_lpQLine_lst_head	= lpQLine->m_lpNext;

		delete	lpQLine;

		lpQLine	= m_lpQLine_lst_head;
	}
}

void CQuality_line::Calculate_sum ( )
{
	LPBYTE	lpyCurrent;

	lpyCurrent	= m_lpyBuffer + m_nSum_length;
	while ( m_nSum_length+SUM_SEGMENT_LENGTH <= m_nBuffer_length )
	{
		WORD	wSum;

		wSum	= 0;
		for ( UINT n=0; n<SUM_SEGMENT_LENGTH; n++ )
		{
			wSum	+= *lpyCurrent++;
		}

		// 判断是否需要增加求和缓冲
		if ( 0 == (m_nSum_cnt % SUM_BUFFER_INCREASING) )
		{
			LPWORD	lpwNew;
			float	*lpfNew;

			lpwNew	= new WORD[m_nSum_cnt + SUM_BUFFER_INCREASING];
			lpfNew	= new float[m_nSum_cnt + SUM_BUFFER_INCREASING];
			if ( NULL != m_lpwSum )
			{
				::memcpy ( lpwNew, m_lpwSum, m_nSum_cnt*sizeof(WORD) );
				::memcpy ( lpfNew, m_lpfMean, m_nSum_cnt*sizeof(float) );

				delete	m_lpwSum;
				delete	m_lpfMean;
			}

			m_lpwSum	= lpwNew;
			m_lpfMean	= lpfNew;
		}

		*(m_lpwSum + m_nSum_cnt )	= wSum;

		if ( 0 == m_nSum_cnt )
		{
			*(m_lpfMean + m_nSum_cnt )	= 0.;
		}
		else
		{
			*(m_lpfMean + m_nSum_cnt )	= (*(m_lpwSum + m_nSum_cnt - 1) + wSum) / 2. / SUM_SEGMENT_LENGTH;
		}

		m_nSum_cnt++;
		m_nSum_length	+= SUM_SEGMENT_LENGTH;

		m_dwSum	+= wSum;
	}
}

void CQuality_line::Add_new_data ( LPBYTE lpyBuffer, UINT nLength )
{
	LPBYTE	lpyBuffer_new;

	lpyBuffer_new	= new BYTE[m_nBuffer_length + nLength];

	::memcpy ( lpyBuffer_new + m_nBuffer_length, lpyBuffer, nLength );

	if ( NULL != m_lpyBuffer )
	{
		::memcpy ( lpyBuffer_new, m_lpyBuffer, m_nBuffer_length );
		delete	m_lpyBuffer;
	}

	m_lpyBuffer	= lpyBuffer_new;

	m_nBuffer_length	+= nLength;

	Calculate_sum ( );
}

CQuality_line::CQuality_line(void)
{
	m_lpyBuffer	= NULL;
	m_lpwSum	= NULL;
	m_lpNext	= NULL;

	m_nBuffer_length	= 0;
	m_nSum_length		= 0;
	m_nSum_cnt			= 0;
	m_dwSum				= 0;
}

CQuality_line::~CQuality_line(void)
{
	if ( NULL != m_lpwSum )
		delete	m_lpwSum;

	if ( NULL != m_lpyBuffer )
		delete	m_lpyBuffer;
}

void CQuality_line::Draw ( CDC *pDC, CPen& pen,
						  float fScaleH, float fScaleV,
						  int iLeft, int iTop, int iHeight )
{
	int		iX, iY;
	float	*lpfMean;

	CPen* pOldPen = pDC->SelectObject(&pen);

	lpfMean	= m_lpfMean+1;
	iX		= iLeft;
	iY		= iHeight - (*lpfMean * fScaleV + iTop) + 0.5;
	pDC->MoveTo ( iX, iY );
	for ( UINT n=1; n<m_nSum_cnt; n++ )
	{
		iX	= iLeft + (n-1) * fScaleH;
		iY	= iHeight - (*lpfMean * fScaleV + iTop) + 0.5;

		pDC->LineTo ( iX, iY );

		lpfMean++;
	}

	pDC->SelectObject(pOldPen);
}