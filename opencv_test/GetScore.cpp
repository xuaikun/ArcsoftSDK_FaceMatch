#include "pch.h"
#include "GetScore.h"
MHandle Init(MHandle handle)
{
	//����SDK
	//��APPID��SDKKeyǿת���;���
	//������������SDK��ֻ���һ������cmd����ALActivation sucess:90114 ���ʾSDK�Ѿ�����
	
	//���������������߽��У����Բ����ټ���SDK�����ǻ����ԣ���ϵͳ֮�����������
	MRESULT res;
	/*res  = ASFActivation((MPChar)APPID, (MPChar)SDKKey);
	if (MOK != res && MERR_ASF_ALREADY_ACTIVATED != res)
		printf("ALActivation fail: %d\n", res);
	else
		printf("ALActivation sucess: %d\n", res);
	*/
	//��ʼ������
	
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
	//����ӡ�ߴ�
	//cout << "img->width = "<< img->width << endl;
	//cout << "img->height= " << img->height << endl;
	//cout << "img1->width =" << img1->width << endl;
	//cout << "img1->height=" << img1->height << endl;
	// �������
	// ��������ͼƬ�쳣�ı�־λ
	bool img_Flag = true;
	bool img1_Flag = true;
	//���ͼƬ��С�Ƿ��������������������������ͼƬ�Ŀ�Ⱥ͸߶�
	//����Ȳ�Ϊ4�ı���ʱ����������Ͳ�����0 �����
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
	//���߶Ȳ�Ϊ2�ı���ʱ������
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
	//��ͼƬ��Ȼ�߶ȳ����쳣�����ʵ��޸�ͼƬ�Ŀ�Ⱥ͸߶�
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
	//������¼������ıȽϵ÷�
	MFloat confidenceLevel;
	// �Ա�����ͼƬ�����Ƴ̶�
	//��ΪͼƬ�Ŀ�Ⱥ͸߶��Ѿ�������ˣ���ܶ��쳣����ɾ��
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
				//����feature
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

		//�ڶ���������ȡ����
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

		// �����������ȶ�

		res = ASFFaceFeatureCompare(handle, &copyfeature1, &feature2, &confidenceLevel);
		if (res != MOK)
			printf("ASFFaceFeatureCompare fail: %d\n", res);
		//else
			//printf("ASFFaceFeatureCompare sucess: %lf\n", confidenceLevel);

		SafeFree(copyfeature1.feature);		//�ͷ��ڴ�
	}
	return confidenceLevel;
}
void UnInit(MHandle handle)
{
	MRESULT res;
	//��ȡ�汾��Ϣ
	const ASF_VERSION* pVersionInfo = ASFGetVersion(handle);

	//����ʼ��
	res = ASFUninitEngine(handle);
	//getchar();
}