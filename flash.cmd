@echo off
wsl --cd %~dp0 PICOTOOL_WINDOWS=/mnt/c/security/pico/picotool/picotool.exe ./flash.sh
timeout /t 15
