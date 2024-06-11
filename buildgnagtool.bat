@echo off

if exist "C:\Program Files\CMake\bin\cmake.exe" (
"C:\Program Files\CMake\bin\cmake.exe" -DCMAKE_BUILD_TYPE=Debug -S "%GNAG_PATH%\tools\gnagtool" -B "%GNAG_PATH%\tools\gnagtool\build"
pause
) else (
echo "CMake not installed!"
pause
)