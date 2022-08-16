#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <memory>
#include <unistd.h>
#include <wiringPi.h>
#include <wiringPiSPI.h>

#include "lora.h"
#include "packet.h"

const uint32_t LoRa::bw[10] = {
	7800,  10400,  15600,
	20800, 31250,  41700,
	62500, 125000, 250000,
	500000
};

LoRa::LoRa(uint8_t spi, uint8_t ss, uint8_t dio0, uint8_t rst) : _spi_channel(spi),
																 _ss_pin(ss),
																 _dio0_pin(dio0),
																 _rst_pin(rst) {}

LoRa *LoRa::setSpreadFactor(LoRa::sf_t sf)
{
	sleep();
	sf = sf < SF_6
			 ? SF_6
			 : sf > SF_12
				   ? SF_12
				   : sf;
	if (sf == SF_6)
	{
		setHeaderMode(HM_IMPLICIT);
		writeRegister(REG_DETECT_OPTIMIZE, 0xC5);
		writeRegister(REG_DETECT_THRESH, 0x0C);
	}
	else
	{
		writeRegister(REG_DETECT_OPTIMIZE, 0xC3);
		writeRegister(REG_DETECT_THRESH, 0x0A);
	}
	writeRegister(REG_MODEM_CONFIG2, (readRegister(REG_MODEM_CONFIG2) & 0x0F) | (sf << 4));
	return this;
}

LoRa::sf_t LoRa::getSpreadFactor()
{
	return (LoRa::sf_t)(readRegister(REG_MODEM_CONFIG2) >> 4);
}

LoRa *LoRa::setFrequency(uint32_t freq)
{
	uint64_t frf = ((uint64_t)freq << 19) / OSC_FREQ;
	writeRegister(REG_FRF_MSB, (uint8_t)(frf >> 16));
	writeRegister(REG_FRF_MID, (uint8_t)(frf >> 8));
	writeRegister(REG_FRF_LSB, (uint8_t)(frf >> 0));
	if (freq < FREQ_868)
	{
		// set LowFrequencyMode
		writeRegister(REG_OPMODE, readRegister(REG_OPMODE & 0xC7) | 0x08);
	}
	return this;
}

uint32_t LoRa::getFrequency()
{
	uint64_t frf = (readRegister(REG_FRF_MSB) << 16 |
					readRegister(REG_FRF_MID) << 8 |
					readRegister(REG_FRF_LSB) << 0);
	LoRa::freq_t freq = (LoRa::freq_t)((uint64_t)(OSC_FREQ * frf) >> 19);
	return freq;
}

LoRa *LoRa::setBandwidth(bw_t bandwidth)
{
	writeRegister(REG_MODEM_CONFIG1, (readRegister(REG_MODEM_CONFIG1) & 0x0F) | (bandwidth << 4));
	long symDuration = 1000 / (bw[getBandwidth()] / (1L << getSpreadFactor()));
	// Set LowDataRateOptimize if symbol duration > 16ms
	writeRegister(REG_MODEM_CONFIG3, (readRegister(REG_MODEM_CONFIG3) & 0x07) | (symDuration > 16 ? 0x08 : 0x00));
	return this;
}

LoRa::bw_t LoRa::getBandwidth()
{
	return (LoRa::bw_t)(readRegister(REG_MODEM_CONFIG1) >> 4);
}

LoRa *LoRa::setTXPower(uint8_t p)
{
	p = p < 2
			? 2
			: p > 17
				  ? 17
				  : p;
	writeRegister(REG_PA_CONFIG, (uint8_t)(PA_BOOST | (p - 2)));
	return this;
}

uint8_t LoRa::getTXPower()
{
	uint8_t pa_config = readRegister(REG_PA_CONFIG);
	double pmax = pa_config & PA_BOOST
					  ? 17.0
					  : 10.8 + 0.6 * (pa_config & 0x38);
	return pmax - (15 - (pa_config & 0x0F));
}

LoRa *LoRa::setHeaderMode(hm_t hm)
{
	writeRegister(REG_MODEM_CONFIG1, (readRegister(REG_MODEM_CONFIG1) & 0xFE) | hm);
	return this;
}

