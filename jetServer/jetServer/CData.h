#ifndef CDATA_H
#define CDATA_H
#include "package.h"
#include <math.h>
#define CIRCLE	1
#define SQUARE	2
#define PI 3.14159

struct Point
{
    int iXpos;
    int iYpos;
};

class CData
{
private:
	int					PackNum;
	LPPACKAGE		mpPackages;
	Point* GetCirclePos(Point *pOrigin, int R, int Num);
	Point *GetSquarePos(Point *pOrign, int width, int length, int &Num);
	void PackageData(Point* pPoints,int PointNum);
public:
	CData();
	CData(int type,int R);
	~CData();
	int GetPackNum();
	LPPACKAGE GetPackages();
};


#endif