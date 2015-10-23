# CuteSDR 1.05 SE with SDR MK1.5 'Andrus' Radio support #

This is a branched-off development from [CuteSdr 1.05](http://sourceforge.net/projects/cutesdr/) code created by Moe Wheatley.
The version here incorporates extras to support SDR MK1.5 'Andrus' radios more effectively than a standard version does.

The rest of the code base is kept the same way as original, so its functionality for RFSpace radios is intact.

The current stable version can be [downloaded here](http://code.google.com/p/cutesdr-se/downloads/detail?name=CuteSdrSE_1.zip) while the latest development version is [here](http://code.google.com/p/cutesdr-se/source/browse/#svn/trunk/Debug).

The project is migrated to Visual Studio 2008, but you will need additional tools to be able to build it.

In order to be able to compile the source code on VC2008 the open source version of the [Qt, the cross-platform application framework](http://qt-project.org/), has to be installed.

Two components need to be installed:
  * [Qt libraries 4.8.3 for Windows](http://releases.qt-project.org/qt4/source/qt-win-opensource-4.8.3-vs2008.exe) (VS 2008, 234 MB)
  * [Qt Visual Studio Add-in](http://releases.qt-project.org/vsaddin/qt-vs-addin-1.1.11-opensource.exe)  (does not work with the Express edition)

Check the [Qt-Project site](http://qt-project.org/downloads) for latest versions.



---


**License:**

The license information below is copied off from the source code file headers. Since the modifications to the code are only a small fraction, the original license remains put without any amendments.

```
//==========================================================================================
// + + +   This Software is released under the "Simplified BSD License"  + + +
//Copyright 2010 Moe Wheatley. All rights reserved.
//
//Redistribution and use in source and binary forms, with or without modification, are
//permitted provided that the following conditions are met:
//
//   1. Redistributions of source code must retain the above copyright notice, this list of
//	  conditions and the following disclaimer.
//
//   2. Redistributions in binary form must reproduce the above copyright notice, this list
//	  of conditions and the following disclaimer in the documentation and/or other materials
//	  provided with the distribution.
//
//THIS SOFTWARE IS PROVIDED BY Moe Wheatley ``AS IS'' AND ANY EXPRESS OR IMPLIED
//WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
//FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL Moe Wheatley OR
//CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
//SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
//ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
//ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
//The views and conclusions contained in the software and documentation are those of the
//authors and should not be interpreted as representing official policies, either expressed
//or implied, of Moe Wheatley.
```