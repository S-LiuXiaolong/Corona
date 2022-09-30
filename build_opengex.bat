@echo off
git submodule update --init External\src\OpenGEX
mkdir External\build\OpenGEX
pushd External\build\OpenGEX
cmake -S ..\..\src\OpenGEX -B .
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
popd