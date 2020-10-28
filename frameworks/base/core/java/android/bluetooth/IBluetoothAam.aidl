package android.bluetooth;

/**
 * API for performing AAM checks.
 * @hide
 */
interface IBluetoothAam{
    boolean checkAamAuth(in String appID, in String devID, in String appName, in String devName);
}