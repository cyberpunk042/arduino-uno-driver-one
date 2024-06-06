#ifndef RF_RECEIVER_H
#define RF_RECEIVER_H

#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>

const char txt_sent[] = "OK";

class RFReceiver {
private:
    RF24 radio;
    const byte address1[6];
    const byte address2[6];

public:
    RFReceiver(int ce, int csn) : radio(ce, csn, 1000000), address1("00001"), address2("00002") {} // CE, CSN CSN

    void setup() {
      radio.begin();  
      radio.setRetries(15,15);
      radio.setPayloadSize(32);
      radio.setDataRate(RF24_1MBPS);
      radio.setPALevel(RF24_PA_MAX);
      //radio.setChannel(0x4c);
      //radio.setAutoAck(1);
      //radio.enableDynamicPayloads();
      //radio.openWritingPipe(address2);    // "2Node"
      radio.openReadingPipe(1, address1); // "1Node"
    }

    String receive() {
      radio.startListening();
      char txt_received[32];
      if (radio.available()) {  
        radio.read(&txt_received, sizeof(txt_received)); 
        //txt_received[sizeof(txt_received) - 1] = '\0';
        //Serial.print("Received: "); Serial.println(txt_received);
        return String(txt_received);
      }
      
      //delay(10);
      
      //radio.stopListening();
      //radio.write(&txt_sent, sizeof(txt_sent));  
      //Serial.print("Sent: "); Serial.println(txt_sent);

      return String("");
    }
};

#endif
