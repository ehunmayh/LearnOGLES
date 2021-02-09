package com.myh.learnogles.glview;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * myh  版权所有
 * Copyright (c) 2021 All Rights Reserved
 * <p>
 * 作者：马彦虎  Email：1184265546@qq.com
 * 创建时间： 2021/2/8.
 * 修改历史:
 * 修改日期         作者        版本        描述说明
 * </p>
 *
 * java层级实现GL 的窗口视图，效率低, 通过c实现
 */
public class MayhGLSurfaceViewRenderer implements GLSurfaceView.Renderer {
    static {
        System.loadLibrary("mayh");
    }

    private final Context mContext;

    public native void initGlView(AssetManager manager);
    public native void onSurfaceChange(int width,int height);
    public native void Render();
    public MayhGLSurfaceViewRenderer(Context context){
        mContext = context;
    }
    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        //设置颜色：在gl的渲染环境中设置ClearColor状态  java层
//        gl.glClearColor(0.1f,0.4f,0.6f,1.0f);
        //c++层
        initGlView(mContext.getAssets());
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        //ViewPort是gl的另一个状态 ， ViewPort：画布  原点左下角（0，0），，等价Canvas
//        gl.glViewport(0,0,width,height);
        //c++
        onSurfaceChange(width,height);

    }

    @Override
    public void onDrawFrame(GL10 gl) {
        //擦出某些东西，  擦出（颜色缓存区）GL_COLOR_BUFFER_BIT
        //GL_DEPTH_BUFFER_BIT 深度缓冲区
        //GL_STENCIL_BUFFER_BIT 模板缓冲
        //open从GL状态机中获取对应的状态值进行擦出
//        gl.glClear(gl.GL_COLOR_BUFFER_BIT|gl.GL_DEPTH_BUFFER_BIT|gl.GL_STENCIL_BUFFER_BIT);

        //c++
        Render();
    }
}
