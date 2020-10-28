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

i. Create a VM using the `original.iso` image file. Make sure you provide sufficient memory (at least 2GB), otherwise it will lag noticeably. If you will be downloading an app from Google Play onto the VM, then choose NAT. If you will be installing the app via ADB, then Host-only network is sufficient.  
ii. Turn on your BLE device.  
iii. Turn on Bluetooth from within the Android Settings (on the VM) and make sure it picks up your BLE device.  
iv. Install an app that you know will talk to your BLE device (if you can't get anything, download nRFConnect from the App Store).  
v. Launch the app, connect it with your device and read/write data. Observe the user interface.  

Close the VM and create a new VM with the same specifications as in step (i) using the `poc.iso` image. Repeat steps (ii) to (v) with the new VM. You will see that the POC asks for explicit user confirmation before it allows the app to read or write information from the BLE device. Kill the app and re-launch it to observe that the user's choice persists between app launches.

### Disclaimer
This POC is intended purely to demonstrate the efficacy of our proposed solution and should not be used in production code.
