#!/bin/bash

clear
cd build
# make CBRMain 2>&1 | ../../compileparser/cp/main.py


../../compileparser/cp/main.py "make CBRMain" "./projects/CBRMain/CBRMain config/config.json" "killall CBRMain" "../projects/"