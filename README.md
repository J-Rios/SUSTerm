# SUSTerm

![SUSTerm GUI](https://github.com/J-Rios/SUSTerm/raw/master/SUSTerm/res/SUSTerm_GUI.png)

SUSTerm (Simple Universal Serial Terminal) is a free software and GNU-GPL licensed Serial terminal developed using QT5, and focused in be minimalist and efficiently fast and easy to use, while contains some more advanced features that can be enabled.

### Characteristics:
  * Minimalist, robust, simple and fast to use.
  * Friendly UI similar to Arduino Serial Monitor.
  * Realtime Serial Ports recognized.
  * Close a port and connect to other without exit the app.
  * Bauds and End-Of-Line selectors.
  * Sent commands history, navigate and recover previous sent commands with arrow keys.
  * Can show Timestamp of received data (on each \n).
  * Can show received data in ASCII, HEX or Both format at same time.
  * Open Source and Free software.

### Install Instructions:
#### Windows:
  * Download [last version](https://github.com/J-Rios/SUSTerm/releases/download/v1.7.0/Windows_SUSTerm_v170.zip).
  * Unzip it and place in "C:\Program File (x86)"
  * Create a desktop shortcut of "C:\Program File (x86)\SUSTerm\SUSTerm.exe"

#### Linux:
  * Install required tools:
  ```
  $ sudo apt update
  $ sudo apt-get install -y unzip fuse
  ```

  * Download last version:
  ```
  $ cd ~
  $ wget https://github.com/J-Rios/SUSTerm/releases/download/v1.7.0/Linux_SUSTerm_v170.zip
  ```
  
  * Unzip it, give execution permission to install script and run it:
  ```
  $ unzip Linux_SUSTerm_v170.zip
  $ cd Linux_SUSTerm_v170
  $ sudo chmod +x ./install
  $ sudo ./install
  ```
  
  * Remove unnecessary used files:
  ```
  $ cd ~
  $ rm -rf Linux_SUSTerm_v170*
  ```
  
  * Add actual user to dialout group to allow open serial ports without been root and reboot system to apply:
  ```
  $ sudo usermod -aG dialout $USER
  $ sudo reboot
  ```
  
  * Run SUSTerm:
  ```
  $ SUSTerm
  ```

### Uninstall Instructions:
#### Windows:
  * Simply remove directory "C:\Program File (x86)\SUSTerm\".

#### Linux:
  * Remove Application related files:
  ```
  sudo rm -f /usr/bin/SUSTerm
  sudo rm -f /usr/share/applications/SUSTerm.desktop
  sudo rm -f /usr/share/icons/hicolor/256x256/apps/SUSTerm.png
  ```

### Donate:
Do you like this software? Buy me a coffee:
  
  Paypal - [https://www.paypal.me/josrios](https://www.paypal.me/josrios)
  
  BTC    - 3N9wf3FunR6YNXonquBeWammaBZVzTXTyR

### About build in Linux:

To build this QT project in Linux, remeber that you need to install QSerialPort module.

- Install QT:
```
sudo apt-get -y install build-essential
sudo apt-get -y install qtcreator
sudo apt-get -y install qt5-default
sudo apt-get -y install qtbase5-private-dev
```

- Get QSerialPort module and install it:
```
cd ~
mkdir qtserialport-build
git clone git://code.qt.io/qt/qtserialport.git
cd qtserialport
git checkout v5.11.2
cd ../qtserialport-build
qmake ../qtserialport/qtserialport.pro
make
sudo make install
```

### Notes:
- QT version is 5.11.2.
- The project was written in QtCreator 4.7.1.
- The implementation uses QSerialPort class to manage Serial ports.
