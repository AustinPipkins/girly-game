#!/bin/bash
# Build Windows executables from Git Bash on Windows
# Usage: bash ./build-windows.sh

set -e  # Stop on first error

# Compilers
WIN_CXX=g++

# Flags
WIN_FLAGS="-lws2_32 -static -pthread"

# Source files
SERVER_SRC="src/server.cpp"
CLIENT_SRC="src/client.cpp"

# Output files
SERVER_WIN="./server-win.exe"
CLIENT_WIN="./client-win.exe"

echo "🧹 Cleaning old builds..."
rm -f "$SERVER_WIN" "$CLIENT_WIN"

# Check if g++ exists
if ! command -v $WIN_CXX &> /dev/null; then
    echo "❌ Windows g++ compiler not found!"
    echo "👉 Install MinGW-w64 and add its bin folder to PATH."
    echo "   Example: C:\\mingw-w64\\mingw64\\bin"
    exit 1
fi

echo "🪟 Building Windows executables..."
$WIN_CXX $SERVER_SRC -o $SERVER_WIN $WIN_FLAGS
$WIN_CXX $CLIENT_SRC -o $CLIENT_WIN $WIN_FLAGS
echo "✅ Windows builds complete!"

echo ""
echo "----------------------------------"
echo "Windows Server: $SERVER_WIN"
echo "Windows Client: $CLIENT_WIN"
echo "----------------------------------"
