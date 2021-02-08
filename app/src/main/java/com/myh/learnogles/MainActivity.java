package com.myh.learnogles;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.widget.TextView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * 渲染器
 */
class MayhGLSurfaceViewRenderer implements GLSurfaceView.Renderer{

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        //设置颜色：在gl的渲染环境中设置ClearColor状态
        gl.glClearColor(0.1f,0.4f,0.6f,1.0f);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        //ViewPort是gl的另一个状态 ， ViewPort：画布  原点左下角（0，0），，等价Canvas
        gl.glViewport(0,0,width,height);

    }

    @Override
    public void onDrawFrame(GL10 gl) {
        //擦出某些东西，  擦出（颜色缓存区）GL_COLOR_BUFFER_BIT
        //GL_DEPTH_BUFFER_BIT 深度缓冲区
        //GL_STENCIL_BUFFER_BIT 模板缓冲
        //open从GL状态机中获取对应的状态值进行擦出
        gl.glClear(gl.GL_COLOR_BUFFER_BIT|gl.GL_DEPTH_BUFFER_BIT|gl.GL_STENCIL_BUFFER_BIT);
    }
}
class MayhGLSurfaceView extends GLSurfaceView{
    //渲染器,渲染视口
    GLSurfaceView.Renderer mRenderer;
    public MayhGLSurfaceView(Context context) {
        super(context);
        //设置OPGL 的版本
        setEGLContextClientVersion(2);
        mRenderer=new MayhGLSurfaceViewRenderer();
        //设置渲染器
        setRenderer(mRenderer);
    }
}
public class MainActivity extends AppCompatActivity {
    private MayhGLSurfaceView mGLSurfaceView;

    // Used to load the 'native-lib' library on application startup.
    //生成路径so: build/intermediates/cmake/
    static {
        System.loadLibrary("mayh");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mGLSurfaceView=new MayhGLSurfaceView(getApplicationContext());
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