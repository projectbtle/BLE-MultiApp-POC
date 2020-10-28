#include "com_android_bluetooth.h"
#include "hardware/bt_aam.h"
#include "utils/Log.h"
#include "android_runtime/AndroidRuntime.h"

#include <string.h>

#include <cutils/log.h>
#define info(fmt, ...)  ALOGI ("%s(L%d): " fmt,__FUNCTION__, __LINE__,  ## __VA_ARGS__)
#define debug(fmt, ...) ALOGD ("%s(L%d): " fmt,__FUNCTION__, __LINE__,  ## __VA_ARGS__)
#define warn(fmt, ...) ALOGW ("WARNING: %s(L%d): " fmt "##",__FUNCTION__, __LINE__, ## __VA_ARGS__)
#define error(fmt, ...) ALOGE ("ERROR: %s(L%d): " fmt "##",__FUNCTION__, __LINE__, ## __VA_ARGS__)
#define asrt(s) if(!(s)) ALOGE ("%s(L%d): ASSERT %s failed! ##",__FUNCTION__, __LINE__, #s)
    
static const bt_interface_t *btIf = NULL;
static const btaam_interface_t *sAamIf = NULL;

namespace android {
    
static jboolean aamInitNative(JNIEnv* env, jobject object)
{
    bool status=true;
    
    info("CROSSAPPBLE. /packages/apps/Bluetooth/jni/com_android_bluetooth_btservice_AamService.cpp=>aamInitNative. ");
    
    if ( (btIf = getBluetoothInterface()) == NULL) {
        error("Bluetooth module is not loaded");
        status = false;
        return status;
    }
    
    if ( (sAamIf = (btaam_interface_t *)
          btIf->get_aam_interface()) == NULL) {
        error("Failed to get Bluetooth AAM Interface");
        status = false;
        return status;
    }
    
    sAamIf->aam_init(); 
    
    return status;
}

static jboolean aamAddAppDevRecordNative(JNIEnv* env, jobject object,
                                     jstring appID, jstring devID, jint permLevel )
{
    bool status=true;

    if ( (btIf = getBluetoothInterface()) == NULL) {
        error("Bluetooth module is not loaded");
        status = false;
        return status;
    }
    
    if ( (sAamIf = (btaam_interface_t *)
          btIf->get_aam_interface()) == NULL) {
        error("Failed to get Bluetooth AAM Interface");
        status = false;
        return status;
    }
    
    info("CROSSAPPBLE. /packages/apps/Bluetooth/jni/com_android_bluetooth_btservice_AamService.cpp=>aamAddAppDevRecordNative. ");
    
    
    // Get a pointer to an array of bytes representing the strings in modified UTF-8 encoding
    const char* app_id = env->GetStringUTFChars(appID, NULL);
    const char* dev_id = env->GetStringUTFChars(devID, NULL);
    
    // Log str
    ALOGI("%s", app_id);
    ALOGI("%s", dev_id);
    
    
    status = sAamIf->add_app_dev_record(app_id, dev_id, permLevel); 
    
    // Inform the VM that the native code no longer needs access to the strings.
    env->ReleaseStringUTFChars(appID, app_id);
    env->ReleaseStringUTFChars(devID, dev_id);
    
    return status;
}


static jboolean aamDeleteAppDevRecordNative(JNIEnv* env, jobject object,
                                     jstring appID, jstring devID )
{
    bool status=true;

    if ( (btIf = getBluetoothInterface()) == NULL) {
        error("Bluetooth module is not loaded");
        status = false;
        return status;
    }
    
    if ( (sAamIf = (btaam_interface_t *)
          btIf->get_aam_interface()) == NULL) {
        error("Failed to get Bluetooth AAM Interface");
        status = false;
        return status;
    }
    
    info("CROSSAPPBLE. /packages/apps/Bluetooth/jni/com_android_bluetooth_btservice_AamService.cpp=>aamDeleteAppDevRecordNative. ");
    
    
    // Get a pointer to an array of bytes representing the strings in modified UTF-8 encoding
    const char* app_id = env->GetStringUTFChars(appID, NULL);
    const char* dev_id = env->GetStringUTFChars(devID, NULL);
    
    // Log str
    ALOGI("%s", app_id);
    ALOGI("%s", dev_id);
    
    
    status = sAamIf->delete_app_dev_record(app_id, dev_id); 
    
    // Inform the VM that the native code no longer needs access to the strings.
    env->ReleaseStringUTFChars(appID, app_id);
    env->ReleaseStringUTFChars(devID, dev_id);
    
    return status;
}


static jboolean aamModifyAppDevRecordNative(JNIEnv* env, jobject object,
                                     jstring appID, jstring devID, jint permLevel )
{
    bool status=true;

    if ( (btIf = getBluetoothInterface()) == NULL) {
        error("Bluetooth module is not loaded");
        status = false;
        return status;
    }
    
    if ( (sAamIf = (btaam_interface_t *)
          btIf->get_aam_interface()) == NULL) {
        error("Failed to get Bluetooth AAM Interface");
        status = false;
        return status;
    }
    
    info("CROSSAPPBLE. /packages/apps/Bluetooth/jni/com_android_bluetooth_btservice_AamService.cpp=>aamModifyAppDevRecordNative. ");
    
    
    // Get a pointer to an array of bytes representing the strings in modified UTF-8 encoding
    const char* app_id = env->GetStringUTFChars(appID, NULL);
    const char* dev_id = env->GetStringUTFChars(devID, NULL);
    
    // Log str
    ALOGI("%s", app_id);
    ALOGI("%s", dev_id);
    
    
    status = sAamIf->modify_app_dev_record(app_id, dev_id, permLevel); 
    
    // Inform the VM that the native code no longer needs access to the strings.
    env->ReleaseStringUTFChars(appID, app_id);
    env->ReleaseStringUTFChars(devID, dev_id);
    
    return status;
}




static jint aamCheckAccessNative(JNIEnv* env, jobject object,
                                     jstring appID, jstring devID )
{
    int status=0;

    if ( (btIf = getBluetoothInterface()) == NULL) {
        error("Bluetooth module is not loaded");
        status = -1;
        return status;
    }
    
    if ( (sAamIf = (btaam_interface_t *)
          btIf->get_aam_interface()) == NULL) {
        error("Failed to get Bluetooth AAM Interface");
        status = -1;
        return status;
    }
    
    info("CROSSAPPBLE. /packages/apps/Bluetooth/jni/com_android_bluetooth_btservice_AamService.cpp=>aamCheckAccessNative. ");
    
    
    // Get a pointer to an array of bytes representing the strings in modified UTF-8 encoding
    const char* app_id = env->GetStringUTFChars(appID, NULL);
    const char* dev_id = env->GetStringUTFChars(devID, NULL);
    
    // Log str
    ALOGI("%s", app_id);
    ALOGI("%s", dev_id);
    
    
    status = sAamIf->check_app_dev_access(app_id, dev_id); 
    
    // Inform the VM that the native code no longer needs access to the strings.
    env->ReleaseStringUTFChars(appID, app_id);
    env->ReleaseStringUTFChars(devID, dev_id);
    
    return status;
}


// JNI functions defined in AamService class.
static JNINativeMethod sMethods[] = {
    {"aamInitNative", "()Z", (void *) aamInitNative},
    {"aamAddAppDevRecordNative", "(Ljava/lang/String;Ljava/lang/String;I)Z", (void *) aamAddAppDevRecordNative},
    {"aamDeleteAppDevRecordNative", "(Ljava/lang/String;Ljava/lang/String;)Z", (void *) aamDeleteAppDevRecordNative},
    {"aamModifyAppDevRecordNative", "(Ljava/lang/String;Ljava/lang/String;I)Z", (void *) aamModifyAppDevRecordNative},
    {"aamCheckAccessNative", "(Ljava/lang/String;Ljava/lang/String;)I", (void *) aamCheckAccessNative}
};


int register_com_android_bluetooth_btservice_AamService(JNIEnv* env)
{
    return jniRegisterNativeMethods(env, "com/android/bluetooth/btservice/AamService",
                                    sMethods, NELEM(sMethods));
}
}