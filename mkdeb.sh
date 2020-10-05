#!/bin/bash

make all
mkdir -pv package/usr/{lib,include}

cp libchash.so package/usr/lib
cp -rf src/* package/usr/include

find package/usr/include -name "*.cpp" -exec rm -rf {} \;
find package/usr/include -name "*.o" -exec rm -rf {} \;

rm -rf package/usr/include/libchash.vcxproj
rm -rf package/usr/include/libchash.vcxproj.filters
rm -rf package/usr/include/libchash.vcxproj.user

dpkg-deb -b package libchash-1.0.deb