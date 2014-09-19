#pragma once

#include "Protocol.h"

typedef	class	CQuality_line	QLINE, *LPQLINE;

#define	SHIFT_QUALITY	16

#define	SUM_SEGMENT_LENGTH		10
#define	SUM_BUFFER_INCREASING	10

#define	MARGIN_H	5
#define	MARGIN_V	5

class CDlgQuality;
class CQuality_line
{
public:
	int		m_iIndex;			// 索引从 0 开始

	int		m_iMachineID;
	char	m_strName[11];

	UINT	m_nBuffer_length;	// m_lpyBuffer 的长度
	UINT	m_nSum_length;		// buffer 中已经被求和的长度
	UINT	m_nSum_cnt;			// 和的个数 ( 即 m_lpwSum 的长度 )

	LPBYTE	m_lpyBuffer;
	LPWORD	m_lpwSum;

	DWORD	m_dwSum;

	LPQLINE	m_lpNext;

	float	*m_lpfMean;
//	UINT	m_nMean_cnt;

	COLORREF	m_crColor;

public:
	void	Add_new_data ( LPBYTE lpyBuffer, UINT nLength );
	void	Calculate_sum ( );

	void	Draw ( CDC *pDC, CPen& pen,
			float fScaleH, float fScaleV,
			int iLeft, int iTop, int iHeight ); 

public:
	CQuality_line(void);
	~CQuality_line(void);
};

class CQuality
{
public:
	CDlgQuality	*m_pDlg;

	COLORREF	m_crLine_colors[SHIFT_QUALITY+1];
	CPen		m_penLine[SHIFT_QUALITY+1];

	int		m_iLine_cnt;
	int		m_iMax_sum_cnt;
	int		m_iMax_vertical;

	float	m_fMean;

	float	m_fFactor_H;
	float	m_fFactor_V;

	LPQLINE	m_lpQLine_lst_head;
	LPQLINE	m_lpQLine_lst_tail;

	CRect	m_rcRect;

public:
	void	Recv_quality_segment ( LPPACK_QUALITY_SEGMENT lpQuality_segment );

	double	Calculate_average_n_max_sum_cnt ( );

	void	Set_drawing_rect ( CRect& rc ){m_rcRect = rc;};
	void	Set_interface_dlg ( CDlgQuality *pDlg );

	void	Draw_quality ( CDC *pDC, int iShift, int iCnt );

	void	Set_Max_vertical_value ( int iValue ){m_iMax_vertical=iValue;};

public:
	CQuality(void);
	~CQuality(void);
};
