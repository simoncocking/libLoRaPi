#ifndef _PACKET_H
#define _PACKET_H

#include <string.h>
#include <stdint.h>

class LoRaPacket {
	private:
		unsigned char *_payload;
		size_t _payload_len;
		int _packet_rssi;
		int _freq_err;
		float _snr;
		void _setPayload(unsigned char *, size_t);

	public:
		LoRaPacket(unsigned char *, size_t);
		LoRaPacket(unsigned char *, size_t, int, int, float);
		~LoRaPacket();
		unsigned char *getPayload();
		size_t payloadLength();
		int getPacketRSSI();
		int getFreqErr();
		float getSNR();
		void setPayload(unsigned char *, size_t size);
};

#endif