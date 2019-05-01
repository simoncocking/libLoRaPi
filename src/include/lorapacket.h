#include <string.h>
#include <stdint.h>

class LoRaPacket {
	private:
		unsigned char *_payload;
		size_t _payload_len;
		int _packet_rssi;
		int _rssi;
		int _snr;

	public:
		LoRaPacket(unsigned char *, size_t);
		LoRaPacket(unsigned char *, size_t, int, int, int);
		unsigned char *getPayload();
		size_t payloadLength();
		int getPacketRSSI();
		int getRSSI();
		int getSNR();
		void setPayload(unsigned char *, size_t size);
};
