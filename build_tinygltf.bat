@echo off
mkdir External\build\tinygltf
pushd External\build\tinygltf
cmake -S ..\..\src\tinygltf -B .
cmake --build . --config Release --target install
cmake --build . --config Debug --target install
popd