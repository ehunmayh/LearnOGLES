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
//对应vertex_shader 上插槽的位置 MVP
GLint modelMatrixLocation,viewMatrixLocation,projectionMatrixLocation;
//传入shader 属性相关的position
GLint attributePosition,attributeColorPosition;
//声明MVP矩阵, 从cpu--->传入到gpu, modelMatrix,viewMatrix不做任何设置,引用用gl的默认行为,都是单位矩阵,
// 而投影矩阵是我们必须要设置的在onSurfaceChange方法中,projectMatrix
glm::mat4 modelMatrix,viewMatrix,projectMatrix;
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
   glClearColor(0.5f,0.5,0.8f,1.0f);

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
   vertice[0].mPosition[2]=5.0f;//z  当z轴大于等于0时,就无法绘制出来了,可以理解为=0时和眼睛在一个面上是无法开出效果的, 大于0就是盲区
   vertice[0].mPosition[3]=1.0f;//w
   vertice[0].mColor[0]=1.0f;//r
   vertice[0].mColor[1]=1.0f;//g
   vertice[0].mColor[2]=0.0f;//b
   vertice[0].mColor[3]=1.0f;//a

    //顶点2
    vertice[1].mPosition[0]=0.5f;//x
    vertice[1].mPosition[1]=-0.5f;//y
    vertice[1].mPosition[2]=5.0f;//z
    vertice[1].mPosition[3]=1.0f;//w
    vertice[1].mColor[0]=1.0f;//r
    vertice[1].mColor[1]=0.0f;//g
    vertice[1].mColor[2]=1.0f;//b
    vertice[1].mColor[3]=1.0f;//a

    //顶点3
    vertice[2].mPosition[0]=0.0f;//x
    vertice[2].mPosition[1]=0.5f;//y
    vertice[2].mPosition[2]=5.0f;//z
    vertice[2].mPosition[3]=1.0f;//w
    vertice[2].mColor[0]=0.0f;//r
    vertice[2].mColor[1]=1.0f;//g
    vertice[2].mColor[2]=1.0f;//b
    vertice[2].mColor[3]=1.0f;//a

    //当z轴大于等于0时,就无法绘制出来了,可通过对矩阵模型的操作在让其显示出来
    modelMatrix=glm::translate(0.0f,0.0f,-8.0f)//平移
            *glm::scale(1.0f,1.0f,1.0f)//缩放
            *glm::rotate(360.0f,0.0f,0.0f,1.0f);//旋转360度, 这个点 表示的z轴旋转, 这些效果都是叠加显示的

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
    //创建一个GPU程序
    program=CreateStandardProgram(aAssetManager,"test.vs","test.fs");
    //获取插槽索引 位置 M V P,  第二个参数是shader中 Uniform变量的名字, 将这些数据从CPU传入GPU
    //通过获取到这些位置,我们是通过这些位置对顶点进行设置的
    modelMatrixLocation=glGetUniformLocation(program,"U_ModeMatrix");
    viewMatrixLocation=glGetUniformLocation(program,"U_ViewMatrix");
    projectionMatrixLocation=glGetUniformLocation(program,"U_ProjectionMatrix");
    attributePosition=glGetAttribLocation(program,"position");
    //即获取shader中Uniform和attribute变量的值， 需要注意的是如果shader中定义的变量没有使用，没有对gl_FragColor和gl_Position造成影响的话，显卡驱动就会把它优化掉，此时获取的值是-1
    attributeColorPosition=glGetAttribLocation(program,"color");//这俩个测试变量在shader中没有用到,获取的值为-1
    //GLint sizePosition=glGetUniformLocation(program,"size");//
    __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,"%d,%d,%d,%d,%d",attributePosition,modelMatrixLocation,viewMatrixLocation,projectionMatrixLocation,attributeColorPosition);
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
    //参数一是一个角度   参数二是固定的opengl的宽高比    第三个:在opengl的坐标中,看下z轴负方向,看到的最近的坐标  第四个是最远的坐标
    projectMatrix=glm::perspective(45.0f,float(width)/float(height),0.1f,1000.0f);
    //opengl中默认的viewMatrix  设置这的这个就是默认值,不设置的话也会用这个默认值
    viewMatrix=glm::lookAt(glm::vec3(0.0f,0.0f,0.0f)//站在哪个位置看
            ,glm::vec3(0.0f,0.0f,-1.0f)//看向哪里
            ,glm::vec3(0.0,1.0f,0.0f));//垂直向上方向的向量,y轴的正方向
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
    //通过vbo去设置shader中attribute变量的值  将状态机指向vbo
    glBindBuffer(GL_ARRAY_BUFFER,vbo);
    //设置用哪个程序绘制 必须线设置程序,在设置属性
    glUseProgram(program);
    //设置MVP矩阵 第一个参数是把矩阵设置到哪个位置  count: 表示设置矩阵的数量 1   ,GAL_FALSE表示矩阵不需要转置    第四个为传入的矩阵
    glUniformMatrix4fv(modelMatrixLocation,1,GL_FALSE,glm::value_ptr(modelMatrix));
    glUniformMatrix4fv(viewMatrixLocation,1,GL_FALSE,glm::value_ptr(viewMatrix));
    glUniformMatrix4fv(projectionMatrixLocation,1,GL_FALSE,glm::value_ptr(projectMatrix));
    //set attribute属性的值
    //激活这个卡槽对应的attribute 属性
    glEnableVertexAttribArray(attributePosition);
    //设置这个属性的值 ,目的是为了让GPU端能够正确的遍历VBO,把数据取出来一个一个发给Vertex_shader去执行
    glVertexAttribPointer(attributePosition,//
            4,//size:这个属性(一个点)有多少个组成部分:x,y,z,w4部分
            GL_FLOAT,// GL_FLOAT:每个组成部分的类型,
            GL_FALSE,// normalized:GL_FALSE表示是否需要映射,是将非浮点数映射成第三个参数(浮点数)类型,可以理解为是否要转换成浮点数 因为fs中gl_FragColor中颜色的类型是浮点型 所以不需要映射,如果我们写的是0-255RGB的值就需要映射成0-1的浮点数
            sizeof(Vertice),//stride每个点之间这个属性之间的间隔
            0);//第一个参数attributePosition这个属性的数据,偏移是多少
     //设置颜色属性
     glEnableVertexAttribArray(attributeColorPosition);
     //最后一个是这个属性的数据偏移, 现在一个点 由position(4个float)+color(4个float), 所以数据偏移是4个float的大小
     glVertexAttribPointer(attributeColorPosition,4,GL_FLOAT,GL_FALSE,sizeof(Vertice),(void *)(sizeof(float)*4));
    //当设置都完成后进行绘制 绘制三角形  从第几个点开始画   一共三个顶点,  从GL_ARRAY_BUFFER指向的VBO中取出三个点
    glDrawArrays(GL_TRIANGLES,0,3);
    //当绘制完成后,设置为0 避免后边的操作对vbo造成干扰
    glBindBuffer(GL_ARRAY_BUFFER,0);
    glUseProgram(0);
}
