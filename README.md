# LearnOGLES
学习使用opengles

#### opengl 绘制一个三角形：
1.齐次坐标：正常的三维（3D）坐标（x,y,z）,  而齐次坐标表示形式（x,y,z,w），转换成3D坐标为（x/w，y/w，z/w），其中当w趋近于0时，表示一个无穷远的坐标
例如：3D坐标（1,2,3）可以写成齐次坐标（1,2,3,1）、（2,4,6,2）等 

2.为什么使用齐次坐标呢：因为3D视野中使用矩阵都是4*4的

3.opengl的坐标体系和数学的3D坐标类似，暂时这么理解

4.ccw（counter clock width）:逆时针，3D事视野的正面， 当我们去看3D视野中的一个面片时，顶点的组织顺序是逆时针方向的， 如果我们在渲染的时候开启了反面剔除，那么反面就不会绘制出来，提高效率  
  cw：顺时针，反面
  
5. VBO： 将顶点数据从cpu设置到gpu中

6. GPU的工作（绘制）流程  3D渲染管线的流程简介
   顶点数据从cpu--》gpu--》然后通过写shader进行绘制， shader的过程三个点的处理是并行的
   shader过程：data-->vertex shader-->tcs shader-->tes shaser-->g shasdr-->光栅器（绘制成面）--》fragment Shader--绘制成三角形
     
   精简两次shader：data-->vertex shader-->光栅器（绘制成面，gpu内部将三个顶点数据绘制成面，并计算输出三角形内所有的点，给f shader）--》fragment Shader--绘制成三角形， 省略的shader一般都用不到
        