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

make

if %ERRORLEVEL% equ 0 (
    goto :OK
) else (
    goto :ERROR
)

:ERROR
echo [41m# Failed to build![0m

:OK
echo [92m# Built GNAG.3dsx[0m

:END

    