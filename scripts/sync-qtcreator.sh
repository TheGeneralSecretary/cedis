#!/usr/bin/env bash

cd ../
find . \( -name build \) -prune -o \( -name '*.cpp' -or -name '*.c' -or -name '*.h' -or -name '*.S' -or -name '*.json' -or -name 'CMakeLists.txt' -or -name '.keep' -or -name 'Makefile' -or -name '*.sh' -or -name '.*' -or -name 'COPYING' -or -name 'README' \) -print > cinit.qt.files
