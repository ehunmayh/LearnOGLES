//
// Created by 马彦虎 on 2021/2/8.
//通过C++实现 MayhGLSurfaceViewRenderer
//

#include "GlView.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_myh_learnogles_glview_MayhGLSurfaceViewRenderer_initGlView(JNIEnv
* env,
jobject thiz
) {
    //打印日志
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"initGlView");
    //设置颜色：在gl的渲染环境中设置ClearColor状态
   glClearColor(0.1f,0.1,0.8f,1.0f);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_myh_learnogles_glview_MayhGLSurfaceViewRenderer_onSurfaceChange(JNIEnv
* env,
jobject thiz, jint
width,
jint height
) {
    //打印日志
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"onSurfaceChange %dx%d",width,height);
    //ViewPort是gl的另一个状态 ， ViewPort：画布  原点平谬左下角（0，0），，等价Canvas
    glViewport(0,0,width,height);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_myh_learnogles_glview_MayhGLSurfaceViewRenderer_Render(JNIEnv
* env,
jobject thiz
) {

    //打印日志
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"Render");
    //擦出某些东西，  擦出（颜色缓存区）GL_COLOR_BUFFER_BIT
    //GL_DEPTH_BUFFER_BIT 深度缓冲区
    //GL_STENCIL_BUFFER_BIT 模板缓冲
    //open从GL状态机中获取对应的状态值进行擦出
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}
