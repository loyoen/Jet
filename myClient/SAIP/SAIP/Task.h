#ifndef TASK_H
#define TASK_H
#include "package.h"
#include <vector>
using namespace std;
struct Task
{
public:
	int							TaskId;
	//vector<LPPACKAGE>	DataPackages;
	vector<LPPACKAGE>	PicPackages;
	Task*				next;
};

#endif