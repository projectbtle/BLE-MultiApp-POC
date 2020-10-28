package android.bluetooth;


import android.bluetooth.IBluetoothAam;

import android.Manifest;
import android.annotation.RequiresPermission;
import android.os.RemoteException;
import android.util.Log;
import android.annotation.SdkConstant;
import android.annotation.SdkConstant.SdkConstantType;


/**
 * BluetoothAam.
 *
 * Used to perform AAM checks prior to connecting an app to GATT.
 *
 * @hide
 */
/*package*/ public final class BluetoothAam {
    private IBluetoothAam mAam;
    
    /*package*/ BluetoothAam(IBluetoothAam iAam) {
        mAam = iAam;
    }
    
    
    /** @hide */
    @SdkConstant(SdkConstantType.BROADCAST_INTENT_ACTION)
    public static final String ACTION_AAM_REQUEST =
            "android.bluetooth.aam.action.AAM_REQUEST";
            
    public static final String EXTRA_APP_ID = "android.bluetooth.aam.extra.APP_ID";
    public static final String EXTRA_APP_NAME = "android.bluetooth.aam.extra.APP_NAME";
    public static final String EXTRA_DEV_ID = "android.bluetooth.aam.extra.DEV_ID";
    public static final String EXTRA_DEV_NAME = "android.bluetooth.aam.extra.DEV_NAME";
    
    @RequiresPermission(Manifest.permission.BLUETOOTH)
    public boolean performAamCheck(String appID, String devID, String appName, String devName)
    {
        boolean status;
        
        Log.d("CROSSAPPBLE", "/frameworks/base/core/.../BluetoothAam.java=>performAamCheck() - appID=" + appID + " devID=" + devID + " appName=" + appName + " devName=" + devName);
        
        try {
            status = mAam.checkAamAuth(appID, devID, appName, devName);
        } catch (RemoteException e) {
            Log.e("CROSSAPPBLE","",e);
            return false;
        }
        
        return status;
    }
}