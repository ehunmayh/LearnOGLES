//
// Created by 马彦虎 on 2021/2/8.
//通过C++实现 MayhGLSurfaceViewRenderer
//
#include "GlView.h"
#include "Utils.h"
//定义 AAssetManager
static AAssetManager * aAssetManager= nullptr;
//GPU程序
GLuint program;
// vertex buffer object ，用于存顶点数据缓冲区对象，顶点数据放置到Gluint上，通过Gluint vbo将顶点数据放置到显卡上去
GLuint vbo;

extern "C"
JNIEXPORT void JNICALL
Java_com_myh_learnogles_glview_MayhGLSurfaceViewRenderer_initGlView(JNIEnv
* env,
jobject thiz,
jobject am // 传入AssetsManager 操作管理 assets目录
) {
    //初始化aAssetManager
    aAssetManager=AAssetManager_fromJava(env,am);
    //打印日志
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"initGlView");
    //设置颜色：在gl的渲染环境中设置ClearColor状态
   glClearColor(0.1f,0.1,0.8f,1.0f);

   int fileSize=0;
   //读取Assets下的内容
   unsigned char *text=loadFileContent(aAssetManager,"test.txt",fileSize);
   if (text!= nullptr){
       __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"%s",(char *)text);
       //删除销毁
       delete [] text;
   }

   //定义三角形的三个顶点，这些顶点数据是在CPU内存上， 目标是把这些呈现到--》GPU上
   Vertice vertice[3];
   //顶点1
   vertice[0].mPosition[0]=-0.5f;//x
   vertice[0].mPosition[1]=-0.5f;//y
   vertice[0].mPosition[2]=-2.0f;//z
   vertice[0].mPosition[3]=1.0f;//w

    //顶点2
    vertice[0].mPosition[0]=0.5f;//x
    vertice[0].mPosition[1]=-0.5f;//y
    vertice[0].mPosition[2]=-2.0f;//z
    vertice[0].mPosition[3]=1.0f;//w

    //顶点3
    vertice[0].mPosition[0]=0.0f;//x
    vertice[0].mPosition[1]=0.5f;//y
    vertice[0].mPosition[2]=-2.0f;//z
    vertice[0].mPosition[3]=1.0f;//w

    //让显卡将vbo初始化，让显卡在显卡上创建一个对象，将对象的标识写如vbo中，通过vbo中的标识操作显卡  1：告诉显卡需要1个vbo   也可以申请多个vboglGenBuffers(2,vbos)
    glGenBuffers(1,&vbo);
    //设置数据  将vbo设置到GL_ARRAY_BUFFER卡槽上去
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    /**
     * 将数据从cpu传入到gpu上   sizeof(Vertice)*3 三个点数据的大小    将vertice中的数据设置到GL_ARRAY_BUFFER（卡槽或状态上）中 GL_STATIC_DRAW用于绘制
     * 向GL_ARRAY_BUFFER（卡槽，状态上）设置sizeof(Vertice)*3 三个点数据的大小的数据，数据是vertice中的数据，GL_STATIC_DRAW用于绘制
     * 从表面上看设置数据时和vbo没用关系，但是卡槽或状态机GL_ARRAY_BUFFER指向的vbo,所以当我们对GL_ARRAY_BUFFER进行操作时，实际上操作的是当前卡槽状态实际指向的vbo
     *
     * 如果只是想在gpu分配空间，而不传输数据 ，
     * glBufferData(GL_ARRAY_BUFFER,sizeof(Vertice)*3, nullptr,GL_STATIC_DRAW);//只是在gpu分配内存而不传输数据
     * 后期在动态设置数据
     * glBufferSubData(GL_ARRAY_BUFFER,0,sizeof(Vertice)*3,vertice);//让后在动态的设置数据cpu--》gpu
     */
    glBufferData(GL_ARRAY_BUFFER,sizeof(Vertice)*3,vertice,GL_STATIC_DRAW);

    //将卡槽GL_ARRAY_BUFFER重新指向0号vbo, 0号是一个缺省值默认值， 为什么这么设置指向0号呢，为了防止后面失误操作了vbo产生数据污染， 因为这个已经将vbo重新指向了0号vbo,
    // 即后面我失误调用了glBufferData(GL_ARRAY_BUFFER,sizeof(Vertice)*3,vertice,GL_STATIC_DRAW)，也不会对之前的产生影响
    glBindBuffer(GL_ARRAY_BUFFER,0);
    //船舰一个GPU程序
    program=CreateStandardProgram(aAssetManager,"test.vs","test.fs");
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

    //打印日志  计算每一次调用render和上一次间隔的时间
//    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"Render %f",GetFrameTime());
    //擦出某些东西，  擦出（颜色缓存区）GL_COLOR_BUFFER_BIT
    //GL_DEPTH_BUFFER_BIT 深度缓冲区
    //GL_STENCIL_BUFFER_BIT 模板缓冲
    //open从GL状态机中获取对应的状态值进行擦出
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}
