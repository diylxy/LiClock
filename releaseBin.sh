esptool.py --chip esp32 elf2image \
  --flash_mode qio \
  --flash_freq 80m \
  --dont-append-digest \
  -o .pio/build/esp32solo1/firmware.bin \
  .pio/build/esp32solo1/firmware.elf

esptool.py --chip esp32 elf2image \
  --flash_mode qio \
  --flash_freq 80m \
  --dont-append-digest \
  -o .pio/build/esp32solo1/bootloader.bin \
  ~/.platformio/packages/framework-arduino-solo1/tools/sdk/esp32/bin/bootloader_qio_80m.elf

esptool.py --chip esp32 merge_bin \
  -o LiClockWebFlash/webflash/bin/4M.bin \
  --flash_mode qio \
  --flash_freq 80m \
  --flash_size 4MB \
  0x1000 .pio/build/esp32solo1/bootloader.bin \
  0x8000 .pio/build/esp32solo1/partitions.bin \
  0xe000 bin/boot_app0.bin \
  0x10000 .pio/build/esp32solo1/firmware.bin

esptool.py --chip esp32 merge_bin \
  -o LiClockWebFlash/webflash/bin/8M.bin \
  --flash_mode qio \
  --flash_freq 80m \
  --flash_size 8MB \
  0x1000 .pio/build/esp32solo1/bootloader.bin \
  0x8000 .pio/build/esp32solo1/partitions.bin \
  0xe000 bin/boot_app0.bin \
  0x10000 .pio/build/esp32solo1/firmware.bin

esptool.py --chip esp32 merge_bin \
  -o LiClockWebFlash/webflash/bin/16M.bin \
  --flash_mode qio \
  --flash_freq 80m \
  --flash_size 16MB \
  0x1000 .pio/build/esp32solo1/bootloader.bin \
  0x8000 .pio/build/esp32solo1/partitions.bin \
  0xe000 bin/boot_app0.bin \
  0x10000 .pio/build/esp32solo1/firmware.bin
