# Raspberry pi zero w

## Setup Guide: 
https://styxit.com/2017/03/14/headless-raspberry-setup.html

* Setup  
```cd /Volumes/boot```  
```touch ssh```  
```touch wpa_supplicant.conf```  

```
country=US
ctrl_interface=DIR=/var/run/wpa_supplicant GROUP=netdev
update_config=1
network={
    ssid="<wifi-name>"
    scan_ssid=1
    psk="<password>"
    key_mgmt=WPA-PSK
}
```

* Connect  
```ssh pi@raspberrypi.local```  
```<default password>: raspberry```  

```ping raspberrypi.local```  
```rpm-config```  

* Shutdown: 
```sudo shutdown -h now```  

## Installation

* CMake and C++ boost library:  

```sudo apt-get install cmake```  
```sudo apt-get install libboost-all-dev```  

* Opencv 4.0.0  

https://www.pyimagesearch.com/2018/09/26/install-opencv-4-on-your-raspberry-pi/  

Note (Do not use NEON and VFPV3 for zero w):   
```cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local -D OPENCV_EXTRA_MODULES_PATH=~/opencv_contrib/modules -D BUILD_TESTS=OFF -D OPENCV_ENABLE_NONFREE=ON -D INSTALL_PYTHON_EXAMPLES=OFF -D BUILD_EXAMPLES=OFF ..```

## Commands

* Disk space:  
```df -h```  

* Compress and Extract:  
```tar cvzf <filename>.zip <filename>```  
```tar -xvzf <filename>```

* Transfer:  
```scp <filename> pi@raspberrypi.local:~/```  

* Tmux:  
```Ctrl + b  %```  
```Ctrl + b  o```  
```Ctrl + b  x```  


## Projects

* os  
https://jsandler18.github.io/

* rtos  
https://github.com/Forty-Tw0/RaspberryPi-FreeRTOS




