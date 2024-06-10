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

cd "%GNAG_PATH%"
make

if %ERRORLEVEL% equ 0 (
    goto :OK
) else (
    goto :ERROR
)

:ERROR
echo [41m# Failed to build![0m
goto :END

:OK
echo [92m# Built GNAG.3dsx[0m
goto :END

:END
if "%~1" neq "/nopause" (
pause
)
    