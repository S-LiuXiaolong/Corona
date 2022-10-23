@echo off
mkdir External\build\bullet3
pushd External\build\bullet3
cmake -S ..\..\src\bullet3 -B .
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
popd