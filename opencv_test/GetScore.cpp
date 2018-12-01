#include "pch.h"
#include "GetScore.h"
MHandle Init(MHandle handle)
{
	//激活SDK
	//对APPID，SDKKey强转类型就行
	//这里用作激活SDK，只需第一次运行cmd出现ALActivation sucess:90114 则表示SDK已经激活
	
	//后续工作即可离线进行，可以不用再激活SDK，除非换电脑，换系统之类的意外事情
	MRESULT res;
	/*res  = ASFActivation((MPChar)APPID, (MPChar)SDKKey);
	if (MOK != res && MERR_ASF_ALREADY_ACTIVATED != res)
		printf("ALActivation fail: %d\n", res);
	else
		printf("ALActivation sucess: %d\n", res);
	*/
	//初始化引擎
	
	MInt32 mask = ASF_FACE_DETECT | ASF_FACERECOGNITION | ASF_AGE | ASF_GENDER | ASF_FACE3DANGLE;
	res = ASFInitEngine(ASF_DETECT_MODE_IMAGE, ASF_OP_0_ONLY, 16, 5, mask, &handle);
	if (res != MOK)
		printf("ALInitEngine fail: %d\n", res);
	//else
		//printf("ALInitEngine sucess: %d\n",res);
	return handle;
}
MFloat Score(MHandle handle, IplImage* img, IplImage* img1, char* imgname, char* imgname1)
{
	MRESULT res;
	//检查打印尺寸
	//cout << "img->width = "<< img->width << endl;
	//cout << "img->height= " << img->height << endl;
	//cout << "img1->width =" << img1->width << endl;
	//cout << "img1->height=" << img1->height << endl;
	// 人脸检测
	// 设置两张图片异常的标志位
	bool img_Flag = true;
	bool img1_Flag = true;
	//检查图片大小是否合理，合理则跳过，不合理将处理图片的宽度和高度
	//当宽度不为4的倍数时，处理，这里就不考虑0 的情况
	if (img->width % 4 != 4)
	{
		img->width = img->width / 4 * 4;
		img_Flag = false;
	}
	if (img1->width % 4 != 0)
	{
		img1->width = img1->width / 4 * 4;
		img1_Flag = false;
	}
	//当高度不为2的倍数时，处理
	if (img->height % 2 != 0)
	{
		img->height = img->height / 2 * 2;
		img_Flag = false;
	}
	if (img1->height % 2 != 0)
	{
		img1->height = img1->height / 2 * 2;
		img1_Flag = false;
	}
	//cout << "img->width = " << img->width << endl;
	//cout << "img->height= " << img->height << endl;
	//cout << "img1->width =" << img1->width << endl;
	//cout << "img1->height=" << img1->height << endl;
	//如图片宽度或高度出现异常，则适当修改图片的宽度和高度
	if (img_Flag == false)
	{
		Mat img_new = imread(imgname);
		resize(img_new, img_new, Size(img->width, img->height), 0, 0, CV_INTER_LINEAR);
		imwrite(imgname, img_new);
		img = cvLoadImage(imgname);
	}
	if (img1_Flag == false)
	{
		Mat img1_new = imread(imgname1);
		resize(img1_new, img1_new, Size(img1->width, img1->height), 0, 0, CV_INTER_LINEAR);
		imwrite(imgname1, img1_new);
		img1 = cvLoadImage(imgname1);
	}
	//用做记录计算出的比较得分
	MFloat confidenceLevel;
	// 对比两张图片的相似程度
	//因为图片的宽度和高度已经不会错了，则很多异常可以删掉
	if (img && img1)
	{
		ASF_MultiFaceInfo detectedFaces1 = { 0 };
		ASF_SingleFaceInfo SingleDetectedFaces1 = { 0 };
		ASF_FaceFeature feature1 = { 0 };
		ASF_FaceFeature copyfeature1 = { 0 };
		res = ASFDetectFaces(handle, img->width, img->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)img->imageData, &detectedFaces1);
		if (MOK == res)
		{
			SingleDetectedFaces1.faceRect.left = detectedFaces1.faceRect[0].left;
			SingleDetectedFaces1.faceRect.top = detectedFaces1.faceRect[0].top;
			SingleDetectedFaces1.faceRect.right = detectedFaces1.faceRect[0].right;
			SingleDetectedFaces1.faceRect.bottom = detectedFaces1.faceRect[0].bottom;
			SingleDetectedFaces1.faceOrient = detectedFaces1.faceOrient[0];

			res = ASFFaceFeatureExtract(handle, img->width, img->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)img->imageData, &SingleDetectedFaces1, &feature1);
			if (res == MOK)
			{
				//拷贝feature
				copyfeature1.featureSize = feature1.featureSize;
				copyfeature1.feature = (MByte *)malloc(feature1.featureSize);
				memset(copyfeature1.feature, 0, feature1.featureSize);
				memcpy(copyfeature1.feature, feature1.feature, feature1.featureSize);
			}
			//else
				//printf("ASFFaceFeatureExtract 1 fail: %d\n", res);
		}
		//else
			//printf("ASFFaceFeatureExtract 1 fail: %d\n", res);

		//第二张人脸提取特征
		ASF_MultiFaceInfo	detectedFaces2 = { 0 };
		ASF_SingleFaceInfo SingleDetectedFaces2 = { 0 };
		ASF_FaceFeature feature2 = { 0 };
		res = ASFDetectFaces(handle, img1->width, img1->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)img1->imageData, &detectedFaces2);
		if (MOK == res)
		{
			SingleDetectedFaces2.faceRect.left = detectedFaces2.faceRect[0].left;
			SingleDetectedFaces2.faceRect.top = detectedFaces2.faceRect[0].top;
			SingleDetectedFaces2.faceRect.right = detectedFaces2.faceRect[0].right;
			SingleDetectedFaces2.faceRect.bottom = detectedFaces2.faceRect[0].bottom;
			SingleDetectedFaces2.faceOrient = detectedFaces2.faceOrient[0];

			res = ASFFaceFeatureExtract(handle, img1->width, img1->height, ASVL_PAF_RGB24_B8G8R8, (MUInt8*)img1->imageData, &SingleDetectedFaces2, &feature2);
			//if(MOK != res)
				//printf("ASFFaceFeatureExtract 2 fail: %d\n", res);
		}
		//else
			//printf("ASFFaceFeatureExtract 2 fail: %d\n", res);

		// 单人脸特征比对

		res = ASFFaceFeatureCompare(handle, &copyfeature1, &feature2, &confidenceLevel);
		if (res != MOK)
			printf("ASFFaceFeatureCompare fail: %d\n", res);
		//else
			//printf("ASFFaceFeatureCompare sucess: %lf\n", confidenceLevel);

		SafeFree(copyfeature1.feature);		//释放内存
	}
	return confidenceLevel;
}
void UnInit(MHandle handle)
{
	MRESULT res;
	//获取版本信息
	const ASF_VERSION* pVersionInfo = ASFGetVersion(handle);

	//反初始化
	res = ASFUninitEngine(handle);
	//getchar();
}