// opencv_test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
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
string strFinalName = "E:\\000007work";
//生成的图片放置的地方
string strSumName ="E:\\000007work\\Sample";

extern vector<string> imgNames[2];
extern vector<string> imgNames_real[2];
// 将字符串转换成字符数组
void StringToChar(char *charname, string str)
{
	for (int p = 0; p < str.length(); p++)
	{
		charname[p] = str[p];
	}
}
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
	MFloat Get_Score;
	// 返回句柄handle
	MHandle handle = NULL;
	handle = Init(handle);
	/*这里的工作主要是将图片从文件夹中提取出来*/
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

	intestfile.open("AlltestFiles.txt", ios::in);
	long FisrtFolderNum = 0;
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
		//第一层for循环表示从测试文件夹中按顺序抽取一张测试图片
		//cout << endl << iNum1 << endl;
		//cout << imgNames[0][iNum1].c_str() << endl;
		// 对测试图片操作成功
		//string str1 = imgNames[0][iNum1].c_str();
		//string imgname_str = imgNames[0][iNum1].c_str();
		char fileName[100] = {}; // 测试图片路径
		StringToChar(testname_new, testname);
		insamplefile.open("AllsampleFiles.txt", ios::in);
		long SecondFolderNum = 0;
		while(!insamplefile.eof())
		{
			SecondFolderNum++;
			if (SecondFolderNum > sampleNum )
			{
				insamplefile.close();
				break;
			}
			getline(insamplefile, samplename, '\n');
			cout << samplename << endl;

			
			// 以下为第二层文件夹的操作
			char fileName1[100] = {};// 样本图片的上上层目录名称
					// 定义两个长度为1000的字串数组，用于保存两张图片路径及名字
			//IplImage * src = cvLoadImage(imgNames[1][iNum].c_str(), 1);
			//if (!src) continue;
			//第二层for循环表示从样本文件夹中按顺序抽取一张样本图片，
			//测试图片依次比较样本图片
			//获取图片数量
			//int imgNum1 = imgNames[0].size();
			//int imgNum2 = imgNames[1].size();
			//cout << "imgNum1 =" << imgNum1 << endl;
			//cout << "imgNum2 =" << imgNum2 << endl;
			//for (int iNum1 = 0; iNum1 < imgNum1; iNum1++)
			{
				string imgname_str = testname;
				//获取文件的名字，不带前面路径
				
				// Num 主要统计 这个 目录的最后一级的名称，不带路径，仅为名称 的字符的个数
				int Num = 0;
				// 从这个图片路径的最后一个字符开始，统计到当前目录的上一层目录时，停止统计
				for (int p = imgname_str.length() - 1; p >= 0; p--)
				{
					//cout << "str1[p] =" << str1[p] << endl;
					if (imgname_str[p] != '\\')
					{
						//cout << "str1[p] =" << str1[p] << endl;
						Num = Num + 1;
					}
					else
						break;
				}
				// 从文件的名字的第一个字符开始赋值给一个字符数组进行保存
				for (int p = imgname_str.length() - Num, j = 0; p < imgname_str.length(); p++, j++)
				{
					fileName[j] = imgname_str[p];
				}
				// 打印生成的图片名称
				//cout << "fileName = " << fileName << endl;
				// 获取到的文件的名称为fileName

				// 第一层文件夹操作完毕

				//for (int iNum2 = 0; iNum2 < imgNum2; iNum2++)
				{
					char imgname[100] = {};
					char imgname1[100] = {};
					//cout << endl << iNum2 << endl;
					//cout << imgNames[1][iNum2].c_str() << endl;
					//IplImage * src = cvLoadImage(imgNames[1][iNum].c_str(), 1);
					//if (!src) continue;
					//将两张图片的路径及名字分别保存到字符串里面
					//string imgname1_str = imgNames[1][iNum2].c_str();
					string imgname1_str = samplename;
					//将两个字符串依次保存到字符数组中，未修改的地方已经初始化为无
					//通过此操作可以分别获得两张图片的路径和名字
					StringToChar(imgname, testname);
					StringToChar(imgname1, samplename);

					//已经获取到两张图片，并提取出来，并通过此去获取它们的比较得分
					IplImage* img = cvLoadImage(imgname);
					IplImage* img1 = cvLoadImage(imgname1);
					//cout << "**********************" << endl;
					//cout << "handle = " << handle << endl;
					//cout << "img = " << img << endl;
					//cout << "img1 = " << img1 << endl;
					//cout << "testname = " << testname << endl;
					//cout << "samplename = " << samplename << endl;
					//cout << "imgname = " << imgname << endl;
					//cout << "imgname1 = " << imgname1 << endl;
					//E:\000007work\img\3\80_100\m.07jz2h_22 - FaceId - 0.jpg
					//E:\000007work\base\1\f.jpg

					// 输入句柄handle，输入两张图片及路径名字，可以返回两张图片比较后的得分Score
					Get_Score = Score(handle, img, img1, imgname, imgname1);
					// 将得分扩大100倍 它的值：0<= Get_Score <= 100
					//释放图片占用的内存
					cvReleaseImage(&img);
					cvReleaseImage(&img1);
					Get_Score = Get_Score * 100;
					cout << "Score = " << Get_Score << endl;
					//https://www.cnblogs.com/tgyf/p/3839894.html 查看链接

					// 从这个图片路径的最后一个字符开始，统计到当前目录的上上（不是上）一层目录时，停止统计
					// 这个路径用于作为筛选出来的根文件夹
					// 计数每次初始化为0
					int Num1 = 0;
					for (int p = imgname1_str.length() - 1; p >= 0; p--)
					{
						//cout << "str1[p] =" << str1[p] << endl;
						if (imgname1_str[p] != '\\')
						{
							//cout << "str1[p] =" << str1[p] << endl;
							Num1 = Num1 + 1;
						}
						else
						{
							for (int q = p - 1; q >= 0; q--)
							{
								if (imgname1_str[q] != '\\')
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
					for (int p = imgname1_str.length() - Num1, j = 0; p < imgname1_str.length(); p++, j++)
					{
						if (imgname1_str[p] != '\\')
						{
							fileName1[j] = imgname1_str[p];
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
					char strSum_new[100] = {};
					char strGoal_new[100] = {};
					StringToChar(strSum_new, strSum);
					CreateDirectoryA((LPCSTR)strSum_new, NULL);
					string strGoal = strSum + "\\" + fileName1;
					// 到这里了

					StringToChar(strGoal_new, strGoal);
					CreateDirectoryA((LPCSTR)strGoal_new, NULL);
					//cout << "strGoal = " << strGoal << endl;
					//已经获得两张图片的比较得分，根据分数的大小可以对图片进行分类
					if ((Get_Score >= 55) && (Get_Score < 60))
					{
						//以下是对测试文件的剪切操作，目前是成功了
						// 定义目标路径
						//cout << "******************55_66 begin************" << endl;
						string strFinal = strGoal + "\\" + "55_60";
						//cout << "strFinal = " << strFinal << endl;
						char Create_path[100] = {};
						StringToChar(Create_path, strFinal);
						//cout << "Create_path = " << Create_path << endl;
						//cout << "******************55_66 ending************" << endl;
						//判断路径是否存在，不存在则创建路径
						CreateDirectoryA((LPCSTR)Create_path, NULL);
						//目标路径和图片名称组合成新的路径
						string str_new = strFinal + "\\" + fileName;
						//cout << "str_new = " << str_new << endl;
						char pathName[100] = {};

						StringToChar(pathName, str_new);
						//cout << "pathName = " << pathName << endl;
						// 剪切文件到指定的文件夹
						// 文件剪切操作 成功
						MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

						// 将图片剪切到55_60文件夹中
						cout << "将图片剪切到55_60文件夹中" << endl;
						// 图片剪切到了指定文件夹，跳出循环
						insamplefile.close();
						break;
					}

					else if ((Get_Score >= 60) && (Get_Score < 70))
					{
						// 定义目标路径
						//cout << "******************60_70 begin************" << endl;
						string strFinal = strGoal + "\\" + "60_70";
						//cout << "strFinal = " << strFinal << endl;
						char Create_path[100] = {};
						StringToChar(Create_path, strFinal);
						//cout << "Create_path = " << Create_path << endl;
						//cout << "******************55_66 ending************" << endl;
						//判断路径是否存在，不存在则创建路径
						CreateDirectoryA((LPCSTR)Create_path, NULL);
						//目标路径和图片名称组合成新的路径
						string str_new = strFinal + "\\" + fileName;
						//cout << "str_new = " << str_new << endl;
						char pathName[100] = {};

						StringToChar(pathName, str_new);
						//cout << "pathName = " << pathName << endl;
						// 剪切文件到指定的文件夹
						// 文件剪切操作 成功
						MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

						// 将图片剪切到60_70文件夹中
						cout << "将图片剪切到60_70文件夹中" << endl;
						// 图片剪切到了指定文件夹，跳出循环
						insamplefile.close();
						break;
					}
					else if ((Get_Score >= 70) && (Get_Score < 80))
					{
						// 定义目标路径
						//cout << "******************70_80 begin************" << endl;
						string strFinal = strGoal + "\\" + "70_80";
						//cout << "strFinal = " << strFinal << endl;
						char Create_path[100] = {};
						StringToChar(Create_path, strFinal);
						//cout << "Create_path = " << Create_path << endl;
						//cout << "******************55_66 ending************" << endl;
						//判断路径是否存在，不存在则创建路径
						CreateDirectoryA((LPCSTR)Create_path, NULL);
						//目标路径和图片名称组合成新的路径
						string str_new = strFinal + "\\" + fileName;
						//cout << "str_new = " << str_new << endl;
						char pathName[100] = {};

						StringToChar(pathName, str_new);
						//cout << "pathName = " << pathName << endl;
						// 剪切文件到指定的文件夹
						// 文件剪切操作 成功
						MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

						// 将图片剪切到70_80文件夹中
						cout << "将图片剪切到70_80文件夹中" << endl;
						// 图片剪切到了指定文件夹，跳出循环
						insamplefile.close();
						break;
					}
					else if (Get_Score >= 80)
					{
						// 定义目标路径
						//cout << "******************80_100 begin************" << endl;
						string strFinal = strGoal + "\\" + "80_100";
						//cout << "strFinal = " << strFinal << endl;
						char Create_path[100] = {};
						StringToChar(Create_path, strFinal);
						//cout << "Create_path = " << Create_path << endl;
						//cout << "******************55_66 ending************" << endl;
						//判断路径是否存在，不存在则创建路径
						CreateDirectoryA((LPCSTR)Create_path, NULL);
						//目标路径和图片名称组合成新的路径
						string str_new = strFinal + "\\" + fileName;
						//cout << "str_new = " << str_new << endl;
						char pathName[100] = {};

						StringToChar(pathName, str_new);
						//cout << "pathName = " << pathName << endl;
						// 剪切文件到指定的文件夹
						// 文件剪切操作 成功
						MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

						// 将图片剪切到80_100文件夹中
						cout << "将图片剪切到80_100文件夹中" << endl;
						insamplefile.close();
						// 图片剪切到了指定文件夹，跳出循环
						break;
					}
					//异常处理
					else if((0 > Get_Score) || (100 < Get_Score))
					{
						//做异常处理
						cout << "图片出现异常" << endl;
						// 定义目标路径
						string strFinal = strFinalName;
						string strFinal_new = strFinal + "\\" + "laji_new";
						//cout << "strFinal_new = " << strFinal_new << endl;
						char Create_path[100] = {};
						StringToChar(Create_path, strFinal_new);
						//cout << "Create_path = " << Create_path << endl;
						//cout << "******************55_66 ending************" << endl;
						//判断路径是否存在，不存在则创建路径
						CreateDirectoryA((LPCSTR)Create_path, NULL);
						//目标路径和图片名称组合成新的路径
						string str_new = strFinal_new + "\\" + fileName;
						//cout << "str_new = " << str_new << endl;
						char pathName[100] = {};
						StringToChar(pathName, str_new);
						//cout << "pathName = " << pathName << endl;
						// 剪切文件到指定的文件夹
						// 文件剪切操作 成功
						MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

						//将这张图片剪切到垃圾文件夹中
						cout << "将图片剪切到laji文件夹中" << endl;
						insamplefile.close();
						break;
					 }
				}
				
			}
			//继续查找，直到所有图片都被遍历完全	
		}insamplefile.close();
		// 这里表示从测试文件夹中抽出的照片不属于任何一个样本文件夹，只能把该图片剪切到laji文件夹
		{
			// 定义目标路径
			//cout << "******************laji begin************" << endl;
			string strFinal = strFinalName;
			string strFinal_new = strFinal + "\\" + "laji_new";
			//cout << "strFinal_new = " << strFinal_new << endl;
			char Create_path[100] = {};
			StringToChar(Create_path, strFinal_new);
			//cout << "Create_path = " << Create_path << endl;
			//cout << "******************55_66 ending************" << endl;
			//判断路径是否存在，不存在则创建路径
			CreateDirectoryA((LPCSTR)Create_path, NULL);
			//目标路径和图片名称组合成新的路径
			string str_new = strFinal_new + "\\" + fileName;
			//cout << "str_new = " << str_new << endl;
			char pathName[100] = {};
			StringToChar(pathName, str_new);
			//cout << "pathName = " << pathName << endl;
			// 剪切文件到指定的文件夹
			// 文件剪切操作 成功
			MoveFileA((LPCSTR)testname_new, (LPCSTR)pathName);

			//将这张图片剪切到垃圾文件夹中
			cout << "将图片剪切到laji文件夹中" << endl;
			insamplefile.close();
		}
	}intestfile.close();
	cout << "图片统计完毕" << endl;
	UnInit(handle);
	finish = clock();
	cout << "time = " << (double)(finish - start) / CLOCKS_PER_SEC << "s" << endl;
	return 0;
}
