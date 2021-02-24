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

/**
 * 用于计算每一帧耗费的时间
 * @return  秒
 */
float GetFrameTime(){
    //在c++中静态变量的初始化只会初始化一次
    static unsigned long long lastTime=0,currentTime=0;
    timeval current;
    //获取当前时间
    gettimeofday(&current, nullptr);
    //tv_sec ：秒    tv_usec：v 秒  ，单位统一成秒
    currentTime=current.tv_sec*1000+current.tv_usec/1000;
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"11","%d&%d",current.tv_sec,current.tv_usec);
    unsigned long long frameTime=lastTime==0?0:currentTime-lastTime;
    lastTime=currentTime;
    //返回秒
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"GetFrameTime","%d&%d",lastTime,currentTime);
    return float (frameTime)/1000.0f;
}

/**
 * 编译shader源码的方法
 * @param shader_type  shader的类型
 * @param source_code  编译的源码
 * @return
 */
GLuint CompileShader(GLenum shader_type,const char * source_code){
    //创建指定类型的shader
    GLuint shader_to_ret=glCreateShader(shader_type);

    /**
     * 给shader设置要编译源码，即从cpu---》传入到gpu
     * shader   count：源代码的个数   源码的指针    length:源码的结束  ，一般我们把shader 放到1分源码中
     *
     * 如果我们的shader存在多分源码：
     * const char * source_codes= nullptr;//加入有 5份源码, 统一拼接到这个里面，
     * GLint source_codes_lengths[5]={10,20,10,20,10};//每一份源码的长度
     * glShaderSource(shader_to_ret,5,&source_codes,source_codes_lengths);
     *
     * 或者，自己在把shader的源码拼接好在调用，不推荐一个shader的源码放到不同的文件中
     */
     glShaderSource(shader_to_ret,1,&source_code, nullptr);

     //编译shader
     glCompileShader(shader_to_ret);
     //编译接口默认成功
     GLint compile_result=GL_TRUE;
     //将编译的结果GL_COMPILE_STATUS 赋值给compile_result
     glGetShaderiv(shader_to_ret,GL_COMPILE_STATUS,&compile_result);
     if(compile_result==GL_FALSE){
         //没有编译成功 复制为0，并将日志输出
         char szLog[1024]={0};
         GLsizei logLen=0;
         glGetShaderInfoLog(shader_to_ret,1024,&logLen,szLog);
         __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"CompileShader_log: %s\ncode:%s\n",szLog,source_code);
         //有问题将Shader删除
         glDeleteShader(shader_to_ret);
         shader_to_ret=0;
     }
    return shader_to_ret;
}

/**
 * 创建一个GPU程序
 * @param vertex_shader
 * @param fragment_shader
 * @return
 */
GLuint CreateProgram(GLuint vertex_shader,GLuint fragment_shader){
    //创建program
    GLuint program_to_return=glCreateProgram();
    //将shader 关联上program
    glAttachShader(program_to_return,vertex_shader);
    glAttachShader(program_to_return,fragment_shader);
    //在C++中，先编译，在链接
    glLinkProgram(program_to_return);
    //当链接完成后解除program相关连的shader
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
    //查看编译结果
    GLint link_result=GL_TRUE;
    //将链接的结果GL_LINK_STATUS 赋值给 link_result
    glGetProgramiv(program_to_return,GL_LINK_STATUS,&link_result);
    if (link_result==GL_FALSE){
        //链接失败
        //没有编译成功 复制为0，并将日志输出
        char szLog[1024]={0};
        GLsizei logLen=0;
        glGetProgramInfoLog(program_to_return,1024,&logLen,szLog);
        __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"CreateProgran_log: %s\n",szLog);
        //有问题将Program删除
        glDeleteProgram(program_to_return);
        program_to_return=0;
    }
    return program_to_return;
}


/**
 * 封装创建一个标准的GPU程序的过程：先编译，后链接，的过程， 这个方法封装了一下上面的这两个方法 CompileShader  CreateProgram
 * @param vertex_shader_path 传入 vertex_shader源码路径
 * @param fragment_shader_path fragment_shader源码路径
 * @return
 */
GLuint CreateStandardProgram(AAssetManager *aAssetManager,const char *vertex_shader_path,const char *fragment_shader_path){
    //编译Assets下的Shader
    int size=0;
    unsigned char *fileContent=loadFileContent(aAssetManager,vertex_shader_path,size);
    //编译vertex_shader
    GLuint vsShader=CompileShader(GL_VERTEX_SHADER, (char *)fileContent);
    delete [] fileContent;
    fileContent=loadFileContent(aAssetManager,fragment_shader_path,size);
    //编译fragment_shader
    GLuint fsShader=CompileShader(GL_FRAGMENT_SHADER,(char*)fileContent);
    delete [] fileContent;
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"vs:%u  fs:%u\n",vsShader,fsShader);

    //当shader只是GPU程序的组成模块，最终是通过gpu程序绘制的
    GLuint program=CreateProgram(vsShader,fsShader);
    //当程序创建完成后删除对应的Shader
    glDeleteShader(vsShader);
    glDeleteShader(fsShader);
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"Program:%u \n",program);
    return program;
}