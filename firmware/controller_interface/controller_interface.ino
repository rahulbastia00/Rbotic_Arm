#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Create the driver object
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// CALIBRATION (Important!)
// Standard servos are usually SERVOMIN 150 and SERVOMAX 600
#define SERVOMIN  150 
#define SERVOMAX  600 

// Channels on the PCA9685 Board
int s_base = 0;
int s_shoulder = 1;
int s_elbow = 2;
int s_wrist = 3;
int s_gripper = 4;

String inputString = "";
bool stringComplete = false;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setPWMFreq(60);  // Analog servos run at ~60 Hz updates
  delay(10);
}

// Helper to convert Angle (0-180) to Pulse (150-600)
int angleToPulse(int ang) {
   int pulse = map(ang, 0, 180, SERVOMIN, SERVOMAX);
   return pulse;
}

void loop() {
  if (stringComplete) {
    // 1. Parse the incoming string "90,45,120,90,0"
    int angles[5];
    int count = 0;
    int strIndex = 0;
    
    for (int i = 0; i < inputString.length(); i++) {
      if (inputString.charAt(i) == ',') {
        angles[count] = inputString.substring(strIndex, i).toInt();
        strIndex = i + 1;
        count++;
      }
    }
    // Get the last number (after last comma)
    angles[count] = inputString.substring(strIndex).toInt();

    // 2. Move Servos
    // Constrain ensures we don't break the motor
    pwm.setPWM(s_base, 0, angleToPulse(constrain(angles[0], 0, 180)));
    pwm.setPWM(s_shoulder, 0, angleToPulse(constrain(angles[1], 0, 180)));
    pwm.setPWM(s_elbow, 0, angleToPulse(constrain(angles[2], 0, 180)));
    pwm.setPWM(s_wrist, 0, angleToPulse(constrain(angles[3], 0, 180)));
    pwm.setPWM(s_gripper, 0, angleToPulse(constrain(angles[4], 0, 180)));

    // Reset
    inputString = "";
    stringComplete = false;
  }
}

// Read Serial data automatically
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();
    if (inChar == '\n') {
      stringComplete = true;
    } else {
      inputString += inChar;
    }
  }
}