package com.example.ffmpegbasic;

import android.app.Activity;
import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
import android.widget.Toast;

public class MainActivity extends Activity {
	private MoviePlayView playView;
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        
        playView = new MoviePlayView(this);
        setContentView(playView);
    }
    
    @Override
    protected void onStop() {
    	// TODO Auto-generated method stub
    	super.onStop();
//    	playView.close();
    }
}

class MoviePlayView extends View{
	private Bitmap mBitmap;
	
	public MoviePlayView(Context context){
		super(context);
		
//		encoderVideo();
		
		if(initBasicPlayer()<0){
			Toast.makeText(context, "CPU doesn't support NEON", Toast.LENGTH_LONG).show();
			((Activity)context).finish();
		}
		
		String fname = Environment.getExternalStorageDirectory().getAbsolutePath() + "/DCIM/Camera/Wonder.mp4";
		int openResult = openMovie(fname);
		if(openResult < 0){
			Toast.makeText(context, "Open Movie Error : "+ openResult, Toast.LENGTH_LONG).show();
			((Activity)context).finish();
		}else{
			mBitmap = Bitmap.createBitmap(getMovieWidth(), getMovieHeight(), Bitmap.Config.RGB_565);
		}
	}
	
	public void close(){
		closeMovie();
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		super.onDraw(canvas);
		renderFrame(mBitmap);
		canvas.drawBitmap(mBitmap, 0, 0, null);
		invalidate();
	}
	
	static{
		System.loadLibrary("basicplayer");
	}
	
	public static native int initBasicPlayer();
	public static native int openMovie(String filePath);
	public static native int renderFrame(Bitmap bitmap);
	public static native int getMovieWidth();
	public static native int getMovieHeight();
	public static native void closeMovie();
	public static native void encoderVideo();
}
