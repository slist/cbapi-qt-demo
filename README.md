# Carbon Black Qt API demo

## What can it do? 

* Lists policies with indicators that tell you things like number of rules, UI `ON/OFF`, number of devices, GoLive `ON/OFF`.

* Let's you compare policies side-by-side, even in different customer instances — you can select two policies, see exactly where there are differences and what they are 

* Copy policies between instances — if you have a policy applied to one customer and you want to apply that same policy to another 

* Policies that are applied can then be viewed in the product UI under Enforce > Policies 

## For Microsoft Windows users

A Zip file containing cbapi-qt-demo.exe is available in [Releases section](https://github.com/slist/cbapi-qt-demo/releases).

## For MacOS users

A DMG file containing cbapi-qt-demo is available in [Releases section](https://github.com/slist/cbapi-qt-demo/releases).


## How to build on Linux CentOS

### Install required packages
``` sh
sudo yum install qt5-qtbase qt5-qtbase-devel vim-X11 gcc-c++ qt5-linguist git
```
### Use QT5 by default
``` sh
sudo ln -s /usr/bin/qmake-qt5 /usr/bin/qmake
sudo ln -s /usr/bin/lrelease-qt5 /usr/bin/lrelease
``` 
### Build
``` sh
qmake-qt5
make
./cbapi-qt-demo
```

## How to build on Linux Ubuntu 23.04

Install qt6-wayland, and follow instructions for Ubuntu 22.04, 22.10 (Qt6).
``` sh
sudo apt install qt6-wayland
```


## How to build on Linux Ubuntu 22.04 LTS or 22.10 (Qt5: recommended version)

### Install required packages and source code
``` sh
sudo apt install -y qtcreator qtbase5-dev qt5-qmake cmake git
git clone https://github.com/slist/cbapi-qt-demo.git
```

### Build
``` sh
cd cbapi-qt-demo
qmake
make
./cbapi-qt-demo
``` 

## How to build on Linux Ubuntu 22.04 LTS or 22.10 (Qt6: experimental)
* Error handling in networking functions seems broken, but it will have no effect if there's no network error.

### Install required packages
``` sh
sudo apt-get install g++ make qt6-tools-dev-tools assistant-qt6 linguist-qt6 designer-qt6 qt6-base-dev qt6-base-dev-tools libqt6core5compat6 libqt6core5compat6-dev git
```

### Build
``` sh
qmake6
make -j4
./cbapi-qt-demo
```


## How to build on Linux Ubuntu 20.04 LTS

### Install required packages
``` sh
sudo apt-get install g++ make qt5-default qt5-qmake git
```
### Build
``` sh
qmake-qt5
make
./cbapi-qt-demo
``` 

### Create a Destop Launcher on Ubuntu

The easiest way to add this app to Ubuntu launcher is to create an entry in Gnome menu using alacarte, and then add it to favorites.

``` sh
sudo apt-get install alacarte
alacarte
``` 

Create a "new Item" and point to the App and Icon (img/qt-cbapi.png).

Click on Top Left of your screen on "Activities", search your newly created application, right click to "Add to Favorites".

## Warning

Warning : Settings/passwords are stored in plain text using unsecure [QSettings](https://doc.qt.io/qt-6/qsettings.html) of Qt framework.

For Linux, see ~/.config/VMware\ Carbon\ Black/cbapi-qt-demo.conf

For Windows, see HKEY_CURRENT_USER\Software\VMware Carbon Black\cbapi-qt-demo

For Mac, see $HOME/Library/Preferences/com/VMware\ Carbon\ Black/cbapi-qt-demo

## Resources

* Developer Network website: https://developer.carbonblack.com - contains reference documentation, video tutorials, and how-to guides on how to integrate with Carbon Black products
* CbAPI Python module: https://cbapi.readthedocs.io - our Python module that makes interfacing with the Carbon Black APIs easy. Check it out if you're using Python to interface with Cb products.
* CbAPI Python module source code: https://github.com/carbonblack/cbapi-python - the source code to the CbAPI Python module, including example scripts

## Support

If you have questions on the Carbon Black Qt API, please contact me at slist@vmware.com

## License

Use of this software is governed by the MIT license.
[![license](https://img.shields.io/badge/license-MIT-red.svg?style=flat)](https://raw.githubusercontent.com/slist/cbapi-qt-demo/refs/heads/master/LICENSE)
