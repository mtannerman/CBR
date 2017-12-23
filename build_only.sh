#!/bin/bash

clear
cd build
make CBRMain 2>&1 | ../compileparser.py