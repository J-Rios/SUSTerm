# SUSTerm
A Simple Universal Serial Terminal developed using QT5.

#### Install Instructions:
Windows:
  * Download [last version](https://github.com/J-Rios/SUSTerm/releases/download/v1.0/Windows_SUSTerm_v100.zip).
  * Unzip it and place in C:\Program File (x86)
  * Create a desktop shortcut of C:\Program File (x86)\SUSTerm\SUSTerm.exe

Linux:
  * Download last version:
  ```
  $ cd ~
  $ wget https://github.com/J-Rios/SUSTerm/releases/download/v1.0/Linux_SUSTerm_v100.zip
  ```
  
  * Unzip it, give execution permission to install script and execute it with sudo:
  ```
  $ unzip Linux_SUSTerm_v100.zip
  $ cd Linux_SUSTerm_v100
  $ chmod +x install
  $ sudo ./install
  ```
  
  * Remove unnecessary used files:
  ```
  $ cd ~
  $ rm -rf Linux_SUSTerm_v100.*
  ```
  
  * Run SUSTerm:
  ```
  $ SUSTerm
  ```

#### Notes:

- QT version is 5.11.2.
- The project was written in QtCreator 4.7.1.
- The implementation uses QSerialPort class to manage Serial ports.
