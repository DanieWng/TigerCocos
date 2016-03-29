package com.yfactory.biberandredboots;

import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.IOException;
import org.cocos2dx.lib.Cocos2dxMusic;
import android.content.Context;
import android.content.Intent;
import android.net.ConnectivityManager;
import android.net.Uri;
import android.os.Environment;
import android.provider.MediaStore;
import android.util.Log;


public class TigerNDKHelper {

	private Context mContext;
	private static Cocos2dxMusic mBiberBGM;
	
	public TigerNDKHelper(Context context){
		this.mContext = context;
		TigerNDKHelper.mBiberBGM = new Cocos2dxMusic(mContext);
	}
	
	void preloadBiberBGM(String soundfile){
		TigerNDKHelper.mBiberBGM.preloadBackgroundMusic(soundfile);
	}
	
	void playBiberBGM(String soundfile, boolean isloop){
		TigerNDKHelper.mBiberBGM.playBackgroundMusic(soundfile, isloop);
	}
	
	void pauseBiberBGM(){
		TigerNDKHelper.mBiberBGM.pauseBackgroundMusic();
	}
	
	void resumeBiberBGM(){
		TigerNDKHelper.mBiberBGM.resumeBackgroundMusic();
	}
	
	void stopBiberBGM(){
		TigerNDKHelper.mBiberBGM.stopBackgroundMusic();
	}
	
	void setBiberBGMVolume(float volume){
		TigerNDKHelper.mBiberBGM.setBackgroundVolume(volume);
	}
	
	void trySendAnMail(String address, String title, String body){
		
		try{
			Intent intent = new Intent(Intent.ACTION_SEND);
			
			intent.putExtra(Intent.EXTRA_EMAIL, new String[]{address});
			intent.putExtra(Intent.EXTRA_SUBJECT, title);
			intent.putExtra(Intent.EXTRA_TEXT, body);
			intent.setType("plain/text");
			
			Intent.createChooser(intent, "Choose Email Client");
			
			mContext.startActivity(intent);;
			
		}catch(Exception e){
			e.printStackTrace();
		}

	}
	
	/**
	 * 
	 * @param address
	 * @param title
	 * @param body
	 * @param imagefile (The file path is SD card path).
	 */
	void trySendAnMailWithImage(String address, String title, String body, String imagefile)
	{
		try{
			Intent intent = new Intent(Intent.ACTION_SEND);
			
			intent.putExtra(Intent.EXTRA_EMAIL, new String[]{address});
			intent.putExtra(Intent.EXTRA_SUBJECT, title);
			intent.putExtra(Intent.EXTRA_TEXT, body);
			intent.putExtra(Intent.EXTRA_STREAM, Uri.parse("file:///" + imagefile));
			intent.setType("plain/text");
			Intent.createChooser(intent, "Choose Email Client");
			mContext.startActivity(intent);;
			
		}catch(Exception e){
			e.printStackTrace();
		}

	}
	
	String trySaveImageToSdcard(String imagefile){
		
		String outfilepath = Environment.getExternalStorageDirectory().getAbsoluteFile() + File.separator  + "/BiberAndRedBoots/BiberAndRedBoots.png";
		
		File infile = new File(imagefile);
		
		File outfile = new File(outfilepath);
		
		if(outfile.exists()){
			outfile.delete();
			outfile.getParentFile().mkdirs();
			try {
				outfile.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}else{
			outfile.getParentFile().mkdirs();
			try {
				outfile.createNewFile();
			} catch (IOException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		}
		
		try {
			FileInputStream fis = new FileInputStream(infile);
		
			FileOutputStream fos = new FileOutputStream(outfile);
			
			byte bt[] = new byte[1024];
			int c;
			
			while( (c = fis.read(bt)) > 0 ){
				fos.write(bt, 0, c);
			}
			fis.close();
			fos.close();
			
			infile.delete();
			
			Log.w("cocos2d-x", "save to sd card done");
			
		} catch (FileNotFoundException e1) {
			// TODO Auto-generated catch block
			e1.printStackTrace();
		} catch (IOException e){
			e.printStackTrace();
		}		
		
		return outfilepath;
	}
	
	void tryCallWeb(String urlStr){
		
		Intent intent = new Intent(Intent.ACTION_VIEW);
		
		intent.setData(Uri.parse(urlStr));
		
		mContext.startActivity(intent);
	}
	
	boolean isInternetConnection()
	{
		ConnectivityManager cm = (ConnectivityManager)mContext.getSystemService(Context.CONNECTIVITY_SERVICE);

		return cm.getActiveNetworkInfo() != null && cm.getActiveNetworkInfo().isConnectedOrConnecting();
	}
	
	void tryCallExit(){
		System.exit(0);
	}
	
	void trySaveImageToPhotoAlbum(String filename){
		try{
			MediaStore.Images.Media.insertImage(mContext.getContentResolver(), filename, "BiberandRedBoots", "coloring game");
		}catch(Exception e){
			e.printStackTrace();
		}
		
	}
}














