@echo off

echo Enter a Memory Address to translate it to a C source line:
echo (CTRL+C to close, or close this window)

"C:\devkitPro\devkitARM\bin\arm-none-eabi-addr2line.exe" -e "%GNAG_PATH%\gnag.elf"