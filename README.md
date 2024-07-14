# st25tb_kameleon_piko
ST25TB / SRx NFC Emulator / Initiator - kameleon - based on TI TRF7970A with Raspberry Pi PICO (RP2040)

## Firmware

```
SKIP_OPENOCD=1 SKIP_VSCODE=1 SKIP_UART=1 ./pico_setup.sh
```


```
..\openocd -f interface\cmsis-dap.cfg -f target\rp2040.cfg -c "adapter speed 5000" -c "program c:\\security\\pico\\st25tb_kameleon_piko\\build\\st25tb_kameleon_piko.elf verify reset exit"
```

## References

### Raspberry Pi

#### RP2040
- https://datasheets.raspberrypi.com/rp2040/rp2040-product-brief.pdf
- https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf
- https://datasheets.raspberrypi.com/rp2040/hardware-design-with-rp2040.pdf

#### Pico
- https://datasheets.raspberrypi.com/pico/pico-product-brief.pdf
- https://datasheets.raspberrypi.com/pico/pico-datasheet.pdf
- https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf
- https://datasheets.raspberrypi.com/pico/Pico-R3-A4-Pinout.pdf
- https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf
- https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-python-sdk.pdf
