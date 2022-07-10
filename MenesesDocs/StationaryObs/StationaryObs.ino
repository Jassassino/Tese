#include <LoRa.h>
#include <SPI.h>

#define WAITTIME 30  //Seconds

const char TYPE = 'o';
const unsigned short ID = 1U;
const double LAT = 38.99054994878942;
const double LNG = -9.078020912749718;
const float SIZ = 1.0;

unsigned char msg_buf[14];

void setup() {
    while (!LoRa.begin(8681E5)) {
        delay(10000);
    }
    LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
    LoRa.setSyncWord(0x73);
    LoRa.setTxPower(2);  //14Anacom 17max
                         //LoRa.enableCrc();
    memcpy(msg_buf, &TYPE, 1);
    memcpy(msg_buf + 1, &ID, 1);
    memcpy(msg_buf + 2, &LAT, 4);
    memcpy(msg_buf + 6, &LNG, 4);
    memcpy(msg_buf + 10, &SIZ, 4);
}

void loop() {
    // send packet
    LoRa.beginPacket();
    LoRa.write(msg_buf, 14);
    LoRa.endPacket();
    LoRa.sleep();

    delay(1000 * WAITTIME);
}
