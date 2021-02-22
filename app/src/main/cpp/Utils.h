//
// Created by 马彦虎 on 2021/2/8.
//
#pragma once
#include "MayhPrefix.h"

//opengl es 画三角形： 定义顶点数据的结构体用于存放 ：
struct Vertice{
    float mPosition[4];//顶点数据的：x轴,y轴,z轴,w（齐次坐标）的
};

//定义加载文件的方法
unsigned char * loadFileContent(AAssetManager  *aAssetManager,const  char *path,int &fileSize);

float GetFrameTime();
