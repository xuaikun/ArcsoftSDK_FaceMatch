// opencv_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
// 总的来说还是环境为：WIN10 + VS2017+ opencv2.4.9好
// 程序还是比较乱的，要加强训练呀
// 思路还是比较好的呢，把文件读到.txt文件中，然后只用读图片位置就好
// 还得多考虑，哪些地方的内存需要释放呢，不让容易内存溢出就麻烦了
// 一定要记得释放内存~~~~~~
// 本程序主要用于基于样本上进行筛图，速度很快~
#include "pch.h"
#include "GetScore.h"
#include "ProcessFolder.h"
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
#include <Windows.h>

using namespace cv;
using namespace std;

// 垃圾图片放置的路径
string strFinalName = "E:\\000007work"; // \laji_new
//生成的图片放置的地方
string strSumName ="E:\\000007work\\Sample";

// 将字符串转换成字符数组
void StringToChar(char *charname, string str)
{
	for (int p = 0; p < str.length(); p++)
	{
		charname[p] = str[p];
	}
}
// 将字符串转换成长整形
long StringToLong(string str)
{
	long x = 0;
	for (int p = 0; p < str.length(); p++)
	{
		x = pow(10, (str.length() - p - 1))*(int)(str[p] - '0') + x;
	}
	return x;
}


