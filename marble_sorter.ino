/*
  Magical Marble Sorting Machine Revisited
  Color sensor from Adafruit type TCS34725 connected to SDA, SCL, GND and VCC on the Arduino Leonardo bit
  Power via IR sensor on A0
  Number+ on D9
  Kickservo, MP3 player and Synth speaker and reset number+ on D1
  Storage select servo,dimmer and hand pointer servo on D5
  D5 set to analog, D9 set to PWM
  This is the original color sensor program that I used from Adi Dax:
  http://www.makerblog.at/2015/01/farben-erkennen-mit-dem-rgb-sensor-tcs34725-und-dem-arduino/
  (It has a very good trick to get a stable output, dividing the color numbers by their average value)

  The Adafruit library for the sensor and a tutorial can be downloaded here:
  https://www.adafruit.com/product/1334


  Alex Pikkert November 2016
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
int yellowball = 0;    // aiming the kick mechanism to the yellow storage lane
int greenball = 30;   // aiming the kick mechanism to the green storage lane
int blueball = 60;    // aiming the kick mechanism to the blue storage lane
int redball = 90;     // aiming the kick mechanism to the red storage lane
int halt = yellowball;        // aiming the kick mechanism to the halt position
int delaytime = 800;    // general time delay used in all subroutines
uint16_t clearcolor, red, green, blue; // variables used by the TCS34725 color sensor
float average, r, g, b;                // variables used by the TCS34725 color sensor

int kickServoPort = 3;        //Green wire
int ledDisplay = 9;           //yellow wire
int colorSelectServoPort = 5; //blue wire
int resetButton = A0;         //white wire
Servo colorSelect;
Servo kickServo;
void setup() {
  kickServo.attach(kickServoPort);  // kickservo, MP3 player and Synth speaker and reset number+ on D1
  kickServo.write(0);
  colorSelect.attach(colorSelectServoPort);   // storage select servo,dimmer and hand pointer servo on D5
  colorSelect.write(halt);   // initial halt position servo on D5
  pinMode(resetButton, INPUT_PULLUP);
  pinMode(ledDisplay, OUTPUT);  // number+ on D9
  Serial.begin(9600);
  Serial.setTimeout(25);
}

void loop() {
  /*
    String s;
    int i = 0;
    if(Serial.available() > 0) {
    s = Serial.readString();
    }
    if(s == "red") {
    i = 1;
    } else if(s == "green") {
    i = 2;
    } else if(s == "blue") {
    i = 3;
    } else if(s == "yellow") {
    i = 4;
    } else if(s == "halt") {
    i = 5;
    } else if(s == "show") {
    i = 6;
    }

    switch(i) {
    case 0: {
      haltposition();
      break;
    }
    case 1: {
      kickred();
      break;
    }
    case 2: {
      kickgreen();
      break;
    }
    case 3: {
      kickblue();
      break;
    }
    case 4: {
      kickyellow();
      break;
    }
    case 5: {
      haltposition();
      break;
    }
    case 6: {
      showamounts();
      break;
    }
    }*/
  measure_RGB();
  // the core process from the color sensor, measurung red, green and blue values from each marble
  if (r > 1.35) { // select, aim and kick a red marble
    Serial.println("red");
    kickred();
  }
  if (g > 1.25) { // select, aim and kick a green marble
    Serial.println("green");
    kickgreen();
  }
  if (r < 1 && g > 1 && g < 1.25) { // select, aim and kick a blue marble
    Serial.println("blue");
    kickblue();
  }
  if (r > 1 && b > 0.75) { // select no marble and go to halt position
    Serial.println("halt");
    haltposition();
  }
  if (r > 1 && r < 1.3 && b < 0.65 ) { // select, aim and kick a yellow marble
    Serial.println("yellow");
    kickyellow();
  }
  if (digitalRead(resetButton) == LOW) { // check if there is an IR signal on A0 and start the final counting procedure
    Serial.println("reset");
    showamounts();
  }

}

void measure_RGB() { // the core process from the color sensor, measuring red, green and blue values from each marble
  delay(delaytime);  // it takes a minimum of 50ms to read
  tcs.getRawData(&red, &green, &blue, &clearcolor);
  average = (red + green + blue) / 3;
  r = red / average;
  g = green / average;
  b = blue / average;
}

void kick() {// the kicking movement to send a marble to a storage lane with servo on D1 (and ring the MP3 bell)
  delay(delaytime * 2);
  kickServo.write(180);
  delay(delaytime / 2);
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
  countdisplay();
  kick();
}
void kickgreen() {
  delay(delaytime);
  colorSelect.write(greenball);
  greennumber++;
  amountondisplay = greennumber;
  countdisplay();
  kick();
}
void kickblue() {
  delay(delaytime);
  colorSelect.write(blueball);
  bluenumber++;
  amountondisplay = bluenumber;
  countdisplay();
  kick();
}
void kickred() {
  delay(delaytime);
  colorSelect.write(redball);
  rednumber++;
  amountondisplay = rednumber;
  countdisplay();
  kick();
}
void haltposition() {
  delay(delaytime);
  colorSelect.write(halt);
}

void countdisplay() { // show the correct number of marbles on the display on D9
  for (i = 0; i < amountondisplay; i++) {
    digitalWrite(ledDisplay, HIGH);
    delay(10);
    digitalWrite(ledDisplay, LOW);
    delay(10);
  }
}

void showamounts() {// the final counting procedure and set all counted numbers to zero
  kick();
  colorSelect.write(redball);
  amountondisplay = rednumber;
  countdisplay();
  delay(delaytime);
  kick();
  colorSelect.write(blueball);
  amountondisplay = bluenumber;
  countdisplay();
  delay(delaytime);
  kick();
  colorSelect.write(greenball);
  amountondisplay = greennumber;
  countdisplay();
  delay(delaytime);
  kick();
  colorSelect.write(yellowball);
  amountondisplay = yellownumber;
  countdisplay();
  delay(delaytime);
  yellownumber = 0;
  greennumber = 0;
  bluenumber = 0;
  rednumber = 0;
  kick();
  haltposition();
}

