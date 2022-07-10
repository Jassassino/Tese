
#include <LoRa.h>
#include <SPI.h>
#include <TinyGPS.h>

#define WAITTIME 1000UL  //Miliseconds

TinyGPS gps;

unsigned long last_send = 0;

const char TYPE = 'O';
const unsigned short ID = 2U;
float LAT;
float LNG;
float CRS;
float SPD;
const float SIZ = 1.0;
char msg_buf[26]={'C','A','R','A','G','O'};
size_t u;

void setup() {
    // initialize both serial ports:
    Serial.begin(9600);  // SoftSerial port to get GPS data.
    LoRa.setSpreadingFactor(8);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(5);
    LoRa.setSyncWord(0x73);
    LoRa.setTxPower(7);
    //LoRa.enableCrc();
    while (!LoRa.begin(8681E5)) {
        delay(10000);
    }
    /*LoRa.setSpreadingFactor(7);
    LoRa.setSignalBandwidth(125E3);
    LoRa.setCodingRate4(6);
    LoRa.setSyncWord(0x73);
    LoRa.setTxPower(7);  //14Anacom 17max
    LoRa.enableCrc();*/
}

void loop() {
    while (Serial.available()) {
        gps.encode(Serial.read());
        Serial.println("Stef's cute");
    }
    if (millis() - last_send > WAITTIME) {
        unsigned long age;
        Serial.println("Stef's cute");
        gps.f_get_position(&LAT, &LNG, &age);
        CRS = gps.f_course();
        SPD = gps.f_speed_knots();
        /*Serial.print(LAT);
        Serial.println(" Latitude");
        Serial.print(LNG);
        Serial.println(" Longitude");
        Serial.print(age);
        Serial.println(" Age");
        Serial.println(CRS);
        Serial.println(SPD);*/
        delay(5000);


        if (LAT == TinyGPS::GPS_INVALID_F_ANGLE || LNG == TinyGPS::GPS_INVALID_F_ANGLE || CRS == TinyGPS::GPS_INVALID_F_ANGLE || SPD == TinyGPS::GPS_INVALID_F_SPEED) {
            //dont send
            Serial.println(msg_buf);
            delay(100);
            LoRa.beginPacket();
            u = LoRa.write(msg_buf, 7);
            LoRa.endPacket();
            //LoRa.onTxDone((void*)Serial.println("Enviado com sucesso"));
            LoRa.sleep();
            Serial.print("Foram enviados caracteres: ");
            Serial.println(u);
        } else {
            Serial.println("ILHA MAGICA DO WORK");
            memcpy(msg_buf, &TYPE, 1);
            memcpy(msg_buf + 1, &ID, 1);
            memcpy(msg_buf + 2, &LAT, 4);   
            memcpy(msg_buf + 6, &LNG, 4);
            memcpy(msg_buf + 10, &SIZ, 4);
            memcpy(msg_buf + 14, &CRS, 4);
            memcpy(msg_buf + 18, &SPD, 4);
            LoRa.beginPacket();
            LoRa.write(msg_buf, 22);
            LoRa.endPacket();
            LoRa.sleep();
        }
        last_send = millis();
    }
  Serial.println("Round");
  delay(6000);

}
