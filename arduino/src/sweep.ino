/* Sweep
  by BARRAGAN <http://barraganstudio.com>
  This example code is in the public domain.

  modified 8 Nov 2013
  by Scott Fitzgerald
  http://www.arduino.cc/en/Tutorial/Sweep
*/

#include <Servo.h>

#define DEBUG false
#define DPHI 2
const int ndists = 180/DPHI;
float dists[ndists];

int analogPin = 5;
int val = 0;
Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;    // variable to store the servo position
const int timeDelay = 30;
void setup() {

  myservo.attach(A3);  // attaches the servo on pin 9 to the servo object
  Serial.begin(115200);
  if (DEBUG)
    Serial.print("beginning..\n");
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
  float maxDist = 80.;
  float dist = minDist + (maxDist - minDist) / (maxVoltage - minVoltage) * (maxVoltage - voltage);
  if (dist < 0.) {
    dist = 0;
  }
  if (dist > maxDist) {
    dist = maxDist;
  }
  return dist;


}
void loop() {
  bool printDist = false;
  val = analogRead(analogPin);
  float dist = distance(val);
  if (printDist)
    Serial.println(dist);

  //delay(100);
  if (printDist)
    Serial.println(120);

  int cnt = 0;
  for (pos = 0; pos < 180; pos += DPHI) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    //myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(timeDelay);                       // waits 15ms for the servo to reach the position
    val = analogRead(analogPin);
    float dist = distance(val);
    if (printDist)
      Serial.println(dist);
    dists[cnt++] = dist;
  }
  cnt--;
  Serial.println("###");
  Serial.println(ndists);
  Serial.write((byte*)dists,ndists*sizeof(float));
  Serial.println("");
  if (DEBUG) {
    Serial.print("cnt: ");
    Serial.println(cnt);
  }
  for (pos = 179; pos >= 0; pos -= DPHI) { // goes from 180 degrees to 0 degrees
    //myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(timeDelay);                       // waits 15ms for the servo to reach the position
    val = analogRead(analogPin);
    float dist = distance(val);
    if (printDist)
      Serial.println(dist);
    dists[cnt--] = dist;
  }
  Serial.println("###");
  Serial.println(ndists);
  Serial.write((byte*)dists,ndists*sizeof(float));
  Serial.println("");
  if (DEBUG) {
    Serial.print("cnta: ");
    Serial.println(cnt);
  }
}
