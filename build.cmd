@echo off
wsl --cd %~dp0 PICO_SDK_PATH=/mnt/c/security/pico/pico-sdk ./build.sh
timeout /t 15
