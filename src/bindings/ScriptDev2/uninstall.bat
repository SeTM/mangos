@echo off
set PATH=%WINDIR%;%WINDIR%\system;%WINDIR%\system32;%PATH%
regsvr32 /u ToFroWin.dll
