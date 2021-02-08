package com.myh.learnogles.glview;

import android.content.Context;
import android.opengl.GLSurfaceView;


/**
 * myh  版权所有
 * Copyright (c) 2018 All Rights Reserved
 * <p>
 * 作者：马彦虎  Email：1184265546@qq.com
 * 创建时间： 2021/2/8.
 * 修改历史:
 * 修改日期         作者        版本        描述说明
 * </p>
 */
public class MayhGlSurfaceView extends GLSurfaceView {
    //渲染器,渲染视口
    public MayhGlSurfaceView(Context context,GLSurfaceView.Renderer renderer) {
        super(context);
        //设置OPGL 的版本
        setEGLContextClientVersion(2);
        //设置渲染器
        setRenderer(renderer);
    }
}