int main()
{
	cout << "程序开始运行……" << endl;
	// 定义分数变量
	MFloat Get_Score;
	MFloat confidenceLevel;
	MRESULT res;
	// 返回句柄handle
	MHandle handle = NULL;
	// 初始化
	handle = Init(handle);
	// 定义开始时间和结束时间
	clock_t start, finish;
	start = clock();
	// 体现读取图片
	//主要是把两个文件夹中的图片全部提取出来
	//分成两部分
	//并把文件信息存入文件夹中
	//测试文件在 AlltestFiles.txt中
	//样本文件在AllsampleFiles.txt中
	//第一行为文件的数量
	//每次运行代码前都得做，因为每次都有数据更新
	cout << "统计文件数量" << endl;
	for (int i = 0; i < 2; i++)
	{
		processingTotal(i);
	}
	cout << "统计完毕" << endl;
	Sleep(1);//延时休息
	cout << "开始比较图片……" << endl;
	
	string testname, testnameNum, samplename, samplenameNum;
	string samplename_new[1000];
	ifstream intestfile, intestfileNum, insamplefile, insamplefileNum;
	intestfileNum.open("AlltestFilesNum.txt", ios::in);
	getline(intestfileNum, testnameNum, '\n');
	cout << "testnameNum = " << testnameNum << endl;
	insamplefileNum.open("AllsampleFilesNum.txt", ios::in);
	getline(insamplefileNum, samplenameNum, '\n');
	cout << "samplenameNum = " << samplenameNum << endl;
	intestfileNum.close();
	insamplefileNum.close();
	
	long testNum = StringToLong(testnameNum);
	long sampleNum = StringToLong(samplenameNum);
	cout << "testNum =" << testNum << endl;
	cout << "sampleNum =" << sampleNum << endl;

	// 定义用于保存n个样本特征的数组
	ASF_FaceFeature sample_feature[100] = {0};
	// 定义用于保存测试图片特征的临时变量
	ASF_FaceFeature test_feature = {0};

	// 提取n个样本的特征
	insamplefile.open("AllsampleFiles.txt", ios::in);
	long SecondFolderNum = 0;
	while (!insamplefile.eof())
	{
		SecondFolderNum++;
		if (SecondFolderNum > sampleNum)
		{
			insamplefile.close();
			break;
		}
		getline(insamplefile, samplename, '\n');
		cout << samplename << endl;
		//将n个样本名字保存到一个数组里面，方便后面提取
		samplename_new[SecondFolderNum - 1] = samplename;
		char imgname[100] = {};
		StringToChar(imgname, samplename);
		// 加载样本图片
		IplImage* img = cvLoadImage(imgname);
		// 提取样本特征
		feature_obtain(handle, img, imgname, &sample_feature[SecondFolderNum - 1]);
	}insamplefile.close();
	// 到此位置，sample_feature[]保存了样本的特征
	
	// 开始获取得分
	intestfile.open("AlltestFiles.txt", ios::in);
	long  FisrtFolderNum = 0;
	while (!intestfile.eof())
	{
		FisrtFolderNum++;
		if (FisrtFolderNum > testNum)
		{
			intestfile.close();
			break;
		}
		char testname_new[100] = {};
		getline(intestfile, testname, '\n');
		cout << testname << endl;
		// 对测试图片操作成功
		char fileName1[100] = {}; // 测试图片路径
		StringToChar(testname_new, testname);
		string imgname1_str = testname;
		char imgname1[100] = {};
		IplImage* img1;
		char strSum_new[100] = {};
		char strGoal_new[100] = {};
		// 图片被剪切的标志
		// true: 表示被剪切
		// false: 表示未被剪切
		bool photo_flag = false;

		// 每张图片操作时，先计时一下
		int start_compare = clock();
		StringToChar(imgname1, testname);
		img1 = cvLoadImage(imgname1);
		// 获取测试图片的特征，操作这一步，晒图的速度增加了40倍
		feature_obtain(handle, img1, imgname1, &test_feature);
		
		for(int i = 0; i < sampleNum; i++)
		{
			
			// 以下为第二层文件夹的操作
			char fileName[100] = {};// 样本图片的上上层目录名称
			samplename = samplename_new[i];
			string imgname_str = samplename;
			{
				
				//获取文件的名字，不带前面路径
				
				// Num 主要统计 这个 目录的最后一级的名称，不带路径，仅为名称 的字符的个数
				int Num = 0;
				// 从这个图片路径的最后一个字符开始，统计到当前目录的上一层目录时，停止统计
				for (int p = imgname1_str.length() - 1; p >= 0; p--)
				{
					//cout << "str1[p] =" << str1[p] << endl;
					if (imgname1_str[p] != '\\')
					{
						//cout << "str1[p] =" << str1[p] << endl;
						Num = Num + 1;
					}
					else
						break;
				}
				// 从文件的名字的第一个字符开始赋值给一个字符数组进行保存
				for (int p = imgname1_str.length() - Num, j = 0; p < imgname1_str.length(); p++, j++)
				{
					fileName1[j] = imgname1_str[p];
				}
				// 打印生成的图片名称
				//cout << "fileName = " << fileName << endl;
				// 获取到的文件的名称为fileName

				// 第一层文件夹操作完毕

				//for (int iNum2 = 0; iNum2 < imgNum2; iNum2++)
				{
					// 获取两张图片比较的分数
					res = ASFFaceFeatureCompare(handle, &sample_feature[i], &test_feature, &confidenceLevel);
					if (res != MOK)
					{
						// 图片异常
						printf("ASFFaceFeatureCompare fail: %d\n", res);
						Get_Score = -1;
					}
					else
					{
						// 图片正常
						Get_Score = confidenceLevel;
					}
					// 将得分扩大100倍 它的值：0<= Get_Score <= 100
					//在接口里面已经释放图片占用的内存，外面已经不用再重复申请了
					
					Get_Score = Get_Score * 100;
					// cout << "Score = " << Get_Score << endl;
					//https://www.cnblogs.com/tgyf/p/3839894.html 查看链接

					// 从这个图片路径的最后一个字符开始，统计到当前目录的上上（不是上）一层目录时，停止统计
					// 这个路径用于作为筛选出来的根文件夹
					// 计数每次初始化为0
					int Num1 = 0;
					for (int p = imgname_str.length() - 1; p >= 0; p--)
					{
						//cout << "str1[p] =" << str1[p] << endl;
						if (imgname_str[p] != '\\')
						{
							//cout << "str1[p] =" << str1[p] << endl;
							Num1 = Num1 + 1;
						}
						else
						{
							for (int q = p - 1; q >= 0; q--)
							{
								if (imgname_str[q] != '\\')
								{
									Num1 = Num1 + 1;
								}
								else
									break;
							}
							Num1 = Num1 + 1;
							break;
						}
					}

					// 从文件的名字的第一个字符开始赋值给一个字符数组进行保存
					for (int p = imgname_str.length() - Num1, j = 0; p < imgname_str.length(); p++, j++)
					{
						if (imgname_str[p] != '\\')
						{
							fileName[j] = imgname_str[p];
						}
						else
						{
							break;
						}

					}
					// 打印生成的图片名称 算是根目录的名字
					//cout << "fileName1 = " << fileName1 << endl;
					// 获取到的文件的名称为fileName

					string strSum = strSumName; //"E:\\000007work\\Sample";
					
					StringToChar(strSum_new, strSum);
					CreateDirectoryA((LPCSTR)strSum_new, NULL);
					string strGoal = strSum + "\\" + fileName;
					// 到这里了

					StringToChar(strGoal_new, strGoal);
					CreateDirectoryA((LPCSTR)strGoal_new, NULL);
					//已经获得两张图片的比较得分，根据分数的大小可以对图片进行分类
					int min_compare = clock();
					if ((Get_Score >= 55) && (Get_Score < 60))
					{
						//以下是对测试文件的剪切操作，目前是成功了
						// 定义目标路径
						//cout << "******************55_66 begin************" << endl;
						string strFinal = strGoal + "\\" + "55_60";
						char Create_path[100] = {};
						StringToChar(Create_path, strFinal);
						//判断路径是否存在，不存在则创建路径
						CreateDirectoryA((LPCSTR)Create_path, NULL);
						//目标路径和图片名称组合成新的路径
						string str_new = strFinal + "\\" + fileName1;
						char pathName[100] = {};

						StringToChar(pathName, str_new);
						// 剪切文件到指定的文件夹
						// 文件剪切操作 成功
						MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

						// 将图片剪切到55_60文件夹中
						cout << strFinal << endl;
						// 图片剪切到了指定文件夹，跳出循环
						photo_flag = true;
						break;
					}

					else if ((Get_Score >= 60) && (Get_Score < 70))
					{
						// 定义目标路径
						//cout << "******************60_70 begin************" << endl;
						string strFinal = strGoal + "\\" + "60_70";
						char Create_path[100] = {};
						StringToChar(Create_path, strFinal);
						//判断路径是否存在，不存在则创建路径
						CreateDirectoryA((LPCSTR)Create_path, NULL);
						//目标路径和图片名称组合成新的路径
						string str_new = strFinal + "\\" + fileName1;
						char pathName[100] = {};

						StringToChar(pathName, str_new);
						// 剪切文件到指定的文件夹
						// 文件剪切操作 成功
						MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

						// 将图片剪切到60_70文件夹中
						cout << strFinal << endl;
						// 图片剪切到了指定文件夹，跳出循环
						photo_flag = true;
						break;
					}
					else if ((Get_Score >= 70) && (Get_Score < 80))
					{
						// 定义目标路径
						//cout << "******************70_80 begin************" << endl;
						string strFinal = strGoal + "\\" + "70_80";
						char Create_path[100] = {};
						StringToChar(Create_path, strFinal);
						//判断路径是否存在，不存在则创建路径
						CreateDirectoryA((LPCSTR)Create_path, NULL);
						//目标路径和图片名称组合成新的路径
						string str_new = strFinal + "\\" + fileName1;
						char pathName[100] = {};

						StringToChar(pathName, str_new);
						// 剪切文件到指定的文件夹
						// 文件剪切操作 成功
						MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

						// 将图片剪切到70_80文件夹中
						cout << strFinal << endl;
						// 图片剪切到了指定文件夹，跳出循环
						photo_flag = true;
						break;
					}
					else if (Get_Score >= 80)
					{
						// 定义目标路径
						//cout << "******************80_100 begin************" << endl;
						string strFinal = strGoal + "\\" + "80_100";
						char Create_path[100] = {};
						StringToChar(Create_path, strFinal);
						//判断路径是否存在，不存在则创建路径
						CreateDirectoryA((LPCSTR)Create_path, NULL);
						//目标路径和图片名称组合成新的路径
						string str_new = strFinal + "\\" + fileName1;
						char pathName[100] = {};

						StringToChar(pathName, str_new);
						// 剪切文件到指定的文件夹
						// 文件剪切操作 成功
						MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

						// 将图片剪切到80_100文件夹中
						cout << strFinal << endl;
						photo_flag = true;
						// 图片剪切到了指定文件夹，跳出循环
						break;
					}
					//异常处理
					else if((0 > Get_Score) || (100 < Get_Score))
					{
						//做异常处理
						cout << "###############图片出现异常##################" << endl;
						// 定义目标路径
						string strFinal = strFinalName;
						string strFinal_new = strFinal + "\\" + "laji_new";
						char Create_path[100] = {};
						StringToChar(Create_path, strFinal_new);
						//判断路径是否存在，不存在则创建路径
						CreateDirectoryA((LPCSTR)Create_path, NULL);
						//目标路径和图片名称组合成新的路径
						string str_new = strFinal_new + "\\" + fileName1;
						char pathName[100] = {};
						StringToChar(pathName, str_new);
						// 剪切文件到指定的文件夹
						// 文件剪切操作 成功
						MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

						//将这张图片剪切到垃圾文件夹中
						cout << strFinal_new << endl;
						photo_flag = true;
						break;
					 }
				}
				
			}
			//继续查找，直到所有图片都被遍历完全	
		}
		// 表示至始至终，测试图片没有被放入任何一张图片
		if (photo_flag == false)
		// 这里表示从测试文件夹中抽出的照片不属于任何一个样本文件夹，只能把该图片剪切到laji文件夹
		{
			// 定义目标路径
			cout << "没有匹配的样本" << endl;
			string strFinal = strFinalName;
			string strFinal_new = strFinal + "\\" + "laji_new";
			//cout << "strFinal_new = " << strFinal_new << endl;
			char Create_path[100] = {};
			StringToChar(Create_path, strFinal_new);
			//判断路径是否存在，不存在则创建路径
			CreateDirectoryA((LPCSTR)Create_path, NULL);
			//目标路径和图片名称组合成新的路径
			string str_new = strFinal_new + "\\" + fileName1;
			char pathName[100] = {};
			StringToChar(pathName, str_new);
			// 剪切文件到指定的文件夹
			// 文件剪切操作 成功
			MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

			//将这张图片剪切到垃圾文件夹中
			cout << strFinal_new << endl;
		}
		int end_compare = clock();
		cout << "操作一张图片需要的时间为"<< (double)(end_compare - start_compare) / CLOCKS_PER_SEC << "s" << endl;
	}intestfile.close();
	cout << "图片统计完毕" << endl;
	UnInit(handle);
	finish = clock();
	cout << "time = " << (double)(finish - start) / CLOCKS_PER_SEC << "s" << endl;
	return 0;
}
