# Firefox ext
Install firefox extensions from the commandline

## Why?
Ever setup a new machine, or reinstalled firefox? Its a pain to have to install every extension every time, to automate this firefoxext allows you to write scripts to install all of the extensions you need with only a command.

## Notice
Currently firefox ext only supports linux. Macos, and Windows support should come soon


## Install from source
First look in the PACKAGES.md file and make sure everthing is installed, then clone this repo with git like below:
```bash
git clone https://github.com/Interfiber/firefoxext.git
```
then ```cd``` into the cloned folder and run ```make && make install```, this will build and install firefoxext.