#!/bin/bash

make

./main_O0 mat1 mat2
./main_O1 mat1 mat2
./main_O2 mat1 mat2
./main_m64 mat1 mat2

echo $HOSTNAME
