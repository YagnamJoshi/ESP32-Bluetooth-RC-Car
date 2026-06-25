#include "BluetoothSerial.h"

// Check if Bluetooth is properly enabled in the ESP32 Core
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;

// --- Pin Definitions ---
const int STBY = 33;
const int PWMA = 25;
const int AIN1 = 26;
const int AIN2 = 27;
const int PWMB = 14;
const int BIN1 = 12;
const int BIN2 = 13;

const int TRIG = 4;
const int ECHO = 2;

// --- ESP32 LEDC PWM Configuration ---
const int freq = 5000;
const int resolution = 8; // 0-255

// --- Logic & Timings ---
int currentSpeed = 60;    
const int MOVE_TIME = 250;
const int TURN_TIME = 300;
const int UTURN_TIME = 650;
const int OBSTACLE_LIMIT_CM = 30;

// --- Telemetry Timer Variables ---
unsigned long lastTelemetryTime = 0;
const int TELEMETRY_INTERVAL = 500; 

void setup() {
  Serial.begin(115200);
  
  SerialBT.begin("ESP32_RC_CAR");
  Serial.println("Bluetooth Started! Ready to pair as 'ESP32_RC_CAR'");

  pinMode(STBY, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  ledcAttach(PWMA, freq, resolution);
  ledcAttach(PWMB, freq, resolution);

  digitalWrite(STBY, HIGH);
  stopCar();
}

void loop() {
  // 1. Handle INCOMING driving commands
  if (SerialBT.available()) {
    char command = SerialBT.read();
    
    switch (command) {
      case 'F': moveForward(); break;
      case 'B': moveBackward(); break;
      case 'L': turnLeft(); break;
      case 'R': turnRight(); break;
      case 'S': stopCar(); break;
      case 'U': uTurn(); break;
        
      case '1': currentSpeed = 60; break;
      case '2': currentSpeed = 90; break;
      case '3': currentSpeed = 120; break;
      case '4': currentSpeed = 150; break;
      case '5': currentSpeed = 180; break;
      case '6': currentSpeed = 210; break;
      case '7': currentSpeed = 240; break;
      case '8': currentSpeed = 255; break;
    }
  }

  // 2. Handle OUTGOING sensor data (Every 500ms for the App)
  if (millis() - lastTelemetryTime >= TELEMETRY_INTERVAL) {
    lastTelemetryTime = millis();
    float distance = getDistance();
    if (SerialBT.hasClient()) {
      SerialBT.println(distance);
    }
  }
}

// --- Sensor Function ---
float getDistance() {
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  long duration = pulseIn(ECHO, HIGH, 30000);
  if (duration == 0) return 999.0;
  float distance = duration * 0.034 / 2.0;
  return distance;
}

// --- Motor Control Functions ---
void setMotors(int a1, int a2, int b1, int b2, int speedA, int speedB) {
  digitalWrite(AIN1, a1);
  digitalWrite(AIN2, a2);
  digitalWrite(BIN1, b1);
  digitalWrite(BIN2, b2);
  
  ledcWrite(PWMA, speedA);
  ledcWrite(PWMB, speedB);
}

void stopCar() {
  setMotors(LOW, LOW, LOW, LOW, 0, 0);
}

void moveForward() {
  float distance = getDistance();
  // ONLY Forward checks the sensor to prevent crashes
  if (distance >= OBSTACLE_LIMIT_CM) {
    setMotors(HIGH, LOW, HIGH, LOW, currentSpeed, currentSpeed);
    delay(MOVE_TIME);
    stopCar();
  } else {
    stopCar(); // Blocked by wall
  }
}

void moveBackward() {
  // Free movement (No sensor check)
  setMotors(LOW, HIGH, LOW, HIGH, currentSpeed, currentSpeed);
  delay(MOVE_TIME);
  stopCar();
}

void turnLeft() {
  // Free movement (No sensor check, locked to speed 60)
  setMotors(HIGH, LOW, LOW, HIGH, 60, 60);
  delay(TURN_TIME);
  stopCar();
}

void turnRight() {
  // Free movement (No sensor check, locked to speed 60)
  setMotors(LOW, HIGH, HIGH, LOW, 60, 60);
  delay(TURN_TIME);
  stopCar();
}

void uTurn() {
  // Free movement (No sensor check, locked to speed 60)
  setMotors(HIGH, LOW, LOW, HIGH, 60, 60); 
  delay(UTURN_TIME);
  stopCar();
}
