package com.dragonpav.gltestapp;

import android.app.Activity;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.widget.TextView;
import java.util.Timer;
import java.util.TimerTask;

public class MainActivity extends Activity {

    private GLSurfaceView gsv;
	private TextView fpsTextView;
	private Renderer r;
	private Timer t, t2;
	
	@Override
    protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		gsv = findViewById(R.id.mainSurfaceView);
		fpsTextView = findViewById(R.id.mainFpsTextView);
		gsv.setEGLContextClientVersion(3);
		r = new Renderer(this, gsv);
		gsv.setRenderer(r);
    }
	@Override
	protected void onResume() {
		super.onResume();
		if (gsv != null) {
			gsv.onResume();
		}
		if (t == null && t2 == null) {
			t = new Timer("SecondCounter");
			t2 = new Timer("FpsUpdater");
			t.schedule(new TimerTask() {
				@Override
				public void run() {
					r.tick();
				}
			}, 0, 10L);
			t2.schedule(new TimerTask() {
				@Override
				public void run() {
					runOnUiThread(() -> {
						double fps = r.getFPS();
						fpsTextView.setText(String.format("FPS: %.1f", fps));
					});
				}
			}, 0, 100L);
		}
	}
	@Override
	protected void onPause() {
		super.onPause();
		gsv.onPause();
		if (gsv != null) {
			gsv.queueEvent(new Runnable() {
				@Override
				public void run() {
					if (r != null) {
						r.onStop();
					}
				}
			});
		}
		t.cancel();
		t2.cancel();
		t = null;
		t2 = null;
	}
}