#!/bin/bash
# Cross-platform build script for the C++ LAN server/client project
# Builds both Linux and Windows executables (if tools available)
# Usage: bash ./build.sh

set -e  # Stop script on first error

# Compilers
WIN_CXX=x86_64-w64-mingw32-g++

# Common flags
COMMON_FLAGS="-pthread"
WIN_FLAGS="-lws2_32 -static -pthread"

# Source files
SERVER_SRC="src/server.cpp"
CLIENT_SRC="src/client.cpp"

# Output files
SERVER_WIN="./server-win.exe"
CLIENT_WIN="./client-win.exe"

echo "🧹 Cleaning old builds..."
rm -f "$SERVER_WIN" "$CLIENT_WIN"

# Check if MinGW cross-compiler exists
if command -v $WIN_CXX &> /dev/null; then
    echo "🪟 Building Windows executables..."
    $WIN_CXX $SERVER_SRC -o $SERVER_WIN $WIN_FLAGS
    $WIN_CXX $CLIENT_SRC -o $CLIENT_WIN $WIN_FLAGS
    echo "✅ Windows builds complete!"
else
    echo "⚠️  Windows cross-compiler not found! Skipping Windows builds."
    echo "👉 To install: sudo apt install mingw-w64"
fi

echo ""
echo "🎉 windows builds finished!"
echo "----------------------------------"
echo "Windows Server: $SERVER_WIN"
echo "Windows Client: $CLIENT_WIN"
echo "----------------------------------"
