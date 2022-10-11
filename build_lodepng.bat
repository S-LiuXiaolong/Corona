@echo off
mkdir External\build\lodepng
pushd External\build\lodepng
cmake -S ..\..\src\lodepng -B .
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
popd