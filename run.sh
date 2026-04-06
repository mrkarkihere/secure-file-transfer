#!/bin/bash

# check if build directory exists
if [ ! -d "build" ]; then
    echo "build dir not found, running build.sh..."
    chmod +x build.sh
    ./build.sh 
    exit 0
fi

if [ -f "build/server" ] && [ -f "build/client" ]; then
    echo "server and client already exist in build/"
else 
    echo "server or client missing, running build.sh..."
    chmod +x build.sh
    ./build.sh
fi