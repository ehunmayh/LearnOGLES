//
// Created by 马彦虎 on 2021/2/8.
//
#include "Sence.h"
extern "C" JNIEXPORT jstring JNICALL
Java_com_myh_learnogles_MainActivity_stringFromSence(JNIEnv *env, jobject){
    std::string hello = "Hello from Sence.cpp C++";
    return  env->NewStringUTF(hello.c_str());
}