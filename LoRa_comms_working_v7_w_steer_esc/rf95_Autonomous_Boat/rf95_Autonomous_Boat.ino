// rf95_client.pde

#include <SPI.h>
#include <RH_RF95.h>
#include <ServoTimer2.h>

ServoTimer2 ESC;     // create servo object to control the ESC
ServoTimer2 LEME;    //Servo object to control the steering

// Singleton instance of the radio driver
RH_RF95 lora;

//////////////////////////////////////////////////////////////////////
char readserial[RH_RF95_MAX_MESSAGE_LEN]={0};//="Idling";
uint8_t i = 0;
int val; //Steering Angle

void setup() { 
  //Motor and Steering control
  
  // Attach the ESC on pin 9
  ESC.attach(9); // (pin, min pulse width, max pulse width in microseconds) 
  ESC.write(8);
  delay(2000);
  ESC.write(0);
  
  //Attach LEME to pin 10
  LEME.attach(10); // (pin, min pulse width, max pulse width in microseconds) 
  LEME.write(5);
  
  //USB to Raspi
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  
  //LoRa Initialization
  if (!lora.init())
    Serial.println("init failed");
  lora.setFrequency(868.1);
  lora.setModemConfig(RH_RF95::Bw125Cr45Sf128);
  // Defaults after init are 434.0MHz, 13dBm, Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on
  // You can change the modulation parameters with eg
  // lora.setModemConfig(RH_RF95::Bw125Cr45Sf128);
  // The default transmitter power is 13dBm, using PA_BOOST.
  // If you are using RFM95/96/97/98 modules which uses the PA_BOOST transmitter pin, then 
  // you can set transmitter powers from 2 to 20 dBm:
  //  lora.setTxPower(20, false);
  // Failure to do that will result in extremely low transmit powers.
}

void loop()
{
  
  RecvMessageLoRa();
  delay(350);

}

void serialEvent(){
  char readserial[RH_RF95_MAX_MESSAGE_LEN]={0};
  uint8_t i = 0;
  while(Serial.available()){
      readserial[i]=Serial.read();
      i++;
  }

  /*
  if(readserial[1]=='D'){
    Steering(readserial);
    if(readserial[6]=='S'){
      speedcontrol(readserial)
    }
  }
  if (readserial[1]=='S'){
    speedcontrol(readserial);
    if(readserial[6]=='D'){
      Steering(readserial)
    }
  }
  
  */
  SendMessageLoRa(readserial, i);
  i=0;
  //Serial.println(readserial);
}



void SendMessageLoRa(uint8_t *data, int datasize){
  lora.send(data, datasize);
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
//      Serial.println("Connected!");
      if(len){
        //Serial.print("Data Received:");
        Serial.println((char*)buf);
      }
    }
    else
    {
      Serial.println("Fail Recv Message");
    }
  }
  else
  {
    Serial.println("No LoRa");
  }
}

void write2esc(int Value){
 Value = map(Value, 0, 10, 1000, 1800);   // scale it to use it with the servo library (value between 0 and 180)
 ESC.write(Value);    // Send the signal to the ESC
}

void write2leme(int Value){
 Value = map(Value, 0, 10, 1000, 1800);   // scale it to use it with the servo library (value between 0 and 180)
 LEME.write(Value);    // Send the signal to the ESC
}
