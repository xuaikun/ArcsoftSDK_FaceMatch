#ifndef GetScore_H
#define GetScore_H

#include "amcomdef.h"
#include "arcsoft_face_sdk.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "merror.h"
#include <direct.h> 
#include <iostream>  
#include <stdarg.h>
#include <string>
#include <opencv.hpp>
using namespace cv;
using namespace std;

#pragma comment(lib, "libarcsoft_face_engine.lib")

#define APPID "3oHkvk7Wh9N3aneiXxyBzR71umuBYxUW2L69DQRuJtBF"
#define SDKKey "7bscFFKX1vHhWYmWzdkPpkkj8x1YpwtoYKoMD9tvZnfG"	

#define SafeFree(p) { if ((p)) free(p); (p) = NULL; }
#define SafeArrayDelete(p) { if ((p)) delete [] (p); (p) = NULL; } 
#define SafeDelete(p) { if ((p)) delete (p); (p) = NULL; } 
/****************************************************
功能：初始化引擎
输入：句柄handle
输出：返回句柄handle
****************************************************/
MHandle Init(MHandle handle);
/****************************************************
功能：返回两张图片比较后的得分
输入:两张图片
输出：得分
*****************************************************/
MFloat Score(MHandle handle, IplImage* img, IplImage* img1, char* imgname, char* imgname1);
/****************************************************
功能：反初始化引擎
输入：None
输出：None
****************************************************/
void UnInit(MHandle handle);

#endif