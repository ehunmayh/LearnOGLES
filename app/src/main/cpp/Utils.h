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

/**
 * 编译shader源码的方法
 * @param shader_type  shader的类型
 * @param source_code  编译的源码
 * @return
 */
GLuint CompileShader(GLenum shader_type,const char * source_code);

/**
 * 创建一个GPU程序
 * @param vertex_shader
 * @param fragment_shader
 * @return
 */
GLuint CreateProgram(GLuint vertex_shader,GLuint fragment_shader);

/**
 * 一个标准的GPU程序是：先编译，后链接的过程， 这个方法封装了一下上面的这两个方法 CompileShader  CreateProgram
 * @param vertex_shader_path 传入 vertex_shader源码路径
 * @param fragment_shader_path fragment_shader源码路径
 * @return
 */
GLuint CreateStandardProgram(const char *vertex_shader_path,const char fragment_shader_path);