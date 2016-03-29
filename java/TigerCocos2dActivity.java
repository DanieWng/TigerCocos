package com.yfactory.biberandredboots;

import java.io.File;
import org.cocos2dx.lib.Cocos2dxActivity;
import org.cocos2dx.lib.Cocos2dxGLSurfaceView;
import android.app.Activity;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.os.Message;
import android.os.StatFs;
import android.util.DisplayMetrics;
import android.util.Log;
import android.view.ViewGroup;
import android.widget.FrameLayout;
import android.widget.ImageView; 


public class TigerCocos2dActivity extends Cocos2dxActivity{
	
	private static TigerNDKHelper mNDKHelper;
	
	private static final int MSG_LOAD_SCENE 	= 0;
	private static final int MSG_COVER_SCENE 	= 1;
	private static final int MSG_MAIN_SCENE 	= 2;
	private static final int MSG_PAGE_SCENE 	= 3;
	private static final int MSG_GAME_SCENE     = 4;
	
	private static final int frame_laytou_id 		= 1001;
	private static final int slider_image_view_id 	= 1002;
	
	private static int tointrate;
	
	private static int load_slider_drawable;
	private static int cover_slider_drawable;
	private static int main_slider_drawable;
	private static int page_slider_drawable;
	private static int game_slider_drawable;
	
	private static ImageView sliderImageView;
	
	static int slider_image_res;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		super.onCreate(savedInstanceState);
		
		mNDKHelper = new TigerNDKHelper(getContext());
		
