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
	public int texSampleNum = -1;
	public int textureId;
	public int glType = GLES30.GL_TEXTURE0;
	public Texture(byte[] rgb, Program p, Config cfg) {
		texSampleNum = GLES30.glGetUniformLocation(p.programId, "textureUnit");
		int num = 256* 256 * 3;
		byte[] colors = new byte[num];
		int i = 0;
		ByteBuffer colorBuffer = ByteBuffer.allocateDirect(num).order(ByteOrder.nativeOrder());
		while (i != num) {
			colorBuffer.put((byte) (rgb[0] & 0xFF));
			colorBuffer.put((byte) (rgb[1] & 0xFF));
			colorBuffer.put((byte) (rgb[2] & 0xFF));
			i += 3;
		}
		colorBuffer.position(0);
		int[] tmpBuf = new int[1];
		GLES30.glGenTextures(1, tmpBuf, 0);
		textureId = tmpBuf[0];
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, cfg.textureWrapS);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, cfg.textureWrapT);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, cfg.textureMinFilter);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, cfg.textureMagFilter);
		GLES30.glTexImage2D(GLES30.GL_TEXTURE_2D, 0, GLES30.GL_RGB, 256, 256, 0, GLES30.GL_RGB, GLES30.GL_UNSIGNED_BYTE, colorBuffer);
		if (cfg.useMipmap) {
			GLES30.glGenerateMipmap(GLES30.GL_TEXTURE_2D);
		}
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
	}
	public Texture(Resources res, int resId, Program p, Config cfg) {
		BitmapFactory.Options opts = new BitmapFactory.Options();
		opts.inScaled = false;
		Bitmap img = BitmapFactory.decodeResource(res, resId, opts);
		texSampleNum = GLES30.glGetUniformLocation(p.programId, "textureUnit");
		int[] tmpBuf = new int[1];
		GLES30.glGenTextures(1, tmpBuf, 0);
		textureId = tmpBuf[0];
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, textureId);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_S, cfg.textureWrapS);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_WRAP_T, cfg.textureWrapT);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MIN_FILTER, cfg.textureMinFilter);
		GLES30.glTexParameteri(GLES30.GL_TEXTURE_2D, GLES30.GL_TEXTURE_MAG_FILTER, cfg.textureMagFilter);
		int byteCount = img.getByteCount();
		ByteBuffer buffer = ByteBuffer.allocateDirect(byteCount).order(ByteOrder.nativeOrder());
		img.copyPixelsToBuffer(buffer);
		buffer.position(0);
		GLES30.glTexImage2D(GLES30.GL_TEXTURE_2D, 0, GLES30.GL_RGBA, img.getWidth(), img.getHeight(), 0, GLES30.GL_RGBA, GLES30.GL_UNSIGNED_BYTE, buffer);
		if (cfg.useMipmap) {
			GLES30.glGenerateMipmap(GLES30.GL_TEXTURE_2D);
		}
		GLES30.glBindTexture(GLES30.GL_TEXTURE_2D, 0);
		img.recycle();
	}
	public void dispose() {
		if (textureId != 0) {
			int[] textures = new int[] {textureId};
			GLES30.glDeleteTextures(1, textures, 0);
			textureId = 0;
		}
	}
	public static class Config {
		public boolean useMipmap = true;
		public int textureWrapS = GLES30.GL_CLAMP_TO_EDGE;
		public int textureWrapT = GLES30.GL_CLAMP_TO_EDGE;
		public int textureMinFilter = GLES30.GL_NEAREST_MIPMAP_LINEAR;
		public int textureMagFilter = GLES30.GL_NEAREST;
	}
}