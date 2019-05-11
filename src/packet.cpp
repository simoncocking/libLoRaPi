#include <string.h>
#include <stdint.h>
#include <stdio.h>

#include "packet.h"

LoRaPacket::LoRaPacket(unsigned char *payload, size_t size) :
	_payload(payload),
	_payload_len(size),
	_packet_rssi(0),
	_rssi(0),
	_snr(0) {}

LoRaPacket::LoRaPacket(unsigned char *payload, size_t size, int packet_rssi, int rssi, int snr) :
	_payload(payload),
	_payload_len(size),
	_packet_rssi(packet_rssi),
	_rssi(rssi),
	_snr(snr) {}

unsigned char *LoRaPacket::getPayload() { return _payload; }
int LoRaPacket::getPacketRSSI()         { return _packet_rssi; }
int LoRaPacket::getRSSI()               { return _rssi; }
int LoRaPacket::getSNR()                { return _snr; }
size_t LoRaPacket::payloadLength()      { return _payload_len; }
