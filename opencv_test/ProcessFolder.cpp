#include "ProcessFolder.h"
#include "pch.h"
#include "GetScore.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <time.h> /*�õ���time����������Ҫ�����ͷ�ļ�*/
#include <fstream>
#include <sstream>
#include <exception>
#include <vector>
#include <io.h>

#include <highgui.h> 
#include <cv.h>

using namespace cv;
using namespace std;
//config.txtΪҪ����ĸ�Ŀ¼����ѵ��model�У�config.txt�е�����Ϊѵ���ǵ������⣬1-13������ˡ�--
//������ɸѡmodel�У�config.txt�������ΪҪɸѡ�������⣬δ���ࡣ---
char configFile[100] = "config.txt";
//ĳ����Ŀ¼��ͼƬ������imgNames����
extern vector<string> imgNames[2];
extern vector<string> imgNames_real[2];
int labelTemp = 0;

//��ȡconfig�ļ��������--
char* trainSetPosPath = (char *)malloc(200 * sizeof(char));
void readConfig(char* configFile, char* trainSetPosPath, int i) {
	fstream f;
	char cstring[1000];
	int readS = 0;
	f.open(configFile, fstream::in);
	char param1[200]; strcpy(param1, "");
	char param2[200]; strcpy(param2, "");
	char param3[200]; strcpy(param3, "");

	//--��ȡ��һ�У�--
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

//����config.txt��ĸ�Ŀ¼�µ����е��ļ���������Ŀ¼��--
// ������Ŀ¼�����־���label����Ŀ¼����ļ�Ϊlabel���ڵ�ѵ����������---


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
		//�ж��Ƿ�����Ŀ¼--
		if (FileInfo.attrib & _A_SUBDIR) {
			//  cout<<FileInfo.name<<" "<<FileInfo.attrib<<endl;
			//���������Ҫ--
			if ((strcmp(FileInfo.name, ".") != 0) && (strcmp(FileInfo.name, "..") != 0)) {
				string newPath = folderPath + "\\" + FileInfo.name;
				//cout << FileInfo.name << " " << newPath << endl;
				//��Ŀ¼���µ���Ŀ¼���־���label�������û����Ŀ¼����Ϊ��Ŀ¼��
				labelTemp = atoi(FileInfo.name);
				//  printf("%d\n",labelTemp);
				dfsFolder(newPath, i);
			}
		}
		else {
			string finalName = folderPath + "\\" + FileInfo.name;
			string finalName_new = FileInfo.name;
			//�����е��ļ���д��һ��txt�ļ�--
			//  cout << FileInfo.name << "\t";
			//  printf("%d\t",label);
			//  cout << folderPath << "\\" << FileInfo.name  << " " <<endl;
			//���ļ����ֺ�label���֣���Ŀ¼���ָ�ֵ��������--
			
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


//������ȡconfig.txt������ļ�ʶ��
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