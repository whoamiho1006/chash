#!/bin/bash

make all
mkdir -pv package/usr/{lib,include}

cp libchash.so package/usr/lib
cp src/* package/usr/include

dpkg-deb -b package libchash-1.0.deb