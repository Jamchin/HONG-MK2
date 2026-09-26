#include <ESP32Servo.h>
#include <ArduinoJson.h>

#define Shoulder 19 
#define Turret 18 


//hardware setup
Servo ShoulderServo;
Servo TurretServo;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ShoulderServo.attach(Shoulder);
  TurretServo.attach(Turret);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Grabs any data(JSON Text) sitting in the buffer 
  //(Transmitting Data via USB Serial)
   if (Serial.available() > 0) {
    String message = Serial.readStringUntil('\n');
    JsonDocument angles;
    DeserializationError err = deserializeJson(angles, message);

    if (err) {
      Serial.print("Invalid JSON: ");
      Serial.println(err.c_str());
      return;
    }

    if (!angles["shoulder_pitch"].is<float>() ||
        !angles["turret"].is<float>()) {
      Serial.println("WARNING: missing or nonnumeric angle");
      return;
    }

    int ShoulderTarget = constrain(round(angles["shoulder_pitch"].as<float>()), 0, 180);

    int TurretTarget = constrain(round(angles["turret"].as<float>()), 0, 180);

    ShoulderServo.write(ShoulderTarget);
    TurretServo.write(TurretTarget);


    Serial.print("Received: ");
    Serial.println(message);
  }

}
