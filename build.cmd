@echo off
setlocal
cls

rem Build script for librefactory
rem Despite the nature of being the "engine" this builds a binary app
rem And the end user (you) can develop Lua extensions to give this app
rem a life! Just run '' build '' in a Developer Command Prompt and you'll
rem have a working game in moments!

set "ROOT=%~dp0"
set "SRC=%ROOT%src"
set "DEPS=%ROOT%deps"
set "OUT=%ROOT%out"
rem Hardcoding the location of DirectX headers assuming windows XP ... edit if not matching yours!
set "INC=/I"%ROOT%include" /I"%DEPS%\miniaudio" /I"%DEPS%\lua" /I"C:\Program Files\Microsoft DirectX SDK (August 2007)\Include""

rem Need this because Lua produces many warnings
set "DEFINES=/D_WIN32_WINNT=0x0501 /DWINVER=0x0501 /D_CRT_SECURE_NO_WARNINGS"
set "CFLAGS=/O2 /W3 /nologo %DEFINES% %INC%"
  
if not exist "%OUT%" md "%OUT%"

echo Building Lua...
pushd "%DEPS%\lua"
for /f "delims=" %%f in ('dir /b /a-d *.c ^| findstr /v /i "lua.c luac.c"') do (
    cl %CFLAGS% /MT /c %%f /Fo"%OUT%\%%~nf.obj"
)
popd

echo Building game sources...
for /f "delims=" %%f in ('dir /b /a-d "%SRC%\*.cpp"') do (
    cl %CFLAGS% /MT /c "%SRC%\%%f" /Fo"%OUT%\%%~nf.obj"
)

echo Linking...
link /nologo /OUT:"%OUT%\librefactory.exe" /subsystem:windows ^
    "%OUT%\*.obj" /LIBPATH:"C:\Program Files\Microsoft DirectX SDK (August 2007)\Lib\x86" ^
    kernel32.lib user32.lib winmm.lib gdi32.lib opengl32.lib d3d8.lib

echo Done.
