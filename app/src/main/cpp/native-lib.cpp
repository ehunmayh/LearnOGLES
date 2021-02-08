//引入全局文件库
#include "MayhPrefix.h"

extern "C" JNIEXPORT jstring JNICALL
Java_com_myh_learnogles_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

//  指针  java 对象  当前，  类似this
extern "C" JNIEXPORT jint Java_com_myh_learnogles_MainActivity_Test(
        JNIEnv *env, jobject MainActivity){
        return 100;

}