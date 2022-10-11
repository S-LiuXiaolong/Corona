:: build successfully but lib path is wrong
@echo off
mkdir External\build\libjpeg-turbo
pushd External\build\libjpeg-turbo
cmake -S ..\..\src\libjpeg-turbo -B .
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
:: TODO: SHIT THESE AWAY 
del ..\..\Windows\lib\turbojpeg-static.lib
del ..\..\Windows\lib\jpeg-static.lib
popd