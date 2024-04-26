#!/bin/bash
target="ConsoleAppExample"

while test $# -gt 0; do
  case "$1" in
    -c|--reset-cache)
    shift
    echo "Removing cached files..."
    if test -f ./CMakeCache.txt; then
      rm ./CMakeCache.txt
    fi
    if test -f ./cmake_install.cmake; then
      rm ./cmake_install.cmake
    fi
    if test -d ./build; then
      rm -rf ./build
    fi
    if test -d ./CMakeFiles; then
      rm -rf ./CMakeFiles
    fi
    if test -d ./ConsoleAppExample_artefacts; then
      rm -rf ./${target}_artefacts
    fi
    ;;
    *)
    break
    ;;
  esac
done

echo "Building ${target}..."

cmake -Bbuild
cmake --build build --target $target
./build/${target}_artefacts/$target