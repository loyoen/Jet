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
	// 对图像信息进行打包
	void packagePictureInfo( const Mat& bwImage, vector<int>& imgX, vector<int>& imgY, vector<int>& imgColor );
	// 等待退出
	void waitESC( void );
	// 完成所有团的查找与记录
	void fillRunVectors( const Mat& bwImage, int& NumberOfRuns, vector<int>& stRun, vector<int>& enRun, vector<int>& rowRun );
	// 完成团的标记与等价对列表的生成
	void firstPass( vector<int>& stRun, vector<int>& enRun, vector<int>& rowRun, int NumberOfRuns, vector<int>& runLabels, vector<pair<int,int>>& equivalences, int offset );
	// 处理等价对列表
	void replaceSameLabel( vector<int>& runLabels, vector<pair<int,int>>& equivalence );
	vector<LPPACKAGE> getPackages(int imgWidth,int imgHeight, vector<int>& imgX, vector<int>& imgY, vector<int>& imgColor,CClient *pClient);
	vector<LPPACKAGE> getPrintPack(CClient *pClient);
	int doColor(int iColor);
};