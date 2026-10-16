#include <Servo.h>
#include <ArduinoJson.h>

#define Shoulder 3
#define Turret 5
#define ElbowP 6

// Pins for all TFBGN66 whatever thingy
#define AIN1 2
#define AIN2 7
#define PWMA 11
#define STBY 8


//for usb serial messages
char message[256];
size_t messageLength = 0;
bool discardMessage = false;


class DC_Motor {
private:
  byte in1Pin;
  byte in2Pin;
  byte pwmPin;
  byte standbyPin;

  int speed = 100;                 // PWM: 0–255
  unsigned long duration = 150;    // milliseconds
  unsigned long startTime = 0;
  bool active = false;

public:
  DC_Motor(byte in1, byte in2, byte pwm, byte standby) {
    this->in1Pin = in1;
    this->in2Pin = in2;
    this->pwmPin = pwm;
    this->standbyPin = standby;
  }

  void wire() {
    pinMode(this->in1Pin, OUTPUT);
    pinMode(this->in2Pin, OUTPUT);
    pinMode(this->pwmPin, OUTPUT);
    pinMode(this->standbyPin, OUTPUT);

    digitalWrite(this->standbyPin, LOW);
    digitalWrite(this->in1Pin, LOW);
    digitalWrite(this->in2Pin, LOW);
    analogWrite(this->pwmPin, 0);

    digitalWrite(this->standbyPin, HIGH);
  }

  void move(int direction) {
    if (direction == 0) {
      this->stop();
      return;
    }

    if (direction != 1 && direction != -1) {
      return;
    }

    // Ignore new nudges until this one finishes.
    if (this->active) {
      return;
    }

    if (direction == 1) {
      digitalWrite(this->in1Pin, LOW);
      digitalWrite(this->in2Pin, HIGH);
    } else {
      digitalWrite(this->in1Pin, HIGH);
      digitalWrite(this->in2Pin, LOW);
    }

    this->startTime = millis();
    this->active = true;
    analogWrite(this->pwmPin, this->speed);
  }

  void stop() {
    analogWrite(this->pwmPin, 0);
    this->active = false;
  }

  void update() {
    if (this->active &&
        millis() - this->startTime >= this->duration) {
      this->stop();
    }
  }
};



  
DC_Motor er1(AIN1,AIN2,PWMA, STBY);

//hardware setup
Servo ShoulderServo;
Servo TurretServo;
Servo ElbowPitchServo;


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  ShoulderServo.attach(Shoulder);
  TurretServo.attach(Turret);
  ElbowPitchServo.attach(ElbowP);
  er1.wire();
}

void handleMsg(const char*text){
  
  JsonDocument angles;
  // deseralize the message and stor the error in err if there is one
  DeserializationError err = deserializeJson(angles, text);

   if (err) { //ignore the messaage if an error happens 
      Serial.print("Invalid JSON: ");
      Serial.println(err.c_str());
      return;
    }

  if(angles["shoulder_pitch"].is<float>()){
    int ShoulderTarget = constrain(round(angles["shoulder_pitch"].as<float>()), 0, 180);
    ShoulderServo.write(ShoulderTarget);
  }

  if(angles["elbow_pitch"].is<float>()){
    int EPitchTarget = constrain(round(angles["elbow_pitch"].as<float>()), 0, 180);
    ElbowPitchServo.write(EPitchTarget);
  }

  if(angles["turret"].is<float>()){
    int TurretTarget = constrain(round(angles["turret"].as<float>()), 0, 180);
    TurretServo.write(TurretTarget);
  }

  if(angles["elbow_roll"].is<int>()){
    er1.move(angles["elbow_roll"].as<int>());
  }

}

//message is decalred above at the top of the file! 
void loop() {
  er1.update();

  if (Serial.available() > 0) {
    //read character by character so you can continuosly run er1.update 
    char received = Serial.read();

    //if recieved message has completed (you see a newline char)
    if (received == '\n') {
      if (!discardMessage && messageLength > 0) {
        message[messageLength] = '\0';
        handleMsg(message);
      }

      messageLength = 0;
      discardMessage = false;

    } 
    else if (received != '\r' && !discardMessage) {
      if (messageLength < sizeof(message) - 1) {
        message[messageLength++] = received;
      } else {
        discardMessage = true;
      }
    }
  }
}
