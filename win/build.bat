@echo off
@call "../../vc_x86.bat"
@xcopy "../src/res" "res" /i /y > nul
cl ../src/bg.cpp SDL2.lib SDL2main.lib SDL2_net.lib glew32s.lib OpenGL32.lib /nologo /MT /Zi /Iinclude ^
/DGLEW_STATIC /D_CRT_SECURE_NO_WARNINGS /link /OUT:bumblegumpers.exe /LIBPATH:lib /SUBSYSTEM:CONSOLE