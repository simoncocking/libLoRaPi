#include <cstdio>
#include "lora.h"

#define SPI_CHANNEL 0
#define SS_PIN      6
#define DIO0_PIN    7
#define RST_PIN     0

static const char *message = "{'hello': ['world']}";
int main() {
	printf("Setting up LoRa\n");
	LoRa lora(SPI_CHANNEL, SS_PIN, DIO0_PIN, RST_PIN);
	if (lora.begin()) {
		printf("LoRa setup successful: chipset version 0x%02x\n", lora.version());
		LoRaPacket p((unsigned char *)message, strlen(message));
		printf("Constructed packet: payload='%s', length=%d\n", p.getPayload(), p.payloadLength());
		printf("Configuring radio\n");
		lora.setFrequency(LoRa::FREQ_433)
			->setTXPower(17)
			->setSpreadFactor(LoRa::SF_12)
			->setBandwidth(LoRa::BW_125k)
			->setCodingRate(LoRa::CR_48)
			->setSyncWord(0x12)
			->setHeaderMode(LoRa::HM_EXPLICIT)
			->enableCRC();
		printf("  TX power     : %d dB\n", lora.getTXPower());
		printf("  Frequency    : %d Hz\n", lora.getFrequency());
		printf("  Spread factor: %d\n", lora.getSpreadFactor());
		printf("  Bandwidth    : %d Hz\n", lora.bw[lora.getBandwidth()]);
		printf("  Coding Rate  : 4/%d\n", lora.getCodingRate() + 4);
		printf("  Sync word    : 0x%02x\n", lora.getSyncWord());
		printf("  Header mode  : %s\n", lora.getHeaderMode() == LoRa::HM_IMPLICIT ? "Implicit" : "Explicit");
		printf("Transmitting packet\n");
		size_t bytes = lora.transmitPacket(&p);
		printf("  %d bytes transmitted\n", bytes);
	}
}