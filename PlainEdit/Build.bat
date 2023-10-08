@echo off
mkdir "Build"
cd "Build"
qmake "../PlainEdit.pro"
mingw32-make
"release/PlainEdit"