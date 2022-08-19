#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <memory>

#include "packet.h"

LoRaPacket::LoRaPacket(unsigned char *payload, size_t size) :
	_payload(nullptr),
	_payload_len(size),
	_packet_rssi(0),
	_freq_err(0),
	_snr(0) {
		_setPayload(payload, size);
	}

LoRaPacket::LoRaPacket(unsigned char *payload, size_t size, int packet_rssi, int freq_err, float snr) :
	_payload(nullptr),
	_payload_len(size),
	_packet_rssi(packet_rssi),
	_freq_err(freq_err),
	_snr(snr) {
		_setPayload(payload, size);
	}

LoRaPacket::~LoRaPacket() {
	if (_payload != nullptr) free(_payload);
}
void LoRaPacket::_setPayload(unsigned char *payload, size_t size) {
	if (_payload != nullptr) free(_payload);
	if ((_payload = (unsigned char *)calloc(size, 1)) != nullptr) {
		strncpy((char *)_payload, (char *)payload, size);
	}
}

unsigned char *LoRaPacket::getPayload() { return _payload; }
int LoRaPacket::getPacketRSSI()         { return _packet_rssi; }
int LoRaPacket::getFreqErr()            { return _freq_err; }
float LoRaPacket::getSNR()              { return _snr; }
size_t LoRaPacket::payloadLength()      { return _payload_len; }
