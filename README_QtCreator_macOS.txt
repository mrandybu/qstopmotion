Building qStopMotion using Qt-Creator IDE on Linux
Last Change: 2017-07-13

1. Preparation of the development environment
================================================================================

1.1 Install necessary libraries with the package manager
--------------------------------------------------------------------------------

* clang
* qt5 libraries
* CMake
* ffmpeg
* libgphoto2

There is a high probability you are using [homebrew](https://brew.sh/) as your package manager. If this is the case, run the following command to install all the requirements.

```
brew install cmake ffmpeg libgphoto2 qt5
```


1.2 Qt Creator
--------------------------------------------------------------------------------

* Install Qt Creator from qt-project.org/downloads or run
```
brew cask install qt-creator
```

2. Access to the sources of qStopMotion
================================================================================

2.1 Download the sources (last release of qStopMotion)
--------------------------------------------------------------------------------

* Downoad qstopmotion-<version>-Source.zip or qstopmotion-<version>-Source.tar.Z
  from http://sourceforge.net/projects/qstopmotion/files/

2.2 Anonymous read only access to the repository (latest developer version)
--------------------------------------------------------------------------------

* hg clone http://hg.code.sf.net/p/qstopmotion/code qstopmotion

2.3 Developer access to the repository (read and write access)
--------------------------------------------------------------------------------

2.3.1 Create a Source Forge account
* Create a Source Forge account on https://sourceforge.net/user/registration
* Add your account to the qStopMotion developers on http://sourceforge.net/projects/qstopmotion

2.3.2 Create a .hgrc file
* Create a .hgrc file in your home directory
* Content of the file:
    [ui]
    username = Your Name <your eMail address>
    [auth]
    qstopmotion.prefix = https://hg.code.sf.net/p/qstopmotion/code
    qstopmotion.username = <your Source Forge user name>
    qstopmotion.password = <your Source Forge password>

2.3.3 Create a local repository
* hg clone https://hg.code.sf.net/p/qstopmotion/code qstopmotion

3. Building qStopMotion
================================================================================

3.1 Build
--------------------------------------------------------------------------------

* Start Qt Creator using the shortcut.
* -DCMAKE_BUILD_TYPE=Release
* Open the CMakeLists.txt file as new project.
* Build | Build Project qStopMotion
* Locking for error and warning messages

3.2 Debugging
--------------------------------------------------------------------------------

* Start Qt Creator using the shortcut.
* -DCMAKE_BUILD_TYPE=Debug
* Build | Build Project qStopMotion
* Locking for error and warning messages
* Open Options | Settings | Debugger
* On the Cdb tab enable the CDB. Normaly the path to the debugger will find
  automatically.
* Start qStopMotion in debugging mode.

4. Create source documentation
================================================================================

4.1 Install Doxygen with the package manager.
--------------------------------------------------------------------------------

* ...