		loadSliderDrawable();
		addSlider();
		
	}
	
	/**
	 * 解决page view， list view等在android下可视区域绿色和z order问题.
	 */
	public Cocos2dxGLSurfaceView onCreateView() {
        Cocos2dxGLSurfaceView glSurfaceView = new Cocos2dxGLSurfaceView(this);
        glSurfaceView.setEGLConfigChooser(5, 6, 5, 0, 16, 8);
        
        return glSurfaceView;
	}
	
	

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
			
		if(sliderImageView != null)
		{
			sliderImageView.setImageResource(slider_image_res);
		}
	}

	@Override
	protected void onPause() {
		// TODO Auto-generated method stub
		super.onPause();
	}

	static Handler myHandle = new Handler(){
		public void handleMessage(Message msg){
			
			if (tointrate ==  133) {
				return;
			}else{
				
				try{
					Activity activity = (Activity)getContext();
					FrameLayout framely = (FrameLayout)activity.findViewById(frame_laytou_id);
					ImageView iv = (ImageView)framely.findViewById(slider_image_view_id);
					
					switch (msg.what)
					{
					case MSG_COVER_SCENE:
						iv.setImageResource(cover_slider_drawable);
						slider_image_res = cover_slider_drawable;
						break;
						
					case MSG_MAIN_SCENE:				
						iv.setImageResource(main_slider_drawable);
						slider_image_res = main_slider_drawable;
						break;
						
					case MSG_PAGE_SCENE:
						iv.setImageResource(page_slider_drawable);
						slider_image_res = page_slider_drawable;
						break;
						
					case MSG_LOAD_SCENE:
						iv.setImageResource(load_slider_drawable);
						slider_image_res = load_slider_drawable;
						break;
						
					case MSG_GAME_SCENE:
						iv.setImageResource(game_slider_drawable);
						slider_image_res = game_slider_drawable;
						break;
					}
				}catch (Exception e) {
					
					e.printStackTrace();
				}
			}
			
			super.handleMessage(msg);
		}
	};
	
	private void loadSliderDrawable()
	{
		DisplayMetrics metrics = new DisplayMetrics();
		getWindowManager().getDefaultDisplay().getMetrics(metrics);
		int width = metrics.widthPixels;
		int height = metrics.heightPixels;
		
		float rate = (float)width/(float)height;
		tointrate = (int)(rate*100); 
		
		Log.e("cocos2d-x", "device[width: " + width + " height: " + height + " metrics: " + tointrate + " ]");
		
		try{
			if (tointrate >= 177) {
				load_slider_drawable  = R.drawable.logo_1136x640;
				cover_slider_drawable = R.drawable.cover_1136x640;
				main_slider_drawable  = R.drawable.main_1136x640;
				page_slider_drawable  = R.drawable.explore_1136x640;
				game_slider_drawable  = R.drawable.game_1136x640;
			}else if (tointrate == 170) {
				load_slider_drawable  = R.drawable.logo_1280x752;
				cover_slider_drawable = R.drawable.cover_1280x752;
				main_slider_drawable  = R.drawable.main_1280x752;
				page_slider_drawable  = R.drawable.explore_1280x752;
				game_slider_drawable  = R.drawable.game_1280x752;
			}else if (tointrate == 173) {
				load_slider_drawable  = R.drawable.logo_1280x736;
				cover_slider_drawable = R.drawable.cover_1280x736;
				main_slider_drawable  = R.drawable.main_1280x736;
				page_slider_drawable  = R.drawable.explore_1280x736;
				game_slider_drawable  = R.drawable.game_1280x736;
			}else if (tointrate == 160)
			{
				// 개럭시4 탭 10.1 & 8.0
				load_slider_drawable  = R.drawable.logo_1280x800;
				cover_slider_drawable = R.drawable.cover_1280x800;
				main_slider_drawable  = R.drawable.main_1280x800;
				page_slider_drawable  = R.drawable.explore_1280x800;
				game_slider_drawable  = R.drawable.game_1280x800;
			}
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	private void addSlider(){
		
		if(tointrate == 133){
			return;
		}
		
		ViewGroup.LayoutParams framelayout_params = new ViewGroup.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT,
				   ViewGroup.LayoutParams.MATCH_PARENT);
		FrameLayout framelayout = new FrameLayout(this);
		framelayout.setLayoutParams(framelayout_params);

		sliderImageView = new ImageView(this);
		
		framelayout.setId(frame_laytou_id);
		sliderImageView.setId(slider_image_view_id);
		
		sliderImageView.setImageResource(load_slider_drawable);

		framelayout.addView(sliderImageView,framelayout_params);
		addContentView(framelayout, framelayout_params);
	}
	
	public static void tryAddSliderInCoverScene()
	{
		myHandle.sendEmptyMessage(MSG_COVER_SCENE);
	}
	
	public static void tryAddSliderInMainScene()
	{
		myHandle.sendEmptyMessage(MSG_MAIN_SCENE);
	}
	
	public static void tryAddSliderInPageScene(){
		myHandle.sendEmptyMessage(MSG_PAGE_SCENE);
	}
	
	public static void tryAddSliderInLoadScene()
	{
		myHandle.sendEmptyMessage(MSG_LOAD_SCENE);
	}
	
	public static void tryAddSliderInGameScene()
	{
		myHandle.sendEmptyMessage(MSG_GAME_SCENE);
	}
	
	public static void callMail(String address, String title, String body){
		
		if(mNDKHelper == null){
			Log.e("cocos2d-x","ndk helper is null.");
			return;
		}
		
		try{
			mNDKHelper.trySendAnMail(address, title, body);
		}catch(Exception e){
			e.printStackTrace();
		}
		
	}
	
	public static String trySaveImageToSdcard(String imagefile){
		return mNDKHelper.trySaveImageToSdcard(imagefile);
	}
	
	public static void trySendAnMailWithImage(String address, String title, String body, String imagefile){
		mNDKHelper.trySendAnMailWithImage(address, title, body, imagefile);
	}
	
	public static void trySaveImageToPhotoAlbum(String filename) throws Exception{
		mNDKHelper.trySaveImageToPhotoAlbum(filename);
	}
	
	public static void callWeb(String urlStr){
		try{
			mNDKHelper.tryCallWeb(urlStr);
		}catch(Exception e){
			e.printStackTrace();
		}
	}
	
	public static boolean callIsInternetConnection(){
		
		return mNDKHelper.isInternetConnection();
	}
	
	public static long tryGetAvailableInternalMemorySize()
	{
		File path = Environment.getDataDirectory();
		StatFs stat = new StatFs(path.getPath());
		long blockSize = stat.getBlockSize();
		long availableBlocks = stat.getAvailableBlocks();
		
		Log.e("cocos2d-x", "able memory size: " + Long.toString((availableBlocks * blockSize) / (1024 * 1024)));
		
		return (availableBlocks * blockSize) / (1024 * 1024);
	}
	
	public static void tryPreloadBiberBGM(String sound){
		TigerCocos2dActivity.mNDKHelper.preloadBiberBGM(sound);
	}
	
	public static void tryPlayBiberBGM(String sound, boolean isloop){
		TigerCocos2dActivity.mNDKHelper.playBiberBGM(sound, isloop);
	}
	
	public static void tryPauseBiberBGM(){
		TigerCocos2dActivity.mNDKHelper.pauseBiberBGM();
	}
	
	public static void tryResumeBiberBGM(){
		TigerCocos2dActivity.mNDKHelper.resumeBiberBGM();
	}
	
	public static void tryStopBiberBGM(){
		TigerCocos2dActivity.mNDKHelper.stopBiberBGM();
	}
	
	public static void trySetBiberBGMVolume(float volume){
		TigerCocos2dActivity.mNDKHelper.setBiberBGMVolume(volume);
	}
}
