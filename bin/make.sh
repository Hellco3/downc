#!/bin/env bash

cd ../build
cmake ..
make
cd ../bin
./OsdClientTool.sh -init