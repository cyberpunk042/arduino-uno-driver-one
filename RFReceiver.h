#ifndef RF_RECEIVER_H
#define RF_RECEIVER_H

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

class RFReceiver {
private:
    RF24 radio;
    const byte address1[6];
    const byte address2[6];

public:
    RFReceiver(int ce, int csn)
        : radio(ce, csn, 1000000), address1("00001"), address2("00002") {}

    void setup() {
        radio.begin();
        radio.setRetries(15, 15);
        radio.setPayloadSize(32);
        radio.setDataRate(RF24_1MBPS);
        radio.setPALevel(RF24_PA_MAX);
        radio.openReadingPipe(1, address1);
        radio.startListening();  // Start once, not in every receive call
    }

    // Check if data is available
    bool available() {
        return radio.available();
    }

    // Non-blocking receive
    String receive() {
        if (!radio.available()) return "";

        char txt_received[32] = {0};  // Ensure buffer is zeroed
        radio.read(&txt_received, sizeof(txt_received));
        return String(txt_received);
    }
};

#endif
