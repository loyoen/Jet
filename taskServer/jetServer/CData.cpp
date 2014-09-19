#include "StdAfx.h"
#include "CData.h"

CData::CData()
{
	PackNum = 0;
	mpPackages = NULL;
}

CData::CData(int type, int R)
{
	Point Origin;
	Origin.iXpos = 0;
	Origin.iYpos = 0;
	Point*	pPoints;
	int Num = 0;
	switch(type)
	{
	case CIRCLE:			Num = 60;
							pPoints = GetCirclePos(&Origin,R,Num);
							PackageData(pPoints,Num);
							delete pPoints;
							break;
	case SQUARE:			pPoints = GetSquarePos(&Origin,R,R,Num);
							PackageData(pPoints,Num);
							delete pPoints;
							break;
	default:				break;
	}
}
CData::~CData()
{
	if(mpPackages!=NULL)
		delete mpPackages;
}
void CData::PackageData(Point *pPoints, int PointNum)
{
	tagPrinterData *ptagPrinterData = new tagPrinterData[PointNum];
	for(int i=0;i<PointNum;i++)
	{
		ptagPrinterData[i].bSwitchControl = true;
		ptagPrinterData[i].cColorSelect = 0;
		ptagPrinterData[i].iTime = 200000;
		ptagPrinterData[i].XAxis = pPoints[i].iXpos;
		ptagPrinterData[i].YAxis = pPoints[i].iYpos;
		ptagPrinterData[i].ZAxis = 1;
		ptagPrinterData[i].bContinuous = 1;
	}
	ptagPrinterData[0].bContinuous = 0;
	ptagPrinterData[PointNum-1].bContinuous = -1;


	int DataNumInOnePack = (sizeof(tagPackageBody)/sizeof(tagPrinterData));
	DWORD dwPackNum = PointNum / DataNumInOnePack;

	int LastNum = 0;
	if(PointNum % DataNumInOnePack != 0)
	{
		dwPackNum ++;
		LastNum = PointNum % DataNumInOnePack;
	}
	LPPACKAGE pPackages = new PACKAGE[dwPackNum];
	if(LastNum == 0)
		for(int i=0;i<int(dwPackNum);i++)
		{
			//memset(pPackages[i].pckBody.lpBuffer,0,sizeof(pPackages[i].pckBody.lpBuffer));
			char* DataPack = (char*)malloc(sizeof(tagTaskData)+sizeof(tagPrinterData)*DataNumInOnePack);
			tagTaskData mytagTaskData;
			mytagTaskData.dwID = i;
			mytagTaskData.dwPackNum = dwPackNum;
			mytagTaskData.dwTaskId = 0;
			memcpy(DataPack,&mytagTaskData,sizeof(mytagTaskData));
			
			memcpy(DataPack+sizeof(tagTaskData),&ptagPrinterData[i*DataNumInOnePack],sizeof(tagPrinterData)*DataNumInOnePack);
			pPackages[i].pckBody.lpBuffer = DataPack;
			
			pPackages[i].pckHeader.dwLength = sizeof(tagTaskData)+sizeof(tagPrinterData)*DataNumInOnePack + sizeof(tagPackageHeader);
			pPackages[i].pckHeader.yType = TAG_PRINT_DATA;

		}
	else
	{
		for(int i=0;i<int(dwPackNum-1);i++)
		{
			char* DataPack = (char*)malloc(sizeof(tagTaskData)+sizeof(tagPrinterData)*DataNumInOnePack);
			tagTaskData mytagTaskData;
			mytagTaskData.dwID = i;
			mytagTaskData.dwPackNum = dwPackNum;
			mytagTaskData.dwTaskId = 0;
			memcpy(DataPack,&mytagTaskData,sizeof(mytagTaskData));
			
			memcpy(DataPack+sizeof(tagTaskData),&ptagPrinterData[i*DataNumInOnePack],sizeof(tagPrinterData)*DataNumInOnePack);
			pPackages[i].pckBody.lpBuffer = DataPack;
			
			pPackages[i].pckHeader.dwLength = sizeof(tagTaskData)+sizeof(tagPrinterData)*DataNumInOnePack + sizeof(tagPackageHeader);
			pPackages[i].pckHeader.yType = TAG_PRINT_DATA;
		}
		//memset(pPackages[dwPackNum-1].pckBody.lpBuffer,0,sizeof(pPackages[dwPackNum-1].pckBody.lpBuffer));
		char* DataPack = (char*)malloc(sizeof(tagTaskData)+sizeof(tagPrinterData)*LastNum);
		tagTaskData mytagTaskData;
		mytagTaskData.dwID = dwPackNum-1;
		mytagTaskData.dwPackNum = dwPackNum;
		mytagTaskData.dwTaskId = 0;
		memcpy(DataPack,&mytagTaskData,sizeof(mytagTaskData));
			
		memcpy(DataPack+sizeof(tagTaskData),&ptagPrinterData[(dwPackNum-1)*DataNumInOnePack],sizeof(tagPrinterData)*LastNum);
		
		pPackages[dwPackNum-1].pckHeader.dwLength = sizeof(tagTaskData)+sizeof(tagPrinterData)*LastNum + sizeof(tagPackageHeader);
		pPackages[dwPackNum-1].pckHeader.yType = TAG_PRINT_DATA;
	}
	mpPackages = pPackages;
	PackNum = dwPackNum;
	delete ptagPrinterData;
}