LoRa::hm_t LoRa::getHeaderMode()
{
	return (LoRa::hm_t)(readRegister(REG_MODEM_CONFIG1) & 0x01);
}

LoRa *LoRa::setCodingRate(cr_t cr)
{
	writeRegister(REG_MODEM_CONFIG1, (readRegister(REG_MODEM_CONFIG1) & 0xF1) | (cr << 1));
	return this;
}

LoRa::cr_t LoRa::getCodingRate()
{
	return (LoRa::cr_t)((readRegister(REG_MODEM_CONFIG1) & 0x0E) >> 1);
}

LoRa *LoRa::setSyncWord(uint8_t sw)
{
	writeRegister(REG_SYNC_WORD, sw);
	return this;
}

uint8_t LoRa::getSyncWord()
{
	return readRegister(REG_SYNC_WORD);
}

LoRa *LoRa::enableCRC()
{
	writeRegister(REG_MODEM_CONFIG2, readRegister(REG_MODEM_CONFIG2) | 0x04);
	return this;
}

LoRa *LoRa::disableCRC()
{
	writeRegister(REG_MODEM_CONFIG2, readRegister(REG_MODEM_CONFIG2) & 0xFB);
	return this;
}

LoRa *LoRa::setLNAGain(lna_gain_t gain)
{
	if (gain == LNA_AGC)
	{
		writeRegister(REG_MODEM_CONFIG3, readRegister(REG_MODEM_CONFIG3) | 0x04);
	}
	else
	{
		writeRegister(REG_MODEM_CONFIG3, readRegister(REG_MODEM_CONFIG3) & 0xFB);
		writeRegister(REG_LNA, (readRegister(REG_LNA) & 0x1F) | gain);
	}
	return this;
}

LoRa::lna_gain_t LoRa::getLNAGain()
{
	if (readRegister(REG_MODEM_CONFIG3) & 0x04)
	{
		return LNA_AGC;
	}
	return (LoRa::lna_gain_t)(readRegister(REG_LNA) >> 5);
}

LoRa *LoRa::setLNABoost(bool boost)
{
	writeRegister(REG_LNA, (readRegister(REG_LNA) & 0xFA) | (boost ? 0x03 : 0x00));
	return this;
}

bool LoRa::getLNABoost()
{
	return (readRegister(REG_LNA) & 0x03) == 0x03;
}

int LoRa::getFrequencyError()
{
	uint32_t fei =
		(readRegister(REG_FEI_MSB) & 0x0F) << 16 |
		readRegister(REG_FEI_MID) << 8 |
		readRegister(REG_FEI_LSB);
	return (uint32_t)((double)((uint64_t)(fei << 24) / OSC_FREQ) * (double)(bw[getBandwidth()] / 500.0));
}

void LoRa::selectReceiver() { digitalWrite(_ss_pin, LOW); }
void LoRa::deselectReceiver() { digitalWrite(_ss_pin, HIGH); }

uint8_t LoRa::readRegister(uint8_t addr)
{
	_spibuf[0] = addr & 0x7f;
	_spibuf[1] = 0x00;
	selectReceiver();
	wiringPiSPIDataRW(_spi_channel, _spibuf, 2);
	deselectReceiver();
	return _spibuf[1];
}

void LoRa::writeRegister(uint8_t addr, uint8_t val)
{
	_spibuf[0] = addr | 0x80;
	_spibuf[1] = val;
	selectReceiver();
	wiringPiSPIDataRW(_spi_channel, _spibuf, 2);
	deselectReceiver();
}

void LoRa::setOpMode(uint8_t mode)
{
	// Preserve AccessSharedReg and LowFrequencyModeOn
	writeRegister(REG_OPMODE, (readRegister(REG_OPMODE) & 0xC8) | OPMODE_LORA | mode);
}

uint8_t LoRa::getOpMode()
{
	return readRegister(REG_OPMODE) & OPMODE_MASK;
}

void LoRa::sleep()
{
	if (getOpMode() != OPMODE_SLEEP)
	{
		setOpMode(OPMODE_SLEEP);
	}
}

void LoRa::standby()
{
	if (getOpMode() != OPMODE_STANDBY)
	{
		setOpMode(OPMODE_STANDBY);
	}
}

