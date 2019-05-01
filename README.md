libLoRaPi
=========

A C++ library to interface with the [Dragino](dragino) [LoRa](lora) Hat for [Raspberry Pi](raspi). This board is based on the Semtech SX127[6789] chipset, and supports all global ISM bands. It also has an on-board GPS chipset based on the [Mediatek MT3339](mtk) however this driver does not support its use. Communication is via SPI, using the [wiringPi](wiringPi) SPI library.

This chipset operates in both LoRa and FSK/OOK modes, however this driver supports only LoRa operation.

[dragino]: https://www.dragino.com/products/lora/item/106-lora-gps-hat.html
[raspi]: https://raspberrypi.org
[lora]: https://lora-alliance.org
[wiringPi]: http://wiringpi.com/reference/spi-library/
[mtk]: https://labs.mediatek.com/en/chipset/MT3339

## Usage

## Examples

See the sample applications in the [examples](examples) folder.

## Credits

Inspiration (and implementation hints) came from the [Arduino LoRa library](arduiono-lora) by [Sandeep Mistry](sandeep-mistry). Without a working reference implementation for testing, development of this library would scarcely have been possible.

[arduino-lora]: https://github.com/sandeepmistry/arduino-LoRa
[sandeep-mistry]: https://github.com/sandeepmistry

## References

[Semtech SX1278 product data]: https://www.semtech.com/products/wireless-rf/lora-transceivers/SX1278
[Semtech SX1278 datasheet]: https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V6.pdf

## License

This software is licensed under the [MIT License](LICENSE.md).