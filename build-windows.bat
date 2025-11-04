@echo off
REM -------------------------------------------
REM Build script for src/client.cpp on Windows
REM Using MinGW
REM -------------------------------------------

echo 🏗️  Cleaning old build...
IF EXIST client.exe (
    del client.exe
)

echo ⚙️  Compiling src/client.cpp...
REM -lws2_32 links Winsock library
REM -static makes the executable standalone
g++ src\client.cpp -o client.exe -lws2_32 -static -pthread

IF %ERRORLEVEL% NEQ 0 (
    echo ❌ Build failed!
    pause
    exit /b %ERRORLEVEL%
)

echo ✅ Build complete!
echo Output: client.exe
pause
