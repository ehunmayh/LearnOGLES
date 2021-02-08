package com.myh.learnogles;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.myh.learnogles.glview.MayhGLSurfaceViewRenderer;
import com.myh.learnogles.glview.MayhGlSurfaceView;

///**
// * 渲染器
// * 通过java层的API调用OpenGl 效率低，我们需要在c++蹭去实现x
// */
//class MayhGLSurfaceViewRenderer implements GLSurfaceView.Renderer{
//
//    @Override
//    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
//        //设置颜色：在gl的渲染环境中设置ClearColor状态
//        gl.glClearColor(0.1f,0.4f,0.6f,1.0f);
//    }
//
//    @Override
//    public void onSurfaceChanged(GL10 gl, int width, int height) {
//        //ViewPort是gl的另一个状态 ， ViewPort：画布  原点左下角（0，0），，等价Canvas
//        gl.glViewport(0,0,width,height);
//
//    }
//
//    @Override
//    public void onDrawFrame(GL10 gl) {
//        //擦出某些东西，  擦出（颜色缓存区）GL_COLOR_BUFFER_BIT
//        //GL_DEPTH_BUFFER_BIT 深度缓冲区
//        //GL_STENCIL_BUFFER_BIT 模板缓冲
//        //open从GL状态机中获取对应的状态值进行擦出
//        gl.glClear(gl.GL_COLOR_BUFFER_BIT|gl.GL_DEPTH_BUFFER_BIT|gl.GL_STENCIL_BUFFER_BIT);
//    }
//}
//class MayhGlSurfaceView extends GLSurfaceView{
//    //渲染器,渲染视口
//    GLSurfaceView.Renderer mRenderer;
//    public MayhGlSurfaceView(Context context) {
//        super(context);
//        //设置OPGL 的版本
//        setEGLContextClientVersion(2);
//        mRenderer=new MayhGLSurfaceViewRenderer();
//        //设置渲染器
//        setRenderer(mRenderer);
//    }
//}
public class MainActivity extends AppCompatActivity {
    private MayhGlSurfaceView mGLSurfaceView;
    //渲染器
    private MayhGLSurfaceViewRenderer mRenderer;

    //生成路径so: build/intermediates/cmake/
    static {
        System.loadLibrary("mayh");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_main);
        mRenderer=new MayhGLSurfaceViewRenderer();
        mGLSurfaceView=new MayhGlSurfaceView(getApplicationContext(),mRenderer);
        //设置窗口
        setContentView(mGLSurfaceView);
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();
    public native int Test();
    public native String stringFromSence();
}