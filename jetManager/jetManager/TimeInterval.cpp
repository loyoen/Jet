// TimeInterval.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "jetManager.h"
#include "TimeInterval.h"


// CTimeInterval �Ի���

IMPLEMENT_DYNAMIC(CTimeInterval, CDialog)
CTimeInterval::CTimeInterval(CWnd* pParent /*=NULL*/)
	: CDialog(CTimeInterval::IDD, pParent)
	, nTimeInterval(0)
{
}

CTimeInterval::~CTimeInterval()
{
}

void CTimeInterval::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TIME_INTERVAL, nTimeInterval);
}


BEGIN_MESSAGE_MAP(CTimeInterval, CDialog)
END_MESSAGE_MAP()


// CTimeInterval ��Ϣ�������
