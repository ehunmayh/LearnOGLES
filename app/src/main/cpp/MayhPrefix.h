//
// Created by 马彦虎 on 2021/2/8.
//  此处可以全局配置需要的头文件
//
#pragma once

#include <iostream>
#include <stdio.h>
#include <jni.h>
#include <string.h>
//引入 opengles 的库
#include <GLES2/gl2.h>
//android 打印日志的库
#include <android/log.h>

//引入Android AssetManager库，操作Android Assets目录下的资源
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

//引入c的时间
#include <time.h>
//c++string
#include <string>

//引入OpenGl的数学库，因为opengl是一个3D编程，肯定会碰到相应:学问题，没必要自己实现这些，直接引入这个库即可
#include "Glm/ext.hpp"
#include "Glm/ext.hpp"

//定义日志的tag
#define LOG_TAG "mayh.so_log"
