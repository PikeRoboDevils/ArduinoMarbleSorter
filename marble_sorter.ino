/*
 * Team 1018 Marble Sorter
 * Version 1.0
 * With help from Alex Pikkert
 * 
 * The Adafruit library for the sensor and a tutorial can be downloaded here:
 * https://www.adafruit.com/product/1334
*/
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_TCS34725.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_50MS, TCS34725_GAIN_4X);

int i;                 // counter used to display the counted number of marbles of a certain color
int yellownumber;      // the number of yellow marbles
int greennumber;       // the number of green marbles
int bluenumber;        // the number of blue marbles
int rednumber;         // the number of red marbles
int amountondisplay;   // the amount of marbles shown on the display
/*
   Total distance is 90 degrees
*/
int yellowball = 41;    // aiming the kick mechanism to the yellow storage lane
int greenball = 25;   // aiming the kick mechanism to the green storage lane
int blueball = 61;    // aiming the kick mechanism to the blue storage lane
int redball = 81;     // aiming the kick mechanism to the red storage lane
int halt = yellowball;        // aiming the kick mechanism to the halt position
int delaytime = 800;    // general time delay used in all subroutines
uint16_t clearcolor, red, green, blue; // variables used by the TCS34725 color sensor
float average, r, g, b;                // variables used by the TCS34725 color sensor

int kickServoPort = 3;        //Green wire
//int ledDisplay = 9;           //yellow wire
int colorSelectServoPort = 5; //blue wire
//int resetButton = A0;         //white wire
Servo colorSelect;
Servo kickServo;
void setup() {
  kickServo.attach(kickServoPort);  // kickservo, MP3 player and Synth speaker and reset number+ on D1
  kickServo.write(0);
  colorSelect.attach(colorSelectServoPort);   // storage select servo,dimmer and hand pointer servo on D5
  colorSelect.write(halt);   // initial halt position servo on D5
  //pinMode(resetButton, INPUT_PULLUP);
  //pinMode(ledDisplay, OUTPUT);  // number+ on D9
  //Serial.begin(9600);
  //Serial.setTimeout(25);
}

void loop() {
  
  measure_RGB();
  //Serial.print("\tRed:\t"); Serial.print(r);Serial.print("\tGreen:\t"); Serial.print(g);Serial.print("\tBlue:\t"); Serial.print(b);Serial.println();
  // the core process from the color sensor, measurung red, green and blue values from each marble
  if (r > 1 && g < 1) { // select, aim and kick a red marble
    Serial.println("red");
    kickred();
  }
  if (g > 1.1 && r < 0.8 && b < 1.3) { // select, aim and kick a green marble
    Serial.println("green");
    kickgreen();
  }
  if (b > 1.2 && r < 0.9) { // select, aim and kick a blue marble
    Serial.println("blue");
    kickblue();
  }
  if (r > 1 && g > 1 && b < 1) { // select, aim and kick a yellow marble
    Serial.println("yellow");
    kickyellow();
  }
}

void measure_RGB() { // the core process from the color sensor, measuring red, green and blue values from each marble
  delay(delaytime * 2);  // it takes a minimum of 50ms to read
  tcs.getRawData(&red, &green, &blue, &clearcolor);
  average = (red + green + blue) / 3;
  r = red / average;
  g = green / average;
  b = blue / average;
}

void kick() {// the kicking movement to send a marble to a storage lane with servo on D1 (and ring the MP3 bell)
  delay(delaytime);
  kickServo.write(180);
  delay(delaytime * 2);
  kickServo.write(0);
}


// set the correct storage lane, point  to the correct marble with the hand pointer,
// add up the amount of marbles, show this on the display and kick the marble into the storage lane
// if there is no marble detected, go to the halt position
void kickyellow() {
  delay(delaytime);
  colorSelect.write(yellowball);
  yellownumber++;
  amountondisplay = yellownumber;
  
  kick();
}
void kickgreen() {
  delay(delaytime);
  colorSelect.write(greenball);
  greennumber++;
  amountondisplay = greennumber;
  kick();
}
void kickblue() {
  delay(delaytime);
  colorSelect.write(blueball);
  bluenumber++;
  amountondisplay = bluenumber;
  kick();
}
void kickred() {
  delay(delaytime);
  colorSelect.write(redball);
  rednumber++;
  amountondisplay = rednumber;
  kick();
}
void haltposition() {
  delay(delaytime);
  colorSelect.write(halt);
}
