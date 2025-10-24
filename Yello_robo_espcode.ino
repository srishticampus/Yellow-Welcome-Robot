#include "BluetoothSerial.h"
#include <ESP32Servo.h>

BluetoothSerial SerialBT;

// ===== Motor Driver (Single L298N) =====
const int IN1 = 27;  // Left motor
const int IN2 = 26;
const int IN3 = 25;  // Right motor
const int IN4 = 23;

// ===== Servos =====
Servo servo1;
Servo servo2;
const int servo1Pin = 4;
const int servo2Pin = 2;

// ===== Variables =====
char command;
int pos1 = 0, pos2 = 0;
bool servo1Active = false;
bool servo2Active = false;

// ===== Smooth Servo Function =====
void moveServoSmooth(Servo &servo, int &pos, int targetAngle) {
  int step = (targetAngle > pos) ? 1 : -1;
  while (pos != targetAngle) {
    pos += step;
    servo.write(pos);
    delay(15);
  }
}

// ===== Car Motion Functions =====
void stopCar() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void forward() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);
}

void backward() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);
}

void left() {
  digitalWrite(IN1, LOW); digitalWrite(IN2, HIGH);   // left motor reverse
  digitalWrite(IN3, HIGH); digitalWrite(IN4, LOW);   // right motor forward
}

void right() {
  digitalWrite(IN1, HIGH); digitalWrite(IN2, LOW);   // left motor forward
  digitalWrite(IN3, LOW); digitalWrite(IN4, HIGH);   // right motor reverse
}

// ===== Servo Control =====
void controlServo1() {
  if (servo1Active)
    moveServoSmooth(servo1, pos1, 0);
  else
    moveServoSmooth(servo1, pos1, 90);
  servo1Active = !servo1Active;
}

void controlServo2() {
  if (servo2Active)
    moveServoSmooth(servo2, pos2, 0);
  else
    moveServoSmooth(servo2, pos2, 90);
  servo2Active = !servo2Active;
}

// ===== Setup =====
void setup() {
  Serial.begin(9600);
  SerialBT.begin("ESP32_RC_Car");  // Bluetooth name

  // Motor pins
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  stopCar();

  // Servo setup
  servo1.attach(servo1Pin);
  servo2.attach(servo2Pin);
  servo1.write(0);
  servo2.write(0);

  Serial.println("✅ ESP32 RC Car Ready! Connect via Bluetooth.");
}

// ===== Main Loop =====
void loop() {
  if (SerialBT.available()) {
    command = SerialBT.read();
    Serial.print("Received: ");
    Serial.println(command);

    switch (command) {
      case 'F': forward(); break;
      case 'B': backward(); break;
      case 'L': left(); break;
      case 'R': right(); break;
      case 'S': stopCar(); break;
      case 'X': controlServo1(); break;
      case 'Y': controlServo2(); break;
    }
  }
}
