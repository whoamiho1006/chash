#!/bin/bash

make all
mkdir -pv package/usr/lib

cp libchash.so package/usr/lib
dpkg-deb -b package libshash-1.0.deb
