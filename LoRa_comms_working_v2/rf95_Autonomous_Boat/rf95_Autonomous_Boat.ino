// rf95_client.pde

#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 lora;

uint8_t MAX_MESSAGE_LEN = 82;

void setup() 
{

  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!lora.init())
    Serial.println("init failed");
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // You can change the modulation parameters with eg
  // lora.setModemConfig(RH_RF95::Bw500Cr45Sf128);
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 2 to 20 dBm:
  //  lora.setTxPower(20, false);
  // Failure to do that will result in extremely low transmit powers.
}

void loop()
{
  
  SendMessageLoRa();
  RecvMessageLoRa();
  delay(500);
  /*Serial.println("Sending current location");
  // Send a message to GroundStation
  uint8_t data[] = "$GPGLL,4916.45,N,12311.12,W,225444,A";
  lora.send(data, sizeof(data));
  lora.waitPacketSent();
  
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
  uint8_t len = sizeof(buf);

  if (lora.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (lora.recv(buf, &len))
   {
      Serial.print("got reply: ");
      Serial.println((char*)buf);
//      Serial.print("RSSI: ");
//      Serial.println(lora.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  else
  {
    Serial.println("No reply, is rf95_server running?");
  }*/
}


void SendMessageLoRa(){
  //Serial.println("Sending current location");
  // Send a message to GroundStation
  uint8_t data[] = "Current ASV position \n$GPGLL,4916.45,N,12311.12,W,225444,A";
  //Serial.println((char*)data);
  lora.send(data, sizeof(data));
  lora.waitPacketSent();
  
}
void RecvMessageLoRa(){
  // Now wait for a reply
  uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]={NULL};
  uint8_t len = sizeof(buf);

  if (lora.waitAvailableTimeout(3000))
  { 
    // Should be a reply message for us now   
    if (lora.recv(buf, &len))
   {
      Serial.println("Connected!");
      if(len){
        Serial.print("Message:");
        Serial.println((char*)buf);
      }
//      Serial.println(len);
//      Serial.print("RSSI: ");
//      Serial.println(lora.lastRssi(), DEC);    
    }
    else
    {
      Serial.println("Failed to receive message");
    }
  }
  else
  {
    Serial.println("No reply, is GroundStation running?");
  }
}
