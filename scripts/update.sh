#!/usr/bin/env bash

if [ -d "../.git" ]; then
    cd ..
    git pull
    git submodule update --init --recursive --remote
    cd script/
    ./build
else
    echo "Not a git repository"
    exit
fi
