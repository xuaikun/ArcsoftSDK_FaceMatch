#ifndef ProcessFolder_H
#define ProcessFolder_H

vector<string> imgNames[2];
vector<string> imgNames_real[2];

void readConfig(char* configFile, char* trainSetPosPath, int i);
void dfsFolder(string folderPath, int i);
void initTrainImage(char *configFileName, int i);
void processingTotal(int i);

#endif