echo off
rem this file will add or change path for glut just run it

setx OpenGL_Inc 		%cd%\inc

setx OpenGL_Lib_86_D 		%cd%\lib\x86\Debug
setx OpenGL_Lib_86_R 		%cd%\lib\x86\Release


setx OpenGL_Lib_64_D 		%cd%\lib\x64\Debug
setx OpenGL_Lib_64_R 		%cd%\lib\x64\Release