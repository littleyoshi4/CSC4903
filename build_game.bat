@echo off
cls

set DRIVE_LETTER=%1:
set GAME_PATH=%DRIVE_LETTER%\Game_Engine
set PATH=%DRIVE_LETTER%\MinGW\bin;%DRIVE_LETTER%\MinGW\msys\1.0\bin;%GAME_PATH%\Ogre_1.8.1\bin;%GAME_PATH%\boost_1.57.0\stage\lib;%GAME_PATH%\ogre-deps\src\bin\release;c:\Windows;c:\Windows\system32

make GAME_DRIVE_LETTER="%DRIVE_LETTER%"
