@echo off
set currentDir=%~dp0
set buildDir=%currentDir%
set cmakeOutputDir=%currentDir%\build
cmake -S %buildDir% -B %cmakeOutputDir% -G"Visual Studio 16 2019" -T v140 -A x64 -D KAIZEN_GLOBAL_VAR="abcdef"
pause