int CData::GetPackNum()
{
	return PackNum;
}
LPPACKAGE CData::GetPackages()
{
	return mpPackages;
}

Point* CData::GetCirclePos(Point *pOrigin, int R, int Num)
{
    //返回360个圆上点的坐标，每1°对应一个坐标;
	Point* pointArray=new Point[Num];
	int Cnt = 0;
	for(int angle=0;angle<360;angle+=360/Num)
	{
		pointArray[Cnt].iXpos=pOrigin->iXpos+int(R*cos(angle*PI/180));
		pointArray[Cnt].iYpos=pOrigin->iYpos+int(R*sin(angle*PI/180));
		int x = pointArray[Cnt].iXpos;
		int y = pointArray[Cnt].iYpos;
		Cnt++;
	}
	return pointArray;
}

Point* CData::GetSquarePos(Point *pOrign, int width, int length, int &Num)
{
	// 返回*Num个坐标，没一个刻度单位对应一个坐标
  
    Num = 2*(length+width);
    
	Point* pointArray = new Point[Num];
	int i, j, index;
	i=0, j=0;
	index=0;
	if(index<Num)
	{
		// x轴为0，y轴方向 （向↑）
		for(j; j<width; j++)
		{
			if(index<Num)
			{
				pointArray[index].iXpos = pOrign->iXpos + i;
				pointArray[index].iYpos = pOrign->iYpos + j;
				index++;
			}
		}
		// y轴为width，x轴方向 (向→)
		if(index<Num)
		{
			for(i; i<length; i++)
			{
				if(index<Num)
				{
					pointArray[index].iXpos = pOrign->iXpos + i;
					pointArray[index].iYpos = pOrign->iYpos + j;
					index++;
				}
			}
		}
		
		if(index<Num)
		{
			// 向↓
			for(j; j>0; j--)
			{
				if(index<Num)
				{
					pointArray[index].iXpos = pOrign->iXpos + i;
					pointArray[index].iYpos = pOrign->iYpos + j;
					index++;
				}
			}
			// 向←
			for(i; i>0;i--)
			{
				if(index<Num)
				{
					pointArray[index].iXpos = pOrign->iXpos + i;
					pointArray[index].iYpos = pOrign->iYpos + j;
					index++;
				}
			}
		}
	}
    return pointArray;
}
