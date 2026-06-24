# EXE Executor
## by Null

## What the heck is this?
EXE Executor is a Linux app that allows you to run Windows Applications (.exe files) on, well, Linux
This app acts as a dashboard layer for Wine, while adding more features and performance tweaks

## What exactly does this app add to Wine?
This app adds:
* Linux X11 Display Tree Listener
* Diagnostc GUI Console
* Audio Management
* Process Automation

## Why should this app be chosen over Wine itself?
Well, in addition to the features above, this app also adds:
* 'Administrator Mode' Toggle
* DLL Overrides (To stop freezing and white screens)
* Real-Time Diagnostics
* Display Server Integration

This app also adds a slick cyan cyber-neon UI, and cool background music (made by Posifurg)

## How to install/build
**These packages, libraries, and compilation tools are required to build this app:**
* CMake (Minimum verison 3.16)
* PKG-Config
* C++ Compiler
* Qt6 Widgets Dev. Modules
* Qt6 Multimedia Dev. Modules
* X11 Dev. Library Headers
* Wine / Wine-Stating
* Winetricks
* GStreamer Media Core + Plugin Libraries

**To compile, in your terminal, run:**
`bash`,
`mkdir build`,
`cd build`,
`cmake ..`, and finally, 
`make && ./exe-executor`

### Credits:
* All code and development by Null
* Background music made by Posifurg

... that's it
