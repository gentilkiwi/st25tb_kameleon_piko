PICOTOOL_BINARY=${PICOTOOL_WINDOWS:-picotool}

$PICOTOOL_BINARY reboot --force --usb
sleep 1
$PICOTOOL_BINARY load --verify build/*.elf
$PICOTOOL_BINARY info --all
$PICOTOOL_BINARY reboot --force
