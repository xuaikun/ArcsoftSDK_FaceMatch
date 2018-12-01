# ArcsoftSDK_FaceMatch
### 利用虹软（ArcSoft）人脸识别SDK，进行筛图
### 在这个程序中使用了文件批量处理，参考这个： 好生看看这篇博客： https://blog.csdn.net/zhubenfulovepoem/article/details/51204551  这篇博客若使用win10 vs2017 则注意
###  char configFile[100] = "config.txt"; intptr_t Handle = _findfirst(strfind.c_str(), &FileInfo); 两个地方。
### 环境：win10 vs2017 opencv 2.4.9

### 想运用虹软的人脸识别sdk需要提前安装opencv
## opencv下载链接：https://opencv.org/releases.html  下载opencv 2.4.9 WinPack 运行解压就好 非常棒
## 解决opencv 在window10和vs2017上的一些列操作问题：https://blog.csdn.net/mars_xiaolei/article/details/78759041
## 解决调用虹软的人脸识别sdk的方案，首先注册账号并下载离线sdk http://ai.arcsoft.com.cn/ucenter/resource/openPlatform/index.html?r=0.8054211301391108 ，安装如链接：
## http://ai.arcsoft.com.cn/manual/arcface_windows_guideV2.html
## 运行例子出现这个错误：error C2664，例子中有一个地方被修改了，记得修改 是MERR_ASF_ALREADY_ACTIVATED， 而不是MERR_ASF_BASE_ALREADY_ACTIVATED
## 可以直接强转类型解决：
## MRESULT res = ASFActivation((MPChar)(APPID), (MPChar)(SDKKey));
## 利用虹软sdk时，得特别注意图片宽度（4的倍数且不为0）和高度（2的倍数）解决方式：wigth = width/4*4  height = height/2*2
