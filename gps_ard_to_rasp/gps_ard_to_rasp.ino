
#include <LoRa.h>
#include <SPI.h>
#include <TinyGPS.h>

#define WAITTIME 10000UL  //Miliseconds

TinyGPS gps;

unsigned long last_send = 0;

const char TYPE = 'O';
const unsigned short ID = 2U;
float LAT;
float LNG;
float CRS;
float SPD;
const float SIZ = 1.0;

void setup() {
  // start serial port at 9600 bps:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(2, INPUT);   // digital sensor is on digital pin 2
  establishContact();  // send a byte to establish contact until receiver responds
}

void loop() {
  // if we get a valid byte, read analog ins:
  while (Serial.available() > 0) {

    // get incoming byte:
    //inByte = Serial.read();
    // read first analog input, divide by 4 to make the range 0-255:
    //firstSensor = analogRead(A0) / 4;
    // delay 10ms to let the ADC recover:
    //delay(10);
    // read second analog input, divide by 4 to make the range 0-255:
    //secondSensor = analogRead(1) / 4;
    // read switch, map it to 0 or 255L
    //thirdSensor = map(digitalRead(2), 0, 1, 0, 255);
    // send sensor values:
    gps.encode(Serial.read());
    if (millis() - last_send > WAITTIME) {
        unsigned long age;
        gps.f_get_position(&LAT, &LNG, &age);
        CRS = gps.f_course();
        SPD = gps.f_speed_knots();

        if (LAT == TinyGPS::GPS_INVALID_F_ANGLE || LNG == TinyGPS::GPS_INVALID_F_ANGLE || CRS == TinyGPS::GPS_INVALID_F_ANGLE || SPD == TinyGPS::GPS_INVALID_F_SPEED) {
            //dont send
        } else {
          Serial.print("&TYPE");  
          Serial.print("&ID");  
          Serial.print("&LAT");  
          Serial.print("&LNG");  
          Serial.print("&SIZ");  
          Serial.print("&SPD");  
        }
        
/*            memcpy(msg_buf, &TYPE, 1);
            memcpy(msg_buf + 1, &ID, 1);
            memcpy(msg_buf + 2, &LAT, 4);
            memcpy(msg_buf + 6, &LNG, 4);
            memcpy(msg_buf + 10, &SIZ, 4);
            memcpy(msg_buf + 14, &CRS, 4);
            memcpy(msg_buf + 18, &SPD, 4);
*/        }
    last_send = millis();


    
    //LoRa.beginPacket();
    //LoRa.write(msg_buf, 22);
    //LoRa.endPacket();
    //LoRa.sleep();
    //Serial.write(firstSensor);
    //Serial.write(secondSensor);
    //Serial.write(thirdSensor);
  }
}

void establishContact() {
  int i=0;
  if (Serial.available() <= 0) {
    i++;
    Serial.print('H');   // send a capital A
    Serial.print('3');
    Serial.print('3');
    Serial.print('L');
    Serial.print('P');
    //delay(300);
  }
}

/* Processing sketch to run with this example:

  // This example code is in the public domain.

  import processing.serial.*;

  int bgcolor;           // Background color
  int fgcolor;           // Fill color
  Serial myPort;                       // The serial port
  int[] serialInArray = new int[3];    // Where we'll put what we receive
  int serialCount = 0;                 // A count of how many bytes we receive
  int xpos, ypos;                // Starting position of the ball
  boolean firstContact = false;        // Whether we've heard from the microcontroller

  void setup() {
    size(256, 256);  // Stage size
    noStroke();      // No border on the next thing drawn

    // Set the starting position of the ball (middle of the stage)
    xpos = width / 2;
    ypos = height / 2;

    // Print a list of the serial ports for debugging purposes
    // if using Processing 2.1 or later, use Serial.printArray()
    println(Serial.list());

    // I know that the first port in the serial list on my Mac is always my FTDI
    // adaptor, so I open Serial.list()[0].
    // On Windows machines, this generally opens COM1.
    // Open whatever port is the one you're using.
    String portName = Serial.list()[0];
    myPort = new Serial(this, portName, 9600);
  }

  void draw() {
    background(bgcolor);
    fill(fgcolor);
    // Draw the shape
    ellipse(xpos, ypos, 20, 20);
  }

  void serialEvent(Serial myPort) {
    // read a byte from the serial port:
    int inByte = myPort.read();
    // if this is the first byte received, and it's an A, clear the serial
    // buffer and note that you've had first contact from the microcontroller.
    // Otherwise, add the incoming byte to the array:
    if (firstContact == false) {
      if (inByte == 'A') {
        myPort.clear();          // clear the serial port buffer
        firstContact = true;     // you've had first contact from the microcontroller
        myPort.write('A');       // ask for more
      }
    }
    else {
      // Add the latest byte from the serial port to array:
      serialInArray[serialCount] = inByte;
      serialCount++;

      // If we have 3 bytes:
      if (serialCount > 2 ) {
        xpos = serialInArray[0];
        ypos = serialInArray[1];
        fgcolor = serialInArray[2];

        // print the values (for debugging purposes only):
        println(xpos + "\t" + ypos + "\t" + fgcolor);

        // Send a capital A to request new sensor readings:
        myPort.write('A');
        // Reset serialCount:
        serialCount = 0;
      }
    }
  }

*/
