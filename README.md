# libLoRaPi (A work in progress)

A C++ library to interface with the [Dragino](dragino) [LoRa](lora) Hat for [Raspberry Pi](raspi). This board is based on the Semtech SX127[6789] chipset, and supports all global ISM bands. It also has an on-board GPS chipset based on the [Mediatek MT3339](mtk) however this driver does not support its use. Communication is via SPI, using the [wiringPi](wiringPi) SPI library.

This chipset operates in both LoRa and FSK/OOK modes, however this driver supports only LoRa operation.

[dragino]: https://www.dragino.com/products/lora/item/106-lora-gps-hat.html
[raspi]: https://raspberrypi.org
[lora]: https://lora-alliance.org
[wiringpi]: http://wiringpi.com/reference/spi-library/
[mtk]: https://labs.mediatek.com/en/chipset/MT3339

## Usage

Note that all `set()` methods return `this`, so they can be chained.

```c++
    /**
     * Set up a LoRa instance
     *
     * Params:
     * SPI_CHANNEL: The SPI channel to use. RasPi has two; 0 and 1
     * SS_PIN: The SPI slave-select pin. The Dragino board uses pin 6.
     * DIO0_PIN: The (interrupt-capable) DIO0 pin. Dragino uses pin 7.
     * RST_PIN: The LoRa RESET pin. Dragino uses pin 0.
     */
    LoRa lora(SPI_CHANNEL, SS_PIN, DIO0_PIN, RST_PIN);

    /**
     * bool begin()
     * Initialise the LoRa radio.
     * Params: none
     * Returns: true if the radio chipset is recognised; false otherwise
     */
    lora.begin();

    /**
     * LoRa setFrequency(freq) - set the carrier centre frequency, in Hz
     * The LoRa class defines three constants for this purpose:
     * - FREQ_433: 433 MHz
     * - FREQ_868: 868.1 MHz
     * - FREQ_915: 915 MHz
     */
    lora.setFrequency(LoRa::FREQ_433);

    /**
     * LoRa setTXPower(power)
     * Set the power amplifier transmit power, in dBm.
     * Minimum: 2 dBm
     * Maximum: 17 dBm normally; 20 dBm in boost mode (see setLNABoost())
     */
    lora.setTXPower(17);

    /**
     * LoRa setSpreadFactor(sf)
     * Set the radio spreading factor. High values maximise range and
     * reliability; low values maximise bandwidth.
     * The default is 7; acceptable values range from 6 to 12.
     * The LoRa class defines constants SF_6 through SF_12.
     */
    lora.setSpreadFactor(LoRa::SF_12);

    /**
     * LoRa setBandwidth(bw)
     * Set the signal bandwidth in Hz
     * The default is 125 kHz (BW_125k)
     * Acceptable discreet values range from 7.8 kHz to 500 kHz (depending on
     * the carrier frequency). The LoRa class defines the following acceptable
     * values:
     * BW_7k8,  BW_10k4, BW_15k6, BW_20k8, BW_31k25,
     * BW_41k7, BW_62k5, BW_125k, BW_250k, BW_500k
     */
    lora.setBandwidth(LoRa::BW_125k);

    /**
     * LoRa setCodingRate(cr)
     * Sets the radio's cyclic error coding rate. High values maximise
     * reliability and error detection/correction, but incur a higher data
     * overhead per transmission. Smaller values decrease the error
     * correction in favour of data throughput. The LoRa class defines the
     * following constants for this use:
     * CR_45 (default), CR_46, CR_47, CR_48
     */
    lora.setCodingRate(LoRa::CR_48);

    /**
     * LoRa setSyncWord(uint8_t)
     * Sets the sync word (essentially an address) for the radio.
     * Packets received with a matching sync word will be processed.
     * A value of 0x34 is reserved for LoRaWAN networks. The default is 0x12.
     */
    lora.setSyncWord(0x12);

    /**
     * LoRa setHeaderMode(hm)
     * LoRa has two header modes - explicit (the default) and implicit.
     * In explicit mode, a header is added to the packet containing the
     * payload length, error coding rate, and CRC, which supports a variable
     * packet length. If a fixed packet length is known (or spread factor 6
     * is selected where implicit mode is mandatory) then the header can be
     * omitted using implicit mode.
     */
    lora.setHeaderMode(LoRa::HM_EXPLICIT);

    /**
     * LoRa enableCRC()
     */
    lora.enableCRC();

```

## Examples

See the sample applications in the [examples](examples) folder.

## To Do

- Code to receive transmissions in single-packet mode
- More examples
- Finish the documentation
- Tests!

## Credits

Inspiration (and implementation hints) came from the [Arduino LoRa library](arduiono-lora) by [Sandeep Mistry](sandeep-mistry). Without a working reference implementation for testing, development of this library would scarcely have been possible.

[arduino-lora]: https://github.com/sandeepmistry/arduino-LoRa
[sandeep-mistry]: https://github.com/sandeepmistry

## References

### Semtech SX1278

- [Product data](https://www.semtech.com/products/wireless-rf/lora-transceivers/SX1278)
- [Datasheet](https://www.semtech.com/uploads/documents/DS_SX1276-7-8-9_W_APP_V6.pdf)

## License

This software is licensed under the [MIT License](LICENSE.md).