uint8_t LoRa::version()
{
	digitalWrite(_rst_pin, LOW);
	delay(100);
	digitalWrite(_rst_pin, HIGH);
	delay(100);
	uint8_t version = readRegister(REG_VERSION);
	return version;
}

bool LoRa::begin()
{
	wiringPiSetup();
	pinMode(_ss_pin, OUTPUT);
	pinMode(_dio0_pin, INPUT);
	pinMode(_rst_pin, OUTPUT);
	wiringPiSPISetup(_spi_channel, 500000);
	sleep();
	if (version() != 0x12)
	{
		// Unknown chipset
		return false;
	}
	writeRegister(REG_FIFO_TX_BASE_ADDR, 0);
	writeRegister(REG_FIFO_RX_BASE_ADDR, 0);
	setLNABoost(true);
	setLNAGain(LNA_AGC);
	writeRegister(REG_PA_RAMP, (readRegister(REG_PA_RAMP) & 0xF0) | 0x08);
	return true;
}

size_t LoRa::receive(unsigned char *buf)
{
	size_t bytes = 0;
	uint8_t irq_flags = readRegister(REG_IRQ_FLAGS);
	writeRegister(REG_IRQ_FLAGS, irq_flags); // clear IRQs
	if ((irq_flags & IRQ_RXDONE_MASK) && ((irq_flags & IRQ_CRCERR_MASK) == 0)) {
		// Received a packet
		bytes = readRegister(REG_RX_NB_BYTES);
		writeRegister(REG_FIFO_ADDR_PTR, readRegister(REG_FIFO_RX_CURRENT_ADDR));
		setOpMode(OPMODE_STANDBY);
		for (size_t i = 0; i < bytes; i++) {
			*(buf + i) = readRegister(REG_FIFO);
		}
	}
	return bytes;
}

LoRaPacket LoRa::receivePacket()
{
	unsigned char buf[256];
	setHeaderMode(HM_EXPLICIT);
	writeRegister(REG_FIFO_ADDR_PTR, 0);
	setOpMode(OPMODE_RX);
	size_t bytes = 0;
	while ((bytes = receive(buf)) == 0) {
		usleep(10000);
	}
	int packet_rssi = readRegister(REG_PACKET_RSSI) - (getFrequency() < FREQ_868 ? 164 : 157);
	float snr = ((int8_t)readRegister(REG_PACKET_SNR)) * 0.25;
	return LoRaPacket(buf, bytes, packet_rssi, getFrequencyError(), snr);
}

size_t LoRa::write(const uint8_t *data, size_t size)
{
	uint8_t currentLength = readRegister(REG_PAYLOAD_LENGTH);
	if (currentLength + size > MAX_PACKET_LEN)
	{
		size = MAX_PACKET_LEN - currentLength;
	}
	unsigned char *buf;
	if (size > 0 && (buf = (unsigned char *)calloc(1, size + 2)))
	{
		buf[0] = REG_FIFO | 0x80;
		for (size_t i = 0; i < size; i++)
		{
			buf[i + 1] = data[i];
		}
		selectReceiver();
		int bytes = wiringPiSPIDataRW(_spi_channel, buf, size + 1);
		deselectReceiver();
		writeRegister(REG_PAYLOAD_LENGTH, currentLength + size);
		free(buf);
		return bytes > 1 ? bytes - 1 : bytes;
	}
	return 0;
}

size_t LoRa::transmitPacket(LoRaPacket *packet)
{
	standby();
	writeRegister(REG_FIFO_ADDR_PTR, 0x80);
	writeRegister(REG_PAYLOAD_LENGTH, 0);
	size_t size = packet->payloadLength();
	unsigned char *payload = packet->getPayload();
	while (size > 0)
	{
		size_t bytes = write(payload, size);
		if (bytes == 0)
			break;
		payload += bytes;
		size -= bytes;
	}
	setOpMode(OPMODE_TX);
	while ((readRegister(REG_IRQ_FLAGS) & IRQ_TXDONE_MASK) == 0)
	{
		usleep(1000);
	}
	writeRegister(REG_IRQ_FLAGS, IRQ_TXDONE_MASK);
	sleep();
	return packet->payloadLength() - size;
}
