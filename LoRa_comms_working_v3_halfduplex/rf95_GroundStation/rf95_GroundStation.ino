// rf95_server.pde

#include <SPI.h>
#include <RH_RF95.h>

// Singleton instance of the radio driver
RH_RF95 lora;

//Buffer for the Receiver in LoRa (RecvMessageLoRa)
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN]; 

void setup() 
{
  // Ensure serial flash is not interfering with radio communication on SPI bus
  //  pinMode(4, OUTPUT);
  //  digitalWrite(4, HIGH);

  pinMode(led, OUTPUT);     
  Serial.begin(9600);
  while (!Serial) ; // Wait for serial port to be available
  if (!lora.init())
    Serial.println("init failed");  
}

void loop()
{  

  RecvMessageLoRa();
  delay(1200);
  
  //Read from serial//////////////////////////////////////
  char readserial[RH_RF95_MAX_MESSAGE_LEN]={0};
  uint8_t i = 0;
  while(Serial.available()>1 ){readserial[i]=Serial.read();i++;}
  delay(300);
  SendMessageLoRa(readserial, i);//size of array needs to be passed as i
  
  i=0;
}


void SendMessageLoRa(uint8_t *data, int datasize){

    if (lora.available()){
      lora.send(data, datasize);
      lora.waitPacketSent();
      //Serial.println(sizeof(data));//this output is wrong because its half?
      Serial.println((char*)data);
      digitalWrite(led, LOW);
    }
    else
    {
      Serial.println("Autonomous Boats not found");
    }
}
void RecvMessageLoRa(){
    if (lora.available())
    {
      //Serial.print("LoRa is available ");
      
      // The Message to be received (recv).   
      //uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
      uint8_t len = sizeof(buf);
      if (lora.recv(buf, &len))
      {
     // RH_RF95::printBuffer("request: ", buf, len);
        Serial.print("Received Message: ");
        Serial.println((char*)buf);
      }
    }
}
