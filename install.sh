#!/bin/bash
cd ./build
cmake . ; make 
rm -rf CMakeFiles cmake_install.cmake CMakeCache.txt Makefile
mv baracle ../
echo -e "\nDone!"
