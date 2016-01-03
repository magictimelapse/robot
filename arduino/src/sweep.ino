/* Sweep
  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>

Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);
#define DEBUG false
#define DPHI 2
const int ndists = 180/DPHI;
float dists[ndists];
const int startDist = 90;
int currentDist = 0;
int analogPin = 0;
int val = 0;
// button with ISR
int sensePin = 2;
int ledPin = 13;

volatile bool running = false;
 int buttonState;             // the current reading from the input pin
int lastButtonState = LOW;   // the previous reading from the input pin

// the following variables are long's because the time, measured in miliseconds,
// will quickly become a bigger number than can be stored in an int.
long lastDebounceTime = 0;  // the last time the output pin was toggled
long debounceDelay = 50;    // the debounce time; increase if the output flickers

void checkButtonISR(){
  // read the state of the switch into a local variable:
  int reading = digitalRead(sensePin);

  // check to see if you just pressed the button
  // (i.e. the input went from LOW to HIGH),  and you've waited
  // long enough since the last press to ignore any noise:

  // If the switch changed, due to noise or pressing:
  if (reading != lastButtonState) {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:

    // if the button state has changed:
    if (reading != buttonState) {
      buttonState = reading;

      // only toggle the LED if the new button state is HIGH
      if (buttonState == LOW) {
        running = !running;
      }
    }
  }

  // set the LED:
  digitalWrite(ledPin, running);

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = reading;



}


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
const int timeDelay = 40;
void displaySensorDetails(void)
{
  sensor_t sensor;
  mag.getSensor(&sensor);
  Serial.println("------------------------------------");
  Serial.print  ("Sensor:       "); Serial.println(sensor.name);
  Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
  Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
  Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" uT");
  Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" uT");
  Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" uT");
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}


const float maxDist = 80.;
void setup() {

  myservo.attach(A3);  // attaches the servo on pin 9 to the servo object
  Serial.begin(115200);
  if (DEBUG)
    Serial.print("beginning..\n");
  myservo.write(startDist);
  currentDist = startDist;
  for(int i=0; i<180; i+=DPHI){
   	  dists[i/DPHI] = maxDist;
  }
  mag.begin();
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin,LOW);
  //attachInterrupt(digitalPinToInterrupt(sensePin), checkButtonISR, CHANGE);
  pinMode(sensePin,INPUT_PULLUP);
  //attachInterrupt(0, checkButtonISR, FALLING);

}
float distance(int val)
{
  float voltage = 1.0 * val / 256 * 3;
  //if (voltage > 3) {
  // voltage = 3.0;
  //}
  //if (voltage < 0.4) {
  //  voltage = 0.4;
  // }
  float maxVoltage = 6.0;
  float minVoltage = 0.2;
  float minDist = 10.;

  float dist = minDist + (maxDist - minDist) / (maxVoltage - minVoltage) * (maxVoltage - voltage);
  if (dist < 0.) {
    dist = 0;
  }
  if (dist > maxDist) {
    dist = maxDist;
  }
  return dist;


}

float getHeading() {
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);
  float heading = atan2(event.magnetic.y, event.magnetic.x);
  float declinationAngle = -0.033; // in rad
  heading += declinationAngle;
  if (heading < 0)
    heading += 2 * PI;

  // Check for wrap due to addition of declination.
  if (heading > 2 * PI)
    heading -= 2 * PI;
  float headingDegrees = heading * 180 / M_PI;
  return headingDegrees;
}

void sweep(const int &start, const int &stop){
     if(abs(currentDist - start) > 15) {
        myservo.write(start);
	delay(1000);
     }

     if(start<stop){
      for(int i=start;i<stop;i+=DPHI)
      {
	myservo.write(i);
	//digitalWrite(ledPin,LOW);
	delay(timeDelay/2);
	//digitalWrite(ledPin,HIGH);
	delay(timeDelay/2);
	val = analogRead(analogPin);
	dists[i/DPHI] = distance(val);
      }
     } 
     if(stop<start){
      for(int i=start-1;i>=stop;i-=DPHI) {
	//digitalWrite(ledPin,LOW);
	       myservo.write(i);
               delay(timeDelay/2);
	       // digitalWrite(ledPin,HIGH);
	       delay(timeDelay/2);
               val = analogRead(analogPin);
               dists[i/DPHI] = distance(val);
	}
     }
     currentDist = stop;
}

void sWriteDists()
{
  Serial.println("###");
  Serial.println(ndists);
  Serial.write((byte*)dists,ndists*sizeof(float));
  //Serial.println("");
}



int startByte  = 0;
int stopByte = 0;
char command =' ';
float heading = 0.0;
void loop() {
  checkButtonISR();
  if(Serial.available() > 0)
    {
      command = (char)(Serial.read());
      Serial.println(command);
      if(command == 's'){
	while(Serial.available() == 0){
	}
	startByte = Serial.read();
	Serial.println(startByte);
	while(Serial.available() == 0){}
	stopByte = Serial.read();
	Serial.println(stopByte);
	sweep(startByte, stopByte);
	//Serial.println("sweeeppp!");
	sWriteDists();  
	
      }
      if(command == 'd'){
	 val = analogRead(analogPin);
	 float myDist = distance(val);
	 Serial.println(myDist);
      }
      
      if(command == 'h'){
	heading = getHeading();
	Serial.println(heading);
      }
      if(command == 'r') {
	if(running)
	  Serial.println("1");
	else
	  Serial.println("0");
      }
    }
}
