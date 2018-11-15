#!/bin/bash
ndk-build
adb push ../libs/armeabi-v7a/libmyhook.so /data/local/kk/
