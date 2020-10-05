#!/bin/bash

make all
mkdir -pv package/usr/{lib,include}

cp libchash.so package/usr/lib
cp -rf src/* package/usr/include

find package/usr/include -name "*.cpp" -exec rm -rf {} \;
dpkg-deb -b package libchash-1.0.deb