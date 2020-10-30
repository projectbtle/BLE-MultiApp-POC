## Proof-of-Concept Solution for Unauthorised Data Access Vulnerability on Multi-App Bluetooth Low Energy Platforms

This is a Proof-of-Concept (POC) that has been implemented according to our proposed solution for the [unauthorised data access vulnerability that has been identified for Bluetooth Low Energy](https://www.usenix.org/system/files/sec19-sivakumaran_0.pdf). Please contact the authors of that paper for further information regarding the vulnerability.

Our proposed solution involves modifications to the Bluetooth specification, which will ensure protection by default for the BLE eco-system. Advantages of our solution include:
* Full backward compatibility.
* No changes to existing layers of the BLE stack.
* No changes to applications.
* Most changes are to mature platforms with stable update capabilities.

In order to demonstrate our solution and prove its feasibility, we have modified the Android-x86 framework (which is a port of Android for x86 architectures) to reflect our proposed changes.

### Implementing the POC

#### Prerequisites
* Server/machine capable of building Android-x86 (the Android kernel needs ~8GB RAM allocated just for the heap, so you'll need a machine with quite a bit of memory). We used an Ubuntu VM with 128GB RAM and 8 CPU cores.
* VM player with BLE capabilities. For our tests, we used VMWare Workstation 14 Player (free) on a Windows 10 laptop, with a CSR adapter.
* A BLE device and associated app.


#### Building the Modified Android-x86
(*Note: Owing to the size of the Android-x86 platform, we don't include the entire source code in our repo. Instead, we provide only the new/modified files.*)

1. Download the Android-x86 source as described [here](https://www.android-x86.org/source.html). Our solution is implemented on top of the Nougat-r4 source, so download that specific release. (*Note: This can take a while!*)
2. Build Android-x86 according to the instructions given at the link above (choose `userdebug` as the `$TARGET_BUILD_VARIANT`). 
3. Save the resultant `.iso` as `original.iso`. 
2. Download the files in this repo and save them somewhere.
3. Traverse through the directory structure of the repo and replace or add the relevant files to the Android-x86 source. So, for example, we have provided a file `IBluetoothAam.aidl`, at location `/frameworks/base/core/java/android/bluetooth/`. Copy this file to the `/frameworks/base/core/java/android/bluetooth/` folder within Android-x86 (Nougat). Do the same for all the files we have included within our repo.
4. Rebuild the Android-x86 code and save the resultant `.iso` as `poc.iso`.



#### Testing the Solution
We first describe how to install the Android-x86 POC to a VM and perform initial setup functions. We next describe how to prepare the Android-x86 for running real-world apps (may not be needed in some cases). We then describe the tests.

##### Installing Android-x86 to VM
1. Create a VM using the `original.iso` image file. Make sure you provide sufficient memory (at least 2GB), otherwise it will lag noticeably. The default disk size of 20GB is fine. Choose Host-only as network adapter option.   
2. Start the VM. Choose `Install Android-x86 to harddisk`.  
3. Create two partitions: one small primary bootable partition and one logical partition. Install Android-x86 on the logical partition.  
4. Select Yes when asked about GRUB and *make sure to select Yes when asked whether to make /system read+write*.  
5. Once installation is done, choose `Reboot`. Select `Debug mode` from the boot options and hit `Enter`.   
6. Wait until the console displays a message about urandom warnings. Once it gets to that point, type `mount -o remount,rw /mnt`.  
7. Next type `cd /mnt/grub`  
8. `vi menu.lst`  
9. Go to the first line beginning with `kernel` and hit `Shift+A` to get to the end of the line. Type `vga=832 nomodeset xforcevesa`
10. Hit `Esc`, then type `:wq` and press `Enter`.
11. `cd /`
12. `umount /mnt`. Ignore warnings about being in use.
13. `reboot -f`
14. Now you can start the VM in normal mode.

##### Preparing the Android-x86 image for apps + App installation
This section assumes you have android-platform-tools. Specifically Android Debug Bridge (`adb`).
1. Within the Android-x86 VM, go to `Settings>Apps Compatibility>Enable Native Bridge` (enable this)
2. Go to `Settings>WiFi` and select `Virtual WiFi`
3. Open Terminal Emulator app and type `ifconfig` to get the IP address.
4. On the host machine, from a command prompt within `android-platform-tools` folder, type `android connect <IP of VM>`. You should get a `connected` message.
5. `adb shell` and then `su`.
6. At root prompt, type `/system/bin/enable_nativebridge`
7. An error message will likely be displayed (if not, you can skip the rest of this section). Something like `mount: 'houdini7_z.sfs'->'/system/lib64/arm64': No such file or directory`. Download the appropriate version of `houdini.sfs` from https://github.com/Rprop/libhoudini and save it within `android-platform-tools` folder.
8. Get out of adb shell by typing `exit` twice (i.e., `exit`, then hit the Enter, then type `exit` again and hit Enter button again).
9. `adb push houdini.sfs /data/local/tmp`
10. `adb shell` and then `su`.
11. At root prompt, type `/system/bin/enable_nativebridge`. This time, there should be no error.


##### Tests
1. Turn on your BLE device.  
2. Within VM, go to `Settings>Bluetooth` and turn Bluetooth on. After a few seconds, it should start listing devices in the vicinity.
3. Install an app that you know will talk to your BLE device (if you can't get anything, use Nordic's nRFConnect). 
    - To install, download the APK and save it within `android-platform-tools` folder.
    - Launch command prompt from within `android-platform-tools` folder.
    - `adb connect <IP of VM>`.
    - `adb install <apk>`.
4. Launch the app, connect it with your device and read/write data. Observe the user interface.  

Close the VM and create a new VM using the `poc.iso` image. Repeat all of the above steps with the new VM. You will see that the POC asks for explicit user confirmation before it allows the app to read or write information from the BLE device. Kill the app and re-launch it to observe that the user's choice persists between app launches.


### Disclaimer
This POC is intended purely to demonstrate the efficacy of our proposed solution and should not be used in production code.
