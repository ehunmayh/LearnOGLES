//
// Created by 马彦虎 on 2021/2/8.
//通过C++实现 MayhGLSurfaceViewRenderer
//
#include "GlView.h"
#include "Utils.h"
//定义 AAssetManager
static AAssetManager * aAssetManager= nullptr;

//unsigned char * loadFileContent(const  char *path,int &fileSize){
//    unsigned char *fileContent= nullptr;
//    fileSize=0;
//    //根据文件路径获取文件的Assets队形
//    AAsset *aAsset=AAssetManager_open(aAssetManager,path,AASSET_MODE_UNKNOWN);
//    if (aAsset!= nullptr){
//        //有文件获取文件内容的长度
//        fileSize=AAsset_getLength(aAsset);
//        //开辟空间, 空间大小为内容的长度+1，个人习惯用于存放\n
//        fileContent=new unsigned char [fileSize+1];
//        //将aAsset中的内容读取到fileContent中 ,读取的长度fileSize
//        AAsset_read(aAsset,fileContent,fileSize);
//        //最后一位写0
//        fileContent[fileSize]=0;
//        //记得释放资源
//        AAsset_close(aAsset);
//    }
//    return fileContent;
//}

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
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"Render %f",GetFrameTime());
    //擦出某些东西，  擦出（颜色缓存区）GL_COLOR_BUFFER_BIT
    //GL_DEPTH_BUFFER_BIT 深度缓冲区
    //GL_STENCIL_BUFFER_BIT 模板缓冲
    //open从GL状态机中获取对应的状态值进行擦出
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);
}
