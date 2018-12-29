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
void feature_obtain(MHandle handle, IplImage* img, char* imgname, LPASF_FaceFeature my_feature)
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
	if (img->width % 4 != 0)
	{
		img->width = img->width / 4 * 4;
		img_Flag = false;
	}
	//���߶Ȳ�Ϊ2�ı���ʱ������
	if (img->height % 2 != 0)
	{
		img->height = img->height / 2 * 2;
		img_Flag = false;
	}

	if (img_Flag == false)
	{
		Mat img_new = imread(imgname);
		resize(img_new, img_new, Size(img->width, img->height), 0, 0, CV_INTER_LINEAR);
		imwrite(imgname, img_new);
		// �Ҿ����������ͷţ���ǰ�ͷ���һ�ε��ڴ棬������һ�Σ����ظ������ʱ�򣬵��������ڴ棬�ڴ�ռ���������
		cvReleaseImage(&img);
		// ���¼���ͼƬ
		img = cvLoadImage(imgname);
	}
	MFloat confidenceLevel;
	// �Ա�����ͼƬ�����Ƴ̶�
	//��ΪͼƬ�Ŀ�Ⱥ͸߶��Ѿ�������ˣ���ܶ��쳣����ɾ��
	if (img)
	{
		ASF_MultiFaceInfo detectedFaces1 = { 0 };
		ASF_SingleFaceInfo SingleDetectedFaces1 = { 0 };
		ASF_FaceFeature feature1 = { 0 };
		//ASF_FaceFeature copyfeature1 = { 0 };
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
				my_feature->featureSize = feature1.featureSize;
				my_feature->feature = (MByte *)malloc(feature1.featureSize);
				memset(my_feature->feature, 0, feature1.featureSize);
				memcpy(my_feature->feature, feature1.feature, feature1.featureSize);
			}
			//else
				//printf("ASFFaceFeatureExtract 1 fail: %d\n", res);
		}
		//else
			//printf("ASFFaceFeatureExtract 1 fail: %d\n", res);
		
		//else
			//printf("ASFFaceFeatureCompare sucess: %lf\n", confidenceLevel);
		/*my_feature->featureSize = copyfeature1.featureSize;
		my_feature->feature = (MByte *)malloc(copyfeature1.featureSize);
		memset(my_feature->feature, 0, copyfeature1.featureSize);
		memcpy(my_feature->feature, copyfeature1.feature, copyfeature1.featureSize);
		SafeFree(copyfeature1.feature);		//�ͷ��ڴ�*/
	}
	// �����Ƿ�������⣬��ֱ���������ͷ�cvloadImage
	cvReleaseImage(&img);
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