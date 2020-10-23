/*
  Blink

  Turns an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino
  model, check the Technical Specs of your board at:
  https://www.arduino.cc/en/Main/Products

  modified 8 May 2014
  by Scott Fitzgerald
  modified 2 Sep 2016
  by Arturo Guadalupi
  modified 8 Sep 2016
  by Colby Newman

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/Blink
*/
#include <SD.h>
#include <SPI.h>
#include <Audio.h>
#include <DAC.h>

  int volP=A2;
int speedP=A10;
const int sf=VARIANT_MCK / 88200 / 8;
// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

pinMode(A0,OUTPUT);
pinMode(A1,OUTPUT);
pinMode(A8,OUTPUT);
pinMode(A9,OUTPUT);
pinMode(volP,INPUT);
pinMode(speedP,INPUT);
digitalWrite(A0,HIGH);
digitalWrite(A8,HIGH);
digitalWrite(A1,LOW);
digitalWrite(A9,LOW);


Serial.begin(250000);
 Serial.print("Initializing SD card...");

  if (!SD.begin(10)) {

    Serial.println(" failed!");

    while(true);

  }

  Serial.println(" done.");

  // hi-speed SPI transfers

  // 44100kHz stereo => 88200 sample rate

  // 100 mSec of prebuffering.

  Audio.begin(88200, 100);
}

// the loop function runs over and over again forever
void loop() {
  
  
   int count = 0;

  // open wave file from sdcard

  File myFile = SD.open("test.wav");

  if (!myFile) {

    // if the file didn't open, print an error and stop

    Serial.println("error opening test.wav");

    while (true);

  }

  const int S = 1024; // Number of samples to read in block

  short buffer[S];

  Serial.print("Playing");

  // until the file is not finished

  while (myFile.available()) {

    // read from the file into buffer

    myFile.read(buffer, sizeof(buffer));

    // Prepare samples

    int volume = 1024;

    Audio.prepare(buffer, S, volume);

    // Feed samples to audio

    Audio.write(buffer, S);

    // Every 100 block print a '.'
   int speed=constrain(((int(analogRead(speedP))-800)/2),-200,20);
  
 int period= sf-speed;
 Serial.println(period); 

  const uint32_t TC = period;
  TC_SetRA(TC0, 1, TC / 2);
  TC_SetRC(TC0, 1, TC);
  //TC_Start(TC0, 1);



    count++;

    if (count == 100) {

      Serial.print(".");

      count = 0;

    }

  }

  myFile.close();

  Serial.println("End of file. Thank you for listening!");

  while (true) ;// wait for a second
}
