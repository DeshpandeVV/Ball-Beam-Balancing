#include <Servo.h>

// Pins
const int trigPin = 6;
const int echoPin = 7;
const int servoPin = 9;

// Servo
Servo myServo;

// === TUNE HERE ===
float Kp = 5000.0;  // Start low (e.g. 2.0) and increase until you get sustained oscillation
float Ki = 20.0;
float Kd = 1000.0;

// PID Variables
float setpoint = 12.0; // Desired position in cm
float input, output;
float lastError = 0;
float integral = 0;

// Time
unsigned long lastTime;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  myServo.attach(servoPin);
  myServo.write(90);  // Start neutral
  delay(1000);
  lastTime = millis();
}

void loop() {
  input = readUltrasonic();
  float error = setpoint - input;

  // Time step
  unsigned long currentTime = millis();
  float deltaTime = (currentTime - lastTime) / 1000.0;
  lastTime = currentTime;

  // PID calculation
  integral += error * deltaTime;
  float derivative = (error - lastError) / deltaTime;
  output = (Kp * error) + (Ki * integral) + (Kd * derivative);
  lastError = error;

  // Servo output
  float servoAngle = 90 + output;
 servoAngle = constrain(servoAngle, 30, 180);

  myServo.write(servoAngle);

  // Log Data
  Serial.print("dist: ");
  Serial.print(input);  // Print distance value
  Serial.print(" cm | Servo: ");
  Serial.print(servoAngle);
  Serial.print(" | Error: ");
  Serial.println(error);

  delay(1);
}

float readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.0343) / 2;
  return distance;
}
