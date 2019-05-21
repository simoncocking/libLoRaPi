#ifndef _LORA_H
#define _LORA_H

#include <stdint.h>
#include "packet.h"

#define OSC_FREQ                 32e6
#define MAX_PACKET_LEN           0xFF

#define REG_FIFO                 0x00
#define REG_OPMODE               0x01
// Reserved                      0x02
// Reserved                      0x03
// Reserved                      0x04
// Reserved                      0x05
#define REG_FRF_MSB              0x06
#define REG_FRF_MID              0x07
#define REG_FRF_LSB              0x08
#define REG_PA_CONFIG            0x09
#define REG_PA_RAMP              0x0A
#define REG_OCP                  0x0B
#define REG_LNA                  0x0C
#define REG_FIFO_ADDR_PTR        0x0D
#define REG_FIFO_TX_BASE_ADDR    0x0E
#define REG_FIFO_RX_BASE_ADDR    0x0F
#define REG_FIFO_RX_CURRENT_ADDR 0x10
#define REG_IRQ_FLAGS_MASK       0x11
#define REG_IRQ_FLAGS            0x12
#define REG_RX_NB_BYTES          0x13
#define REG_RX_HDR_CNT_MSB       0x14
#define REG_RX_HDR_CNT_LSB       0x15
#define REG_RX_PKT_CNT_MSB       0x16
#define REG_RX_PKT_CNT_LSB       0x17
#define REG_MODEM_STATUS         0x18
#define REG_PACKET_SNR           0x19
#define REG_PACKET_RSSI          0x1A
#define REG_RSSI                 0x1B
#define REG_HOP_CHANNEL          0x1C
#define REG_MODEM_CONFIG1        0x1D
#define REG_MODEM_CONFIG2        0x1E
#define REG_SYMB_TIMEOUT_LSB     0x1F
#define REG_PREAMBLE_MSB         0x20
#define REG_PREAMBLE_LSB         0x21
#define REG_PAYLOAD_LENGTH       0x22
#define REG_MAX_PAYLOAD_LENGTH   0x23
#define REG_HOP_PERIOD           0x24
#define REG_FIFO_RX_BYTE_ADDR    0x25
#define REG_MODEM_CONFIG3        0x26
#define REG_PPM_CORRECTION       0x27
#define REG_FEI_MSB              0x28
#define REG_FEI_MID              0x29
#define REG_FEI_LSB              0x2A
// Reserved                      0x2B
#define REG_RSSI_WIDEBAND        0x2C
// Reserved                      0x2D
// Reserved                      0x2E
#define REG_IF_FREQ1             0x2F
#define REG_IF_FREQ2             0x30
#define REG_DETECT_OPTIMIZE      0x31
// Reserved                      0x32
#define REG_INVERT_IQ            0x33
// Reserved                      0x34
// Reserved                      0x35
#define REG_HIGH_BW_OPTIM1       0x36
#define REG_DETECT_THRESH        0x37
// Reserved                      0x38
#define REG_SYNC_WORD            0x39
#define REG_HIGH_BW_OPTIM2       0x3A
#define REG_INVERT_IQ2           0x3B
// Reserved                      0x3C
// Reserved                      0x3D
// Reserved                      0x3E
// Reserved                      0x3F
#define REG_DIO_MAPPING1         0x40
#define REG_DIO_MAPPING2         0x41
#define REG_VERSION              0x42
#define REG_TCXO                 0x4B
#define REG_PA_DAC               0x4D
#define REG_FORMER_TEMP          0x5B
#define REG_AGC_REF              0x61
#define REG_AGC_THRESH1          0x62
#define REG_AGC_THRESH2          0x63
#define REG_AGC_THRESH3          0x64
#define REG_PLL                  0x70

#define PA_BOOST                 0x80

// Radio operating modes
#define OPMODE_LORA              0x80
#define OPMODE_MASK              0x07
#define OPMODE_SLEEP             0x00
#define OPMODE_STANDBY           0x01
#define OPMODE_FSTX              0x02
#define OPMODE_TX                0x03
#define OPMODE_FSRX              0x04
#define OPMODE_RX                0x05
#define OPMODE_RX_SINGLE         0x06
#define OPMODE_CAD               0x07

#define IRQ_RXTOUT_MASK          0x80
#define IRQ_RXDONE_MASK          0x40
#define IRQ_CRCERR_MASK          0x20
#define IRQ_HEADER_MASK          0x10
#define IRQ_TXDONE_MASK          0x08
#define IRQ_CDDONE_MASK          0x04
#define IRQ_FHSSCH_MASK          0x02
#define IRQ_CDDETD_MASK          0x01

// DIO function mappings                  D0D1D2D3
#define MAP_DIO0_LORA_RXDONE     0x00  // 00------
#define MAP_DIO0_LORA_TXDONE     0x40  // 01------
#define MAP_DIO1_LORA_RXTOUT     0x00  // --00----
#define MAP_DIO1_LORA_NOP        0x30  // --11----
#define MAP_DIO2_LORA_NOP        0xC0  // ----11--

class LoRa {
	private:
		unsigned char _spibuf[2];
		uint8_t  _spi_channel;
		uint8_t  _ss_pin;
		uint8_t  _dio0_pin;
		uint8_t  _rst_pin;
		void selectReceiver();
		void deselectReceiver();
		uint8_t readRegister(uint8_t);
		void writeRegister(uint8_t, uint8_t);
		void setOpMode(uint8_t);
		uint8_t getOpMode();
		size_t receive(unsigned char *);
		size_t write(const uint8_t *, size_t);

	public:
		enum freq_t {
			FREQ_433 = 433003300,
			FREQ_868 = 868100000,
			FREQ_915 = 915000000
		};
		static const uint32_t bw[10];
		enum bw_t {
			BW_7k8,  BW_10k4,  BW_15k6,
			BW_20k8, BW_31k25, BW_41k7,
			BW_62k5, BW_125k,  BW_250k,
			BW_500k
		};
		enum sf_t { SF_6 = 6, SF_7, SF_8, SF_9, SF_10, SF_11, SF_12 };
		enum hm_t { HM_EXPLICIT, HM_IMPLICIT };
		enum cr_t { CR_45 = 1, CR_46, CR_47, CR_48 };
		enum lna_gain_t {
			LNA_G1 = 1, LNA_G2, LNA_G3,
			LNA_G4,     LNA_G5, LNA_G6,
			LNA_AGC
		};

		LoRa(uint8_t, uint8_t, uint8_t, uint8_t);
		LoRa      *setSpreadFactor(sf_t);
		LoRa      *setFrequency(uint32_t);
		LoRa      *setBandwidth(bw_t);
		LoRa      *setTXPower(uint8_t);
		LoRa      *setHeaderMode(hm_t);
		LoRa      *setCodingRate(cr_t);
		LoRa      *setSyncWord(uint8_t);
		LoRa      *setLNAGain(lna_gain_t);
		LoRa      *setLNABoost(bool);
		LoRa      *enableCRC();
		LoRa      *disableCRC();
		sf_t       getSpreadFactor();
		uint32_t   getFrequency();
		bw_t       getBandwidth();
		uint8_t    getTXPower();
		hm_t       getHeaderMode();
		cr_t       getCodingRate();
		uint8_t    getSyncWord();
		lna_gain_t getLNAGain();
		bool       getLNABoost();
		int        getFrequencyError();

		bool begin();
		void sleep();
		void standby();
		uint8_t version();

		LoRaPacket receivePacket();
		size_t transmitPacket(LoRaPacket *);
};

#endif