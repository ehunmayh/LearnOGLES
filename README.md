# LearnOGLES
学习使用opengles

#### opengl 绘制一个三角形：
1:齐次坐标:
        
    正常的三维（3D）坐标（x,y,z）,  而齐次坐标表示形式（x,y,z,w），转换成3D坐标为（x/w，y/w，z/w），其中当w趋近于0时，表示一个无穷远的坐标
    例如：3D坐标（1,2,3）可以写成齐次坐标（1,2,3,1）、（2,4,6,2）等 

2: 为什么使用齐次坐标呢：
    
    因为3D视野中使用矩阵都是4*4的

3: opengl的坐标体系和数学的3D坐标类似，暂时这么理解

4: ccw（counter clock width）:

    逆时针，3D事视野的正面， 当我们去看3D视野中的一个面片时，顶点的组织顺序是逆时针方向的， 如果我们在渲染的时候开启了反面剔除，那么反面就不会绘制出来，提高效率  
    cw：顺时针，反面
  
5: VBO： 
     
     将顶点数据从cpu设置到gpu中

6: GPU的工作（绘制）流程  3D渲染管线的流程简介

    顶点数据从cpu--》gpu--》然后通过写shader进行绘制， shader的过程三个点的处理是并行的
    shader过程：data-->vertex shader-->tcs shader-->tes shaser-->g shasdr-->光栅器（绘制成面）--》fragment Shader--绘制成三角形
     
    精简两次shader：data-->vertex shader-->光栅器（绘制成面，gpu内部将三个顶点数据绘制成面，并计算输出三角形内所有的点，给f shader）--》fragment Shader--绘制成三角形， 省略的shader一般都用不到
      
7: 最简单的Shader代码讲解：（shader的语法和c语法是一样的，我们学shader，主要学的是显卡的工作原理）
          
    test.vs： （vertex shader）
    attribute vec4 position; //vec4 变量类型，四位齐次坐标的向量, vs的输入是一个点，即三角形的顶点，  属性
    uniform mat4 U_ModeMatrix;//mat4 变量类型，表示4乘4的矩阵 
    uniform mat4 U_ViewMatrix;
    uniform mat4 U_ProjectionMatrix;
    //上面三个MVP的矩阵，
    void main(){
       //这个vertex shader最重要的是输出是一个点gl_Position是显卡的内置变量，必须这么写，
       //三个变量的使用pvm
       gl_Position=U_ProjectionMatrix*U_ViewMatrix*U_ModeMatrix*position;
    }      
    
    text.fs : uniform可以使用在fs中
    void main {
       // 重要的是输出一个像素，每次输出一个颜色：gl_FragColor是显卡的内置变量，必须这么写
       gl_FragColor=vec4(1.0,1.0,1.0,1.0);
    }  
    
    上面两个shader中gl_Position和gl_FragColor是gpu（显卡）的内置变量，必须这么写， 最终对于我们开发者可以操作的是attribute和uniform
    shader的代码是在不同的核心上并行执行
    
8: attribute和uniform是什么区别又是什么？

    uniform：英文解释是一致的，即我们在渲染点的时候用的模型、视口、投影矩阵是一样的。如果渲染某些顶点数据时，这些数据使用的一些变量是一样的我们就用uniform,
    如果有些东西是跟着顶点走的，比如各个顶点的具体位置坐标是跟着顶点走的，是不一样的，我们就把它们归类成顶点的属性attribute，跟着属性走的
    attribute在opengles2.0上最多有八个属性组，对于Android开发者来说一般用不到8个，3.0有16个
    总结：有区别时attribute， 没有区别用uniform， 这两个个数的使用都是有限制的，只是一般永不了那么多，可以忽略这个限制， 一般在游戏开发时，一些特别复杂的场景需要考虑这个问题
   
9：详解3D渲染管线（MVP)

    MVP的矩阵：
            U_ModeMatrix：模型矩阵用来摆放物体的，object  比如平移、旋转、缩放  ，如果我们不对其设置，默认值一个单位矩阵，不做任何变换
            U_ViewMatrix：视口矩阵：用来摆放相机的 camera  看什么东西    默认是在左边系原点看z轴的负方向
            U_ProjectionMatrix:投影矩阵是将相机看到的3D内容投影成一个2D的画面的  投影矩阵必须要定义默认值，否则无法绘制出内容
            
      gl_Position=U_ProjectionMatrix*U_ViewMatrix*U_ModeMatrix*position;
      U_ModeMatrix*position（模型矩阵乘以坐标转成--》世界坐标）--》U_ViewMatrix*U_ModeMatrix*position（世界坐标*视口矩阵--》视口坐标）---》U_ProjectionMatrix（通过投影矩阵转到屏幕上）

10: shader之间的数据传递必须通过varying 变量

    attribute vec4 position;
    attribute vec4 color;
    uniform mat4 size;
    uniform mat4 U_ModeMatrix;
    uniform mat4 U_ViewMatrix;
    uniform mat4 U_ProjectionMatrix;
    varying vec4 V_color;//shader值键数据传递通过 varying变量, shader之前传递的变量需要同名
    void main(){
       V-color=color;//额外输出这个变量 ,携带颜色
       gl_Position=U_ProjectionMatrix*U_ViewMatrix*U_ModeMatrix*position;
    }     
    
    //fragment shader 
    #ifdef GL_ES
    precision mediump float; //定义 gl中float的精度 ,如果编译出错No precision specified for (float)
    #endif
    varying vec4 V_color;//声明相同变量名的变量去接受这个
    void main() {
       gl_FragColor=V_color;
    }
    
    虽然我们指给三个顶点定义了颜色,但是三角形内部其他点的颜色都是光栅画内部插值出来的,各个点颜色和位置都是显卡内部做的, 法线\纹理坐标等插值也是由显卡完成
    
 11: 透视投影和正交投影
 
     透视投影:在3D视野中,近大远小, 投影矩阵默认就是一个透视投影
     glm::perspective(45.0f,float(width)/float(height),0.1f,1000.0f);
     
     正交投影:没有近大远小的效果,能够按照屏幕坐标绘制东西, 
     glm::ortho(-half_width,half_width,-half_height,half_height,0.1f,100.0f);
     
     当设置都完成后进行绘制 绘制三角形  从第几个点开始画   一共三个顶点,  从GL_ARRAY_BUFFER指向的VBO中取出三个点
     glDrawArrays(GL_TRIANGLES,0,3);
     //绘制矩形 顶点定义必须按照左右 左右的顺序定义   左下角 右下角  左上角 右上角
     glDrawArrays(GL_TRIANGLE_STRIP,0,4);
