package com.dragonpav.gltestapp;

import android.content.Context;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;

import com.dragonpav.dengine.Camera;
import com.dragonpav.dengine.CameraTouchControl;
import com.dragonpav.dengine.Lighting;
import com.dragonpav.dengine.Object3D;
import com.dragonpav.dengine.ObjectCreator;
import com.dragonpav.dengine.Program;
import com.dragonpav.dengine.RenderUtils;
import com.dragonpav.dengine.Shader;
import com.dragonpav.dengine.Texture;
import com.dragonpav.dengine.Values;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class Renderer extends com.dragonpav.dengine.Renderer {
	private Shader vertex, fragment;
	private Program program;
	private Object3D cube, sphere, quad;
	private Camera camera;
	private CameraTouchControl ctt;
	private RenderUtils rendUtils;
	private float seconds;
	@Override
	public void onSurfaceCreated(GL10 unused, EGLConfig config) {
		vertex = new Shader(context.getResources().openRawResource(R.raw.vertex), Shader.Type.Vertex);
		vertex.compile();
		fragment = new Shader(context.getResources().openRawResource(R.raw.fragment), Shader.Type.Fragment);
		fragment.compile();
		program = new Program(vertex, fragment);
		program.link();
		GLES30.glUseProgram(program.programId);
		
		Lighting lighting = new Lighting(new Values.Vector3(0.5f, 2, 3), program);
		lighting.lightPos.rgb = new float[] {0.5f, 2, -2};
		rendUtils = new RenderUtils(program, lighting, gsv);
		rendUtils.init();
		
		camera = new Camera(rendUtils);
		camera.lookAt(new Values.Vector3(0, 0, 0));
		camera.setPos(new Values.Vector3(0.5f, 2, 3));
		ctt = new CameraTouchControl(rendUtils);
		camera.setCameraTouchControl(ctt);
		Texture texture = new Texture(context.getResources(), R.drawable.cat, program, new Texture.Config());
		Texture texture2 = new Texture(new byte[] {(byte) 0x50, (byte) 0x50, (byte) 0x50}, program, new Texture.Config());
		Texture textureRed = new Texture(new byte[] {(byte) 0xff, 0, 0}, program, new Texture.Config());
		cube = ObjectCreator.createBox(new Values.Vector3(), new Values.Vector3(1, 1, 1), camera, texture);
		quad = ObjectCreator.createSurface(new Values.Vector3(0, -1, 0), new Values.Vector2(50, 50), camera, texture2);
		sphere = ObjectCreator.createSphere(new Values.Vector3(0, 0, -3), 1, 36, 18, camera, textureRed);
		GLES30.glEnable(GLES30.GL_DEPTH_TEST);
	}
	@Override
	public void onSurfaceChanged(GL10 unused, int width, int height) {
		GLES30.glViewport(0, 0, width, height);
		camera.perspective(80, (float) width / height, 0.01f, 100f);
	}
	@Override
	public void onDrawFrame(GL10 unused) {
		super.onDrawFrame(unused);
		GLES30.glClearColor(0, 0, 0, 1);
		GLES30.glClear(GLES30.GL_COLOR_BUFFER_BIT | GLES30.GL_DEPTH_BUFFER_BIT);
		quad.begin();
		quad.render();
		quad.end();
		cube.begin();
		cube.model.rotate(new Values.Vector3(1, 1, 0), seconds * 50, cube);
		cube.render();
		cube.end();
		sphere.begin();
		sphere.render();
		sphere.end();
	}
	public void tick() {
		seconds += 0.01;
	}
	public Renderer(Context ctx, GLSurfaceView gsv) {
		super(ctx, gsv);
	}
}