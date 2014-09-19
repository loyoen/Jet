#include "highgui.h"
#include "cv.h"
#include "package.h"
#include "cclient.h"
#include <vector>
using namespace cv;
using namespace std;
class cvPic
{
public:
	LPPACKAGE mpPackages;
	int				TaskId;
	string			ImgName;
	cvPic(int id,string srcImg);
	// ��ͼ����Ϣ���д��
	void packagePictureInfo( const Mat& bwImage, vector<int>& imgX, vector<int>& imgY, vector<int>& imgColor );
	// �ȴ��˳�
	void waitESC( void );
	// ��������ŵĲ������¼
	void fillRunVectors( const Mat& bwImage, int& NumberOfRuns, vector<int>& stRun, vector<int>& enRun, vector<int>& rowRun );
	// ����ŵı����ȼ۶��б������
	void firstPass( vector<int>& stRun, vector<int>& enRun, vector<int>& rowRun, int NumberOfRuns, vector<int>& runLabels, vector<pair<int,int>>& equivalences, int offset );
	// ����ȼ۶��б�
	void replaceSameLabel( vector<int>& runLabels, vector<pair<int,int>>& equivalence );
	vector<LPPACKAGE> getPackages(int imgWidth,int imgHeight, vector<int>& imgX, vector<int>& imgY, vector<int>& imgColor,CClient *pClient);
	vector<LPPACKAGE> getPrintPack(CClient *pClient);
	int doColor(int iColor);
};