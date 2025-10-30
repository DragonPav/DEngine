package com.dragonpav.dengine;

import android.content.res.Resources;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.opengl.GLES30;
import android.opengl.GLUtils;

public class NormalMap {
	public int glType = GLES30.GL_TEXTURE1;
	public int textureId;
	public int samplerLoc = -1;
	public int useNormalMapLoc = -1;
	
	public NormalMap(Resources res, int resId, Program p) {
		samplerLoc = GLES30.glGetUniformLocation(p.programId, "normalMapUnit");
		useNormalMapLoc = GLES30.glGetUniformLocation(p.programId, "useNormalMap");
		int[] tmpBuf = new int[1];
		GLES30.glGenTextures(1, tmpBuf, 0);
		textureId = tmpBuf[0];
		BitmapFactory.Options opts = new BitmapFactory.Options();
		opts.inScaled = false;
		Bitmap img = BitmapFactory.decodeResource(res, resId, opts);
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