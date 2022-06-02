# Carbon Black Qt API demo

## What can it do? 

* Lists policies with indicators that tell you things like number of rules, UI `ON/OFF`, GoLive `ON/OFF` 

* Let's you compare policies side-by-side, even in different customer instances — you can select two policies, see exactly where there are differences and what they are 

* Copy policies between instances — if you have a policy applied to one customer and you want to apply that same policy to another 

* Policies that are applied can then be viewed in the product UI under Enforce > Policies 


## How to build on Linux CentOS

### Install required packages
``` sh
sudo yum install qt5-qtbase qt5-qtbase-devel vim-X11 gcc-c++ qt5-linguist
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
./cb
```
## How to build on Linux Ubuntu

### Install required packages
``` sh
sudo apt-get install g++ make qt5-default qt5-qmake
```
### Build
``` sh
qmake-qt5
make
./cb
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

Warning : Settings/passwords are stored in plain text.

For Linux, see ~/.config/Carbon Black/cb.conf

For Windows, see HKEY_CURRENT_USER\Software\Carbon Black\cb

## Resources

* Developer Network website: https://developer.carbonblack.com - contains reference documentation, video tutorials, and how-to guides on how to integrate with Carbon Black products
* CbAPI Python module: https://cbapi.readthedocs.io - our Python module that makes interfacing with the Carbon Black APIs easy. Check it out if you're using Python to interface with Cb products.
* CbAPI Python module source code: https://github.com/carbonblack/cbapi-python - the source code to the CbAPI Python module, including example scripts

## Support

If you have questions on the Carbon Black Qt API, please contact me at slist@vmware.com

## License

Use of this software is governed by the license found in LICENSE.md.
