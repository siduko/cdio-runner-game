/****************************************************************************
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Zynga Inc.
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org
****************************************************************************/
/*
Copyright (c) 2014 Mudafar
GPLv3
*/

package org.cocos2dx.cpp;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.annotation.TargetApi;
import android.graphics.Color;
import android.graphics.Point;
import android.os.Build;
import android.os.Bundle;
import android.view.Display;
import android.view.View;
import android.view.WindowManager;
import android.widget.LinearLayout;

import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdView;

public class AppActivity extends Cocos2dxActivity {

private static AppActivity _appActiviy;
private AdView adView;
private static final String AD_UNIT_ID = "ca-app-pub-1432269596717411/3671925483";

// Helper get display screen to avoid deprecated function use
private Point getDisplaySize(Display d)
    {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.HONEYCOMB)
        {
            return getDisplaySizeGE11(d);
        }
        return getDisplaySizeLT11(d);
    }

    @TargetApi(Build.VERSION_CODES.HONEYCOMB_MR2)
    private Point getDisplaySizeGE11(Display d)
    {
        Point p = new Point(0, 0);
        d.getSize(p);
        return p;
    }
    private Point getDisplaySizeLT11(Display d)
    {
        try
        {
            Method getWidth = Display.class.getMethod("getWidth", new Class[] {});
            Method getHeight = Display.class.getMethod("getHeight", new Class[] {});
            return new Point(((Integer) getWidth.invoke(d, (Object[]) null)).intValue(), ((Integer) getHeight.invoke(d, (Object[]) null)).intValue());
        }
        catch (NoSuchMethodException e2) // None of these exceptions should ever occur.
        {
            return new Point(-1, -1);
        }
        catch (IllegalArgumentException e2)
        {
            return new Point(-2, -2);
        }
        catch (IllegalAccessException e2)
        {
            return new Point(-3, -3);
        }
        catch (InvocationTargetException e2)
        {
            return new Point(-4, -4);
        }
    }




		    @Override
		protected void onCreate(Bundle savedInstanceState){
		super.onCreate(savedInstanceState);
		
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
		
		int width = getDisplaySize(getWindowManager().getDefaultDisplay()).x;
		
		
		LinearLayout.LayoutParams adParams = new LinearLayout.LayoutParams(
		width,
		LinearLayout.LayoutParams.WRAP_CONTENT);
		
		adView = new AdView(this);
		adView.setAdSize(AdSize.SMART_BANNER);
		adView.setAdUnitId(AD_UNIT_ID);
		
		AdRequest adRequest = new AdRequest.Builder()
		.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
		.addTestDevice("1D0841EECB3984CF569CE7AA60A069CC")
		.build();
		
		adView.loadAd(adRequest);
        adView.setBackgroundColor(Color.BLACK);
        adView.setBackgroundColor(0);
        addContentView(adView,adParams);

        _appActiviy = this;

}


     public static void hideAd()
    {
     _appActiviy.runOnUiThread(new Runnable()
     {

     @Override
     public void run()
     {
if (_appActiviy.adView.isEnabled())
_appActiviy.adView.setEnabled(false);
if (_appActiviy.adView.getVisibility() != 4 )
_appActiviy.adView.setVisibility(View.INVISIBLE);
     }
     });

    }


     public static void showAd()
    {
     _appActiviy.runOnUiThread(new Runnable()
     {

     @Override
     public void run()
     {  
if (!_appActiviy.adView.isEnabled())
_appActiviy.adView.setEnabled(true);
if (_appActiviy.adView.getVisibility() == 4 )
_appActiviy.adView.setVisibility(View.VISIBLE); 
     }
     });

    }

@Override
protected void onResume() {
super.onResume();
if (adView != null) {
adView.resume();
}
}

@Override
protected void onPause() {
if (adView != null) {
adView.pause();
}
super.onPause();
}

    @Override
    protected void onDestroy() {
     adView.destroy();
        super.onDestroy();
    }

}