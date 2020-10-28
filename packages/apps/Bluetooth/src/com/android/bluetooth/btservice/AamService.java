package com.android.bluetooth.btservice;

import android.content.DialogInterface;
import android.view.WindowManager;
import android.app.AppOpsManager;
import android.app.Service;
import android.app.AlertDialog;
import android.bluetooth.BluetoothAam;
import android.bluetooth.IBluetoothAam;
import android.content.Intent;
import android.content.Context;
import android.os.Binder;
import android.os.IBinder;
import android.os.RemoteException;
import android.util.Log;
import android.os.Handler;
import android.os.Looper;
import android.os.Message;
import android.text.Html;

import com.android.bluetooth.R;
import com.android.bluetooth.Utils;
import com.android.bluetooth.btservice.BluetoothProto;

import android.app.AlertDialog;
import android.view.Gravity;
import android.bluetooth.BluetoothClass;
import android.bluetooth.BluetoothAam;
import android.bluetooth.BluetoothUuid;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Bundle;
import android.text.Editable;
import android.text.InputFilter;
import android.text.InputFilter.LengthFilter;
import android.text.InputType;
import android.util.EventLog;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.TextView;

import com.android.internal.app.AlertActivity;
import com.android.internal.app.AlertController;

import java.util.Locale;
import java.util.concurrent.Semaphore;

import static android.content.pm.PackageManager.PERMISSION_GRANTED;


/*
 * @hide
 */
public class AamService extends Service {
    static final int AAM_ERROR = -1;
    static final int AAM_OK = 0;
    static final int AAM_USER_AUTH_REQD = 1;
    static final int AAM_DENY = 2;
    
    static int mUserChoice = AAM_ERROR;

    Handler mHandlerBinder;
    Handler mHandlerMain;
    Context mContext;
    
    String mAamAppID = "";
    String mAamAppName = "";
    String mAamDevID = "";
    String mAamDevName = "";
    
    private BluetoothAamBinder mBinder;
    
    @Override
    public void onCreate() {
        // Handler will get associated with the current thread, 
        // which is the main thread.
        mHandlerMain = new Handler();
        
        super.onCreate();
        mContext = this;
        mBinder = new BluetoothAamBinder(this);
        Log.d("CROSSAPPBLE", "/packages/apps/Bluetooth/src/.../btservice/AamService.java=>onCreate() Binder code: " + mBinder.toString());
    }
    
    private void runOnUiThread(Runnable runnable) {
        mHandlerMain.post(runnable);
    }
  
    int pressedButtonID;
    private final Semaphore dialogSemaphore = new Semaphore(0, true);
    final Runnable mMyDialog = new Runnable()
        {
            public void run()
            {
                createConfirmationDialog();
            }
        };

