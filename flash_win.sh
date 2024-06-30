../windows/picotool.exe reboot --force --usb
sleep 1
../windows/picotool.exe load --verify --execute build/st25tb_kameleon_piko.elf
