
#include <NewPing.h>

#define TRIGGER_PIN  9  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     10  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 100 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.


int count = 0;
const int motorSpeed = 1000;
const int countsperrev = 32*16;
const int countsperflap = countsperrev / 16;

// can do > 1100 uS
int lookup[8] = {B1000,
                 B1000,
                 B0100,
                 B0110,
                 B0010,
                 B0011,
                 B0001,
                 B1001};

// can do > 800 uS
//                abcd
int wave[12] =  {B1000,
                 B1000,
                 B1000,
                 B0100,
                 B0100,
                 B0100,
                 B0010,
                 B0010,
                 B0010,
                 B0001,
                 B0001,
                 B0001}; 

//                   abcd
int fullStep[12] = {B0110,
                    B0011,
                    B0011,
                    B1001,
                    B1001,
                    B1100,
                    B0110,
                    B0110,
                    B0111,
                    B0011,
                    B1001,
                    B1001};

//                   abcd
int halfStep[9] = { B0001,
                    B1001,
                    B1000,
                    B1100,
                    B0100,
                    B0110,
                    B0010,
                    B0011,
                    B0001};

                      

void setup() {
//               76543210
  DDRD = DDRD | B00111100; // Set pins 2-5 as outputs
  pinMode(8, OUTPUT);
  pinMode(11, OUTPUT);
  digitalWrite(8, HIGH);
  digitalWrite(11, LOW);
}

void loop(){
  if(count < countsperflap) {
    clockwise();
    count++;
  } else {
    measureDistanceAndDelay();
    count = 0; 
  }
}

void measureDistanceAndDelay() {
  int ms = sonar.ping_median(10);
  int cm = sonar.convert_cm(ms);
  if(cm < 1) {
    cm = 100;
  }
  int interval = map(cm, 1, 100, 100, 3000);
  delay(interval);
}

void clockwise()
{
  for(int i = 8; i >= 0; i--)
  {
    // Set the pin states for this step
    PORTD = (halfStep[i] << 2);
    delayMicroseconds(motorSpeed);
  }
  // Set all pins low
  PORTD = (B0000 << 2);
}

