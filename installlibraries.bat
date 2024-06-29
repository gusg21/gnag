@echo off

pacman -S 3ds-libogg 3ds-libopus 3ds-opusfile --noconfirm

mkdir "%APPDATA%\Citra\sdmc\3ds" 2>nul

copy misc/dspfirm.cdc "%APPDATA%\Citra\sdmc\3ds"

echo.
echo [92mUpdated/installed libraries and copied 3DS sound firmware to Citra![0m
echo.

pause