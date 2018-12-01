#include "ProcessFolder.h"
#include "pch.h"
#include "GetScore.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> /*用到了time函数，所以要有这个头文件*/
#include <fstream>
#include <sstream>
#include <exception>
#include <vector>
#include <io.h>

#include <highgui.h> 
#include <cv.h>

using namespace cv;
using namespace std;
//config.txt为要处理的跟目录，在训练model中，config.txt中的内容为训练是的样本库，1-13分类好了。--
//在样本筛选model中，config.txt里的内容为要筛选的样本库，未分类。---
char configFile[100] = "config.txt";
//某个根目录的图片保存在imgNames里面
extern vector<string> imgNames[2];
extern vector<string> imgNames_real[2];
int labelTemp = 0;

//读取config文件里的内容--
char* trainSetPosPath = (char *)malloc(200 * sizeof(char));
void readConfig(char* configFile, char* trainSetPosPath, int i) {
	fstream f;
	char cstring[1000];
	int readS = 0;
	f.open(configFile, fstream::in);
	char param1[200]; strcpy(param1, "");
	char param2[200]; strcpy(param2, "");
	char param3[200]; strcpy(param3, "");

	//--读取第一行：--
	f.getline(cstring, sizeof(cstring));
	readS = sscanf(cstring, "%s %s %s", param1, param2, param3);
	if (i == 0)
	{
		strcpy(trainSetPosPath, param1);
	}
	else if (i == 1)
	{
		strcpy(trainSetPosPath, param2);
	}

}

//遍历config.txt里的根目录下的所有的文件，包括子目录。--
// 其中子目录的名字就是label，子目录里的文件为label对于的训练测试样本---


void dfsFolder(string folderPath, int i) {
	_finddata_t FileInfo;
	string strfind = folderPath + "\\*";
	intptr_t Handle = _findfirst(strfind.c_str(), &FileInfo);
	if (Handle == -1L)
	{
		cerr << "can not match the folder path" << endl;
		exit(-1);
	}
	do {
		//判断是否有子目录--
		if (FileInfo.attrib & _A_SUBDIR) {
			//  cout<<FileInfo.name<<" "<<FileInfo.attrib<<endl;
			//这个语句很重要--
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0)) {
				string newPath = folderPath + "\\" + FileInfo.name;
				//cout << FileInfo.name << " " << newPath << endl;
				//根目录下下的子目录名字就是label名，如果没有子目录则其为根目录下
				labelTemp = atoi(FileInfo.name);
				//  printf("%d\n",labelTemp);
				dfsFolder(newPath, i);
			}
		}
		else {
			string finalName = folderPath + "\\" + FileInfo.name;
			string finalName_new = FileInfo.name;
			//将所有的文件名写入一个txt文件--
			//  cout << FileInfo.name << "\t";
			//  printf("%d\t",label);
			//  cout << folderPath << "\\" << FileInfo.name  << " " <<endl;
			//将文件名字和label名字（子目录名字赋值给向量）--
			
			//cout << "finalName_new  = " << finalName_new << endl;
			imgNames[i].push_back(finalName);
			imgNames_real[i].push_back(finalName_new);
		}
	} while (_findnext(Handle, &FileInfo) == 0);
	_findclose(Handle);

}

void initTrainImage(char *configFileName, int i) {
	readConfig(configFileName, trainSetPosPath, i);

	string folderPath = trainSetPosPath;
	dfsFolder(folderPath, i);
}


//批量读取config.txt里面的文件识别
void processingTotal(int i) {
	initTrainImage(configFile, i);
	if (i == 0)
	{
		char distAll[100] = "AlltestFiles.txt";
		char distAll_new[100] = "AlltestFilesNum.txt";
		ofstream ofn(distAll);
		ofstream ofn_new(distAll_new);
		int imgNum = imgNames[i].size();
		ofn_new << imgNum << endl;
		for (int iNum = 0; iNum < imgNum; iNum++) {

			//cout << endl << iNum << endl;
			//cout << imgNames[i][iNum].c_str() << endl;
			ofn << imgNames[i][iNum] << endl;
			IplImage * src = cvLoadImage(imgNames[i][iNum].c_str(), 1);
			if (!src) continue;
			else {
				cvReleaseImage(&src);
			}
			//processing-------------

		}
		ofn_new.close();
		ofn.close();
	}
	if (i == 1)
	{
		char distAll[100] = "AllsampleFiles.txt";
		char distAll_new[100] = "AllsampleFilesNum.txt";
		ofstream ofn(distAll);
		ofstream ofn_new(distAll_new);
		int imgNum = imgNames[i].size();
		ofn_new << imgNum << endl;
		for (int iNum = 0; iNum < imgNum; iNum++) {

			//cout << endl << iNum << endl;
			//cout << imgNames[i][iNum].c_str() << endl;
			ofn << imgNames[i][iNum] << endl;
			IplImage * src = cvLoadImage(imgNames[i][iNum].c_str(), 1);
			if (!src) continue;
			else {
				cvReleaseImage(&src);
			}
			//processing-------------

		}
		ofn.close();
	}

	
}