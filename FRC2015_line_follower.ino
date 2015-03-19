#include <QTRSensors.h>

#define NUM_SENSORS   7     // number of sensors used
#define TIMEOUT       2500  // waits for 2500 microseconds for sensor outputs to go low
#define EMITTER_PIN   2     // emitter is controlled by digital pin 10, not actually used with this sensor
#define RIO_PIN       10    // pin connected to the roborio
#define HIGH_PIN      11    // used for +5 volts
#define CALIBRATE_PIN 12    // when this pin is pulled low the Arduino will enter calibrate mode

// sensors 0 through 6 are connected to digital pins 3 through 9, respectively
QTRSensorsRC qtrrc((unsigned char[]) {3, 4, 5, 6, 7, 8, 9},
  NUM_SENSORS, TIMEOUT, EMITTER_PIN);
unsigned int sensorValues[NUM_SENSORS];

void setup()
{
  Serial.begin(9600); // set the data rate in bits per second for serial data transmission
  
  //Set the DIO pins to the correct mode
  pinMode(13, OUTPUT);
  pinMode(HIGH_PIN, OUTPUT);
  digitalWrite(HIGH_PIN, OUTPUT);
  pinMode(RIO_PIN, OUTPUT);
  pinMode(CALIBRATE_PIN, INPUT);
  digitalWrite(CALIBRATE_PIN, HIGH);
}


void loop()
{
  if (digitalRead(CALIBRATE_PIN)) {
    // read raw sensor values
    unsigned int position = qtrrc.readLine(sensorValues);

    // print the sensor values as numbers from 0 to 2500, where 0 means maximum reflectance and
    // 2500 means minimum reflectance

    Serial.print(position);
    Serial.print("\t");
    position = map(position, 0, 1000*(NUM_SENSORS-1), 0, 255);
    analogWrite(RIO_PIN, position);
    Serial.print(position);
    Serial.print("\t");
    for (int i = 0; i < NUM_SENSORS; i++) {
      Serial.print(sensorValues[i]);
      Serial.print("\t");
    }
    Serial.println();
  }
  else {
    while (!digitalRead(CALIBRATE_PIN)) {
      digitalWrite(13, HIGH);              //Turn the LED on while Calibrating
      qtrrc.calibrate();
      Serial.println("Calibrating!");
    }
    for (int i = 0; i < NUM_SENSORS; i++) {
      Serial.print(qtrrc.calibratedMinimumOn[i]);
      Serial.print(' ');
    }
    Serial.println();
    delay(5000);
    digitalWrite(13, LOW);
  }
}
