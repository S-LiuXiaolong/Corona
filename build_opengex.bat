@echo off
git submodule update --init External\src\OpenGEX
mkdir External\build\OpenGEX
pushd External\build\OpenGEX
cmake -S ..\..\src\OpenGEX -B .
cmake --build . --config Release
cmake --build . --config Debug
popd