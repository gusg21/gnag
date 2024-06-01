@echo off

if exist C:\devkitpro\ (
  echo Found devKitPro... 
) else (
  echo [41m# ERROR: devKitPro not found![0m
  goto :ERROR
)

where make >nul 2>nul
if %ERRORLEVEL% equ 0 (
    echo Found make...
) else (
    echo [41m# ERROR: make not found![0m
    goto :ERROR
)

echo.

cd %GNAG_PATH%
make clean

REM No error checking because the .3dsx file is likely open in Citra, which results in an error
REM even though the actual clean process worked just fine.
:OK
echo [92m# Cleaned the build.[0m
goto :END

:END
if "%~1" neq "/nopause" (
pause
)
    