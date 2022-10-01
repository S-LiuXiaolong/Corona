@echo off
mkdir External\build\crossguid
pushd External\build\crossguid
cmake -S ..\..\src\crossguid -B .
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
popd