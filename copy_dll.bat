@echo off
set VCPKG_BIN_DIR="C:\vcpkg\installed\x64-windows\debug\bin"
set BUILD_DIR="C:\wamp64\www\Projet Piscine\Environnement-3D-en-cpp-avec-Allegro\build\Debug"

echo Copying DLLs from %VCPKG_BIN_DIR% to %BUILD_DIR%
copy %VCPKG_BIN_DIR%\*.dll %BUILD_DIR%
echo DLLs copied successfully.