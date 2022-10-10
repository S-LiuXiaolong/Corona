@echo off
mkdir External\build\zlib
pushd External\build\zlib
cmake -S ..\..\src\zlib -B .
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
popd