@echo off
wsl --cd %~dp0 PICOTOOL_WINDOWS=/mnt/c/security/pico/picotool_windows/picotool.exe ./flash.sh
timeout /t 15
