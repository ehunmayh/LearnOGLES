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
        //设置颜色
        gl.glClearColor(0.1f,0.4f,0.6f,1.0f);
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        gl.glViewport(0,0,width,height);

    }

    @Override
    public void onDrawFrame(GL10 gl) {
        gl.glClear(gl.GL_COLOR_BUFFER_BIT);
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