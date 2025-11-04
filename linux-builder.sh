#!/bin/bash
# Cross-platform build script for the C++ LAN server/client project
# Builds both Linux and Windows executables (if tools available)
# Usage: bash ./build.sh

set -e  # Stop script on first error

# Compilers
LINUX_CXX=g++

# Common flags
COMMON_FLAGS="-pthread"

# Source files
SERVER_SRC="src/server.cpp"
CLIENT_SRC="src/client.cpp"

# Output files
SERVER_LINUX="./server-linux"
CLIENT_LINUX="./client-linux"

echo "🧹 Cleaning old builds..."
rm -f "$SERVER_LINUX" "$CLIENT_LINUX"

echo "⚙️  Building Linux executables..."
$LINUX_CXX $SERVER_SRC -o $SERVER_LINUX $COMMON_FLAGS
$LINUX_CXX $CLIENT_SRC -o $CLIENT_LINUX $COMMON_FLAGS
echo "✅ Linux builds complete!"

echo ""
echo "🎉 Linux builds finished!"
echo "----------------------------------"
echo "Linux  Server: $SERVER_LINUX"
echo "Linux  Client: $CLIENT_LINUX"
echo "----------------------------------"
