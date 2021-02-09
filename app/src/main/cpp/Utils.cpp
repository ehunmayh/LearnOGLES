//
// Created by 马彦虎 on 2021/2/8.
//
#include "Utils.h"
/**
 * ndk 访问Android系统的assets目录下的资源文件
 * @param aAssetManager
 * @param path 文件名
 * @param fileSize 文件大小
 * @return 读取文件的内容返回
 */
unsigned  char * loadFileContent(AAssetManager *aAssetManager,const  char *path,int &fileSize){
    fileSize=0;
    unsigned char * fileContent= nullptr;
    AAsset *aAsset=AAssetManager_open(aAssetManager,path,AASSET_MODE_UNKNOWN);
    if(aAsset!= nullptr){
        //有文件获取文件内容的长度
        fileSize=AAsset_getLength(aAsset);
        //开辟空间, 空间大小为内容的长度+1，个人习惯用于存放\n
        fileContent=new unsigned char [fileSize+1];
        //将aAsset中的内容读取到fileContent中 ,读取的长度fileSize
        AAsset_read(aAsset,fileContent,fileSize);
        //最后一位写0
        fileContent[fileSize]=0;
        //记得释放资源
        AAsset_close(aAsset);
    }
    return fileContent;
}