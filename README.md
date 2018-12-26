# ArcsoftSDK_FaceMatch 利用虹软（ArcSoft）人脸识别SDK，进行筛图 已经成功，以后就是用它了，过两天去公司要将它进一步优化
### 如果只是读取文件名字的话，不需要将图片加载，这样读取文件的速度将大大提升，在修改图片的时候，修改完后，应该先释放图片，然后再加载图片到内存里面，如果没有释放将会一直积累，导致内存溢出
# 换了电脑路径是会变的，一定一定要记得修改，不然程序无法运行~接下来的两个链接就是解决路径问题的方法
##  一般就是这3个地方的路径需要修改：
### （1）属性页，VC++目录 ：包含目录、库目录(与opencv有关)
### （2）属性页，C/C++常规：附加包含目录（与vs工程头文件包含有关）
### （3）属性页，链接器常规：附加库目录（与vs工程lib静态库有关）
# 大家使用我的程序前，先解决您当前IDE的环境路径
## 解决opencv 在window10和vs2017上的一些列操作问题：https://blog.csdn.net/mars_xiaolei/article/details/78759041  主要是路径的导入问题
## http://ai.arcsoft.com.cn/manual/arcface_windows_guideV2.html  路径导入问题

### 在这个程序中使用了文件批量处理，参考这个： 好生看看这篇博客： https://blog.csdn.net/zhubenfulovepoem/article/details/51204551  这篇博客若使用win10 vs2017 则注意
###  char configFile[100] = "config.txt"; intptr_t Handle = _findfirst(strfind.c_str(), &FileInfo); 两个地方。
### 环境：win10 vs2017 opencv 2.4.9

### 想运用虹软的人脸识别sdk需要提前安装opencv
## opencv下载链接：https://opencv.org/releases.html  下载opencv 2.4.9 WinPack 运行解压就好 非常棒
## 解决opencv 在window10和vs2017上的一些列操作问题：https://blog.csdn.net/mars_xiaolei/article/details/78759041  主要是路径的导入问题
## 解决调用虹软的人脸识别sdk的方案，首先注册账号并下载离线sdk http://ai.arcsoft.com.cn/ucenter/resource/openPlatform/index.html?r=0.8054211301391108 ，安装如链接：
## http://ai.arcsoft.com.cn/manual/arcface_windows_guideV2.html  路径导入问题
## 运行例子出现这个错误：error C2664，例子中有一个地方被修改了，记得修改 是MERR_ASF_ALREADY_ACTIVATED， 而不是MERR_ASF_BASE_ALREADY_ACTIVATED
## 可以直接强转类型解决：
## MRESULT res = ASFActivation((MPChar)(APPID), (MPChar)(SDKKey));
## 利用虹软sdk时，得特别注意图片宽度（4的倍数且不为0）和高度（2的倍数）解决方式：wigth = width/4x4  height = height/2x2

### opencv_test.cpp 为程序的主程序，运行从这个程序开始
### GetScore.cpp 主要获取图片特征
### ProcessFolder.cpp 这个文件主要是批量获取图片的名字
### 对于config.txt 的解释，第一个路径为测试图片的根目录 第二个路径为样本图片的根目录（每个样本图片应该只有一张（即每个人只要一张图片））
### AllsampleFiles.txt 保存所有样本图片路径
### AllsampleFilesNum.txt 保存样本图片的总数
### AlltestFiles.txt  保存所有测试图片的路径
###  AlltestFilesNum.txt  保存测试图片的总数 

### 提前将图片的特征提取出来再操作，整个程序的速度都会加快~
