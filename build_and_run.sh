#!/bin/bash

clear
cd build
make -j4 CBRMain
./projects/CBRMain/CBRMain config/config.json