    private void createConfirmationDialog() {
        String message = "Application <b>" + mAamAppName + "</b> wants to access data from BLE device <b>";
        if (mAamDevName != "") {
            message += mAamDevName;
            message += " (";
            message += mAamDevID;
            message += ")";
        } else {
            message += "with hardware address ";
            message += mAamDevID;
        }
        message += "</b>";
        
        AlertDialog mDialog = new AlertDialog.Builder(AamService.this).create();
        mDialog.setTitle("Authorisation Requested");
        mDialog.setMessage(Html.fromHtml(message, Html.FROM_HTML_MODE_LEGACY));
        mDialog.setButton(DialogInterface.BUTTON_POSITIVE, "Allow", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                pressedButtonID = AAM_OK;
                dialogSemaphore.release();
                }
            });
        mDialog.setButton(DialogInterface.BUTTON_NEGATIVE, "Allow Once", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                pressedButtonID = AAM_USER_AUTH_REQD;
                dialogSemaphore.release();
                }
            });
        mDialog.setButton(DialogInterface.BUTTON_NEUTRAL, "Deny", new DialogInterface.OnClickListener() {
            @Override
            public void onClick(DialogInterface dialog, int which) {
                pressedButtonID = AAM_DENY;
                dialogSemaphore.release();
                }
            });
            
            
        mDialog.setCancelable(false);
        mDialog.getWindow().setType(WindowManager.LayoutParams.TYPE_SYSTEM_ALERT);
        mDialog.show();
    }
    
    public int displayConfirmationDialog()  //should be called from non-UI thread
    {
        pressedButtonID = AAM_ERROR;
        runOnUiThread(mMyDialog);
        try
        {
            dialogSemaphore.acquire();
        }
        catch (InterruptedException e)
        {
        }
        return pressedButtonID;
    }
    
    
    @Override
    public IBinder onBind(Intent intent) {
        Log.d("CROSSAPPBLE", "/packages/apps/Bluetooth/src/.../btservice/AamService.java=>onBind()");
        // Return the interface
        return mBinder;
    }
    
    public boolean onUnbind(Intent intent) {
        Log.d("CROSSAPPBLE", "/packages/apps/Bluetooth/src/.../btservice/AamService.java=>onUnbind()");
        cleanup();
        return super.onUnbind(intent);
    }
        
    private static class BluetoothAamBinder extends IBluetoothAam.Stub {
        private AamService mService;
        
        public BluetoothAamBinder(AamService svc) {
            mService = svc;
        }
        
        public boolean cleanup() {
            mService = null;
            return true;
        }
        
        public AamService getService() {
            if (mService  != null) {
                return mService;
            }
            return null;
        }
        
        public boolean checkAamAuth(String appID, String devID, String appName, String devName) {
            AamService service = getService();
            if (service == null) return false;
            return service.checkAamAuth(appID, devID, appName, devName);
        }
    }
    
    boolean checkAamAuth(String appID, String devID, String appName, String devName) {
        mAamAppID = appID;
        mAamAppName = appName;
        mAamDevID = devID;
        mAamDevName = devName;
        
        Log.d("CROSSAPPBLE", "/packages/apps/Bluetooth/src/.../btservice/AamService.java=>checkAamAuth() - appID=" + mAamAppID + " devID=" + mAamDevID + " appName=" + mAamAppName + " devName=" + mAamDevName);
        
        int status = aamCheckAccessNative(mAamAppID, mAamDevID);
        
        Log.d("CROSSAPPBLE", "/packages/apps/Bluetooth/src/.../btservice/AamService.java=>checkAamAuth() - Received status=" + status);
        
        if (status == AAM_OK) {
            return true;
        } else if (status == AAM_DENY) {
            return false;
        } else if (status == AAM_USER_AUTH_REQD) {
            /* Request user auth.
            If outcome is Allow, then call aamAddAppDevRecordNative and add AAM_OK to record. Return true.
            If outcome is Deny, then call aamAddAppDevRecordNative and add AAM_DENY to record. Return false.
            If outcome is Allow Once, then call aamAddAppDevRecordNative and add AAM_USER_AUTH_REQD to record. Return true.
            */
            
            mUserChoice = displayConfirmationDialog();
            
            Log.d("CROSSAPPBLE", "/packages/apps/Bluetooth/src/.../btservice/AamService.java=>checkAamAuth() - User choice was " + mUserChoice);
            
            if (mUserChoice == AAM_OK) {
                aamAddAppDevRecordNative(appID, devID, AAM_OK);
                return true;
            } else if (mUserChoice == AAM_USER_AUTH_REQD) {
                aamAddAppDevRecordNative(appID, devID, AAM_USER_AUTH_REQD);
                return true;
            } else if (mUserChoice == AAM_DENY) {
                aamAddAppDevRecordNative(appID, devID, AAM_DENY);
                return false;
            }
            
            Log.e("CROSSAPPBLE", "/packages/apps/Bluetooth/src/.../btservice/AamService.java=>checkAamAuth() - User choice error");
            return false;
        } 
        
        Log.e("CROSSAPPBLE", "/packages/apps/Bluetooth/src/.../btservice/AamService.java=>checkAamAuth() - Error status=" + status);
        
        return false;
    }
    
    void cleanup() {
        if (mBinder != null) {
            mBinder.cleanup();
            mBinder = null;  //Do not remove. Otherwise Binder leak!
        }
    }
    
    private native boolean aamInitNative();
    private native boolean aamAddAppDevRecordNative(String appID, String devID, int permLevel);
    private native boolean aamDeleteAppDevRecordNative(String appID, String devID);
    private native boolean aamModifyAppDevRecordNative(String appID, String devID, int permLevel);
    private native int aamCheckAccessNative(String appID, String devID);
}


