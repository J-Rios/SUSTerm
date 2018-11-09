# SUSTerm
A Simple Universal Serial Terminal developed using QT5.

#### Install Instructions:
Windows:
  * Download [last version](https://github.com/J-Rios/SUSTerm/releases/download/v1.1/Windows_SUSTerm_v110.zip).
  * Unzip it and place in C:\Program File (x86)
  * Create a desktop shortcut of C:\Program File (x86)\SUSTerm\SUSTerm.exe

Linux:
  * Download last version:
  ```
  $ cd ~
  $ wget https://github.com/J-Rios/SUSTerm/releases/download/v1.1/Linux_SUSTerm_v110.zip
  ```
  
  * Unzip it, give execution permission and place it to user binaries apps dir:
  ```
  $ unzip Linux_SUSTerm_v110.zip
  $ sudo chmod +x Linux_SUSTerm_v110/SUSTerm
  $ sudo cp -a Linux_SUSTerm_v110/SUSTerm /usr/bin/
  ```
  
  * Remove unnecessary used files:
  ```
  $ rm -rf Linux_SUSTerm_v110*
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

#### Notes:

- QT version is 5.11.2.
- The project was written in QtCreator 4.7.1.
- The implementation uses QSerialPort class to manage Serial ports.
