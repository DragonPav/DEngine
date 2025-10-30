package com.dragonpav.dengine;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES30;
import android.opengl.GLUtils;
import android.util.Log;
import java.nio.ByteBuffer;
import java.nio.ByteOrder;

public class Texture {
	public int texSampleNum;
	public int textureId;
	public int glType = GLES30.GL_TEXTURE0;
	public Texture(byte[] rgb, Program p) {
		this.glType = glType;
		texSampleNum = GLES30.glGetUniformLocation(p.programId, "textureUnit");
		byte[] colors = new byte[256 * 256 * 3];
		int i = 0;
		while (i != colors.length) {
			colors[i] = rgb[0];
			i++;
			colors[i] = rgb[1];
			i++;
			colors[i] = rgb[2];
			i++;
		}
		ByteBuffer colorBuffer = ByteBuffer.allocateDirect(colors.length * 4).order(ByteOrder.nativeOrder());
		colorBuffer.put(colors);
		colorBuffer.position(0);
		int[] tmpBuf = new int[1];
		GLES30.glGenTextures(1, tmpBuf, 0);
		textureId = tmpBuf[0];
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_LINEAR_MIPMAP_LINEAR);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_NEAREST);
		GLES30.glTexImage2D(GLES30.GL_TEXTURE_2D, 0, GLES30.GL_RGB, 256, 256, 0, GLES30.GL_RGB, GLES30.GL_UNSIGNED_BYTE, colorBuffer);
		GLES30.glGenerateMipmap(GLES30.GL_TEXTURE_2D);
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
	}
	public Texture(Resources res, int resId, Program p) {
		BitmapFactory.Options opts = new BitmapFactory.Options();
		opts.inScaled = false;
		Bitmap img = BitmapFactory.decodeResource(res, resId, opts);
		this.glType = glType;
		texSampleNum = GLES30.glGetUniformLocation(p.programId, "textureUnit");
		int[] tmpBuf = new int[1];
		GLES30.glGenTextures(1, tmpBuf, 0);
		textureId = tmpBuf[0];
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, GLES30.GL_CLAMP_TO_EDGE);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, GLES30.GL_CLAMP_TO_EDGE);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, GLES30.GL_NEAREST_MIPMAP_LINEAR);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, GLES30.GL_NEAREST);
		GLUtils.texImage2D(GLES30.GL_TEXTURE_2D, 0, GLES30.GL_RGBA, img, 0);
		GLES30.glGenerateMipmap(GLES30.GL_TEXTURE_2D);
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
		img.recycle();
	}
}