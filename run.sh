#!/bin/bash
target="ConsoleAppExample"
cmake -Bbuild
cmake --build build --target $target
./build/${target}_artefacts/$target