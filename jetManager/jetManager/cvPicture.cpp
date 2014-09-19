#include "StdAfx.h"
#include "cvPicture.h"
cvPic::cvPic(int id,string srcImg)
{
	TaskId = id;
	ImgName = srcImg;
}
void cvPic::packagePictureInfo( const Mat& bwImage, vector<int>& imgX, vector<int>& imgY, vector<int>& imgColor )
{
	int nRGB;
	for ( int i=0; i<bwImage.rows; i++ )
    {
        const uchar* rowData = bwImage.ptr<uchar>( i );						// ��ȡ��i�е�ָ��		
        for ( int j=0; j<bwImage.cols; j++ )
        {
			imgX.push_back( i );
			imgY.push_back( j );
			nRGB = (rowData[3*j] << 16) | (rowData[3*j+1] << 8) | rowData[3*j+2]; 
			imgColor.push_back( nRGB );
        }
    }
}
void cvPic::waitESC()
{

}
void cvPic::fillRunVectors( const Mat& bwImage, int& NumberOfRuns, vector<int>& stRun, vector<int>& enRun, vector<int>& rowRun )
{
	for ( int i=0; i<bwImage.rows; i++ )
    {
        const uchar* rowData = bwImage.ptr<uchar>( i );						// ��ȡ��i�е�ָ��
        if ( rowData[0] == 0 )
        {// ��һ�����ص㱻�ж�Ϊ�ŵĿ�ʼ
            NumberOfRuns++;
            stRun.push_back( 0 );
            rowRun.push_back( i );
        }
        for ( int j=1; j<bwImage.cols; j++ )
        {
            if ( rowData[j-1] == 255 && rowData[j] == 0 )
            {// һ���ſ�ʼ
                NumberOfRuns++;
                stRun.push_back( j );
                rowRun.push_back( i );
            }
            else if ( rowData[j-1] == 0 && rowData[j] == 255 )
            {// һ���Ž���
                enRun.push_back(j-1);
            }
        }
        if ( rowData[bwImage.cols-1] == 0 )
        {// ���һ�����ص㱻�ж�Ϊ�ŵĽ���
            enRun.push_back( bwImage.cols-1 );
        }
    }
}
void cvPic::firstPass( vector<int>& stRun, vector<int>& enRun, vector<int>& rowRun, int NumberOfRuns, vector<int>& runLabels, vector<pair<int,int>>& equivalences, int offset )
{
	runLabels.assign( NumberOfRuns, 0 );				// ��ʼ��runLabels

    int idxLabel = 1;
    int curRowIdx = 0;
    int firstRunOnCur = 0;								// ��ǰ�еĵ�һ���ŵ�����
    int firstRunOnPre = 0;								// ��һ�еĵ�һ���ŵ�����
    int lastRunOnPre = -1;								// ��һ�е����һ���ŵ�����
	
    for ( int i=0; i<NumberOfRuns; i++ )
    {
        if ( rowRun[i] != curRowIdx )
        {// �����µ�һ��
            curRowIdx = rowRun[i];
            firstRunOnPre = firstRunOnCur;
            lastRunOnPre = i - 1;
            firstRunOnCur = i;
        }
        for ( int j=firstRunOnPre; j<=lastRunOnPre; j++ )
        {
            if ( stRun[i]<=enRun[j]+offset && enRun[i]>=stRun[j]-offset )
            {
                if ( runLabels[i] == 0 )				// û�б���Ź�
                    runLabels[i] = runLabels[j];		// ����һ�еĵ��Ǹ��ŵı�Ÿ�����
                else if ( runLabels[i] != runLabels[j] )// �Ѿ������             
                    equivalences.push_back( make_pair(runLabels[i],runLabels[j]) ); // ����ȼ۶�
            }
        }
        if ( runLabels[i] == 0 )						// û����ǰһ�е��κ�run�غ�
            runLabels[i] = idxLabel++;
    }
}
void cvPic::replaceSameLabel( vector<int>& runLabels, vector<pair<int,int>>& equivalence )
{
	int maxLabel = *max_element( runLabels.begin(), runLabels.end() );
    vector<vector<bool>> eqTab( maxLabel, vector<bool>(maxLabel,false) );
    vector<pair<int,int>>::iterator vecPairIt = equivalence.begin();
    while ( vecPairIt != equivalence.end() )
    {
        eqTab[vecPairIt->first-1][vecPairIt->second-1] = true;
        eqTab[vecPairIt->second-1][vecPairIt->first-1] = true;
        vecPairIt++;
    }
    vector<int> labelFlag( maxLabel, 0 );
    vector<vector<int>> equaList;
    vector<int> tempList;

	printf( "�������:%d\n", maxLabel );

    for ( int i=1; i<=maxLabel; i++ )
    {
        if ( labelFlag[i-1] )
            continue;
        labelFlag[i-1] = equaList.size() + 1;
        tempList.push_back(i);
        for ( vector<int>::size_type j=0; j<tempList.size(); j++ )
        {
            for ( vector<bool>::size_type k = 0; k != eqTab[tempList[j]-1].size(); k++ )
            {
                if ( eqTab[tempList[j]-1][k] && !labelFlag[k] )
                {
                    tempList.push_back(k+1);
                    labelFlag[k] = equaList.size() + 1;
                }
            }
        }
        equaList.push_back(tempList);
        tempList.clear();
    }

	printf( "��ͨ����Ŀ:%d\n", equaList.size() );

    for ( vector<int>::size_type i=0; i!=runLabels.size(); i++ )
        runLabels[i] = labelFlag[runLabels[i]-1];
}
int cvPic::doColor(int iColor)
{
	return 1;
}
vector<LPPACKAGE> cvPic::getPackages(int imgWidth,int imgHeight, vector<int>& imgX, vector<int>& imgY, vector<int>& imgColor,CClient *pClient)
{
	vector<LPPACKAGE> packages;
	int PointNum =  (imgWidth) * imgHeight ;
	//PointNum = 10*imgWidth;
	tagPrinterData *ptagPrinterData = new tagPrinterData[PointNum];
	int Cnt = 0;
	for(int i=0;i<imgHeight;i++)
	{
		if(i%2==0)
			for(int j=0;j<imgWidth;j++)
			{
				if(imgColor[i*imgWidth+j]!=((255 << 16) | (255<< 8) | 255))
				{
					ptagPrinterData[Cnt].XAxis = imgX[i*imgWidth+j];
					ptagPrinterData[Cnt].YAxis = imgY[i*imgWidth+j];
					ptagPrinterData[Cnt].ZAxis = (-1)*Cnt;
					ptagPrinterData[Cnt].cColorSelect = doColor(imgColor[i*imgWidth+j]);
					Cnt++;
				}
			}
		else
			for(int j=0;j<imgWidth;j++)
			{
				if(imgColor[i*imgWidth+imgWidth-j-1]!=((255 << 16) | (255<< 8) | 255))
				{
					ptagPrinterData[Cnt].XAxis = imgX[i*imgWidth+imgWidth-j-1];
					ptagPrinterData[Cnt].YAxis = imgY[i*imgWidth+imgWidth-j-1];
					ptagPrinterData[Cnt].ZAxis = (-1)*Cnt;
					ptagPrinterData[Cnt].cColorSelect = doColor(imgColor[i*imgWidth+imgWidth-j-1]);
					Cnt++;
				}
			}
	}

	int TrueLen = BODY_LEN-sizeof(tagTaskData);
	int DataInOnePack = TrueLen/sizeof(tagPrinterData);
	int BODY_TRUE_LEN = DataInOnePack*sizeof(tagPrinterData) + sizeof(tagTaskData);
	int PackNum = Cnt/DataInOnePack;
	int i=0;	

	for(i=0;i<PackNum;i++)
	{
		LPPACKAGE ptagPackage = pClient->GetPackageFromMem();
		
		tagTaskData *pTaskData = (tagTaskData*)&ptagPackage->pckBody;
		pTaskData->dwTaskId = TaskId;
		pTaskData->dwPackNum = Cnt%(DataInOnePack)==0?PackNum:PackNum+1;
		pTaskData->dwID = i;
		
		memcpy((char*)&ptagPackage->pckBody + sizeof(tagTaskData),&ptagPrinterData[i*DataInOnePack],DataInOnePack*sizeof(tagPrinterData));
		ptagPackage->pckHeader.dwLength = BODY_TRUE_LEN + sizeof(PACKAGE_HEADER);
		ptagPackage->pckHeader.yType = TAG_PRINT_DATA;
		
		packages.push_back(ptagPackage);
	}
	if(PointNum%(DataInOnePack)!=0)
	{
		LPPACKAGE ptagPackage = pClient->GetPackageFromMem();
		
		tagTaskData *pTaskData = (tagTaskData*)&ptagPackage->pckBody;
		pTaskData->dwTaskId = TaskId;
		pTaskData->dwPackNum = Cnt%(DataInOnePack)==0?PackNum:PackNum+1;
		pTaskData->dwID = i;

		memcpy((char*)&ptagPackage->pckBody + sizeof(tagTaskData),&ptagPrinterData[i*DataInOnePack],(PointNum%(DataInOnePack))*sizeof(tagPrinterData));
		ptagPackage->pckHeader.dwLength = (PointNum%(DataInOnePack))*sizeof(tagPrinterData)+sizeof(tagTaskData) + sizeof(PACKAGE_HEADER);
		ptagPackage->pckHeader.yType = TAG_PRINT_DATA;
		
		packages.push_back(ptagPackage);
	}
	
	return packages;
}
vector<LPPACKAGE> cvPic::getPrintPack(CClient *pClient)
{
	string strSrcImg = ImgName;									// ԴͼƬ��λ��
	int num = 0;															// ��ͨͼ����
	cv::Mat srcImg = cv::imread( strSrcImg );									// ��ͼƬ����
	cv::Mat labelImg;
	vector<int> stRun,enRun,rowRun,runLabels;						
	vector<pair<int,int>> equivalences;

	vector<int> imgX,imgY,imgColor;
	int imgWidth,imgHeight;
	
	//cv::cvtColor( grayImg, labelImg, CV_BGR2GRAY );							// �ҶȻ�ͼ��
	//cv::threshold( labelImg, labelImg, 150, 255, CV_THRESH_BINARY );		// ��ֵ��ͼ��
	labelImg = srcImg;

	packagePictureInfo( labelImg, imgX, imgY, imgColor );
	imgWidth = labelImg.cols;
	imgHeight = labelImg.rows;
	
	//fillRunVectors( labelImg, num, stRun, enRun, rowRun );					// ����ŵı����ȼ۶��б������
	//firstPass( stRun, enRun, rowRun, num, runLabels, equivalences, 1 );		// ���ڽ�
	//replaceSameLabel( runLabels, equivalences );

	printf( "��ͨͼ��ĿΪ:%d\n", num );
	return getPackages(imgWidth,imgHeight,imgX,imgY,imgColor,pClient);
}