#include <ESP32Servo.h>
#include <ArduinoJson.h>

#define Shoulder 19 
#define Elbow 18 

Servo ShoulderServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Grabs any data(JSON Text) sitting in the buffer 
  //(Transmitting Data via USB Serial)
   if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    Serial.print("Received: ");
    Serial.println(message);
  }

}
