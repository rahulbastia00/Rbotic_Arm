#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

#define PWM_FREQ 50  // 50Hz for standard servos

// Servo channel assignments
#define BASE_CH     0
#define SHOULDER_CH 1
#define ELBOW_CH    2
#define WRIST_CH    3
#define GRIPPER_CH  4  // Gripper servo

// Pulse width limits
#define MIN_US 500
#define MAX_US 2500

float usPerTick = 20000.0 / 4096.0; // ≈4.88 µs/tick

// Convert angle to PWM ticks
uint16_t angleToTicks(int angle) {
  if (angle < 0) angle = 0;
  if (angle > 180) angle = 180;
  float pulse = MIN_US + (angle / 180.0) * (MAX_US - MIN_US);
  return (uint16_t)(pulse / usPerTick);
}

void moveServo(uint8_t ch, int angle) {
  pwm.setPWM(ch, 0, angleToTicks(angle));
}

// Smooth movement including gripper
void moveSmooth(int baseA, int shoulderA, int elbowA, int wristA, int gripperA,
                int baseB, int shoulderB, int elbowB, int wristB, int gripperB, int stepDelay) {
  for (int step = 0; step <= 100; step++) {
    int basePos     = map(step, 0, 100, baseA, baseB);
    int shoulderPos = map(step, 0, 100, shoulderA, shoulderB);
    int elbowPos    = map(step, 0, 100, elbowA, elbowB);
    int wristPos    = map(step, 0, 100, wristA, wristB);
    int gripperPos  = map(step, 0, 100, gripperA, gripperB);

    moveServo(BASE_CH, basePos);
    moveServo(SHOULDER_CH, shoulderPos);
    moveServo(ELBOW_CH, elbowPos);
    moveServo(WRIST_CH, wristPos);
    moveServo(GRIPPER_CH, gripperPos);

    delay(stepDelay);
  }
}

void setup() {
  Serial.begin(115200);
  pwm.begin();
  pwm.setPWMFreq(PWM_FREQ);

  // Initial neutral position
  moveServo(BASE_CH, 0);
  moveServo(SHOULDER_CH, 40);
  moveServo(ELBOW_CH, 60);
  moveServo(WRIST_CH, 90);
  moveServo(GRIPPER_CH, 0);  // Gripper fully open initially
  delay(1000);
  Serial.println("Robotic Arm Ready.");
}

void loop() {
  // Pickup and place positions
  int baseA = 0;
  int shoulderA = 60;
  int elbowA = 60;
  int wristA = 135;
  int gripperOpen = 0;
  int gripperClosed = 180;

  int baseB = 230;
  int shoulderB = 60;
  int elbowB = 30;
  int wristB = 135;

  // ----- Move to Pickup Position -----
  Serial.println("Moving to Pickup Position...");
  moveSmooth(baseA, shoulderA, elbowA, 90, gripperOpen,  // start with wrist neutral, gripper open
             baseA, shoulderA, elbowA, wristA, gripperClosed, 50); // gripper closes while moving
  delay(1000); // pause at pickup

  // ----- Shoulder rise after pickup -----
  Serial.println("Raising Shoulder...");
  moveSmooth(baseA, shoulderA, elbowA, wristA, gripperClosed,
             baseA, shoulderB, elbowA, wristA, gripperClosed, 30); // shoulder rises, gripper stays closed
  delay(500);

  // ----- Move to Place Position -----
  Serial.println("Moving to Place Position...");
  moveSmooth(baseA, shoulderB, elbowA, 90, gripperClosed,  // start from wrist neutral
             baseB, shoulderB, elbowB, wristB, gripperOpen, 30); // gripper opens while placing
  delay(500);

  // Return wrist to neutral after placing
  moveServo(WRIST_CH, 90);

  // ----- Return to Pickup Position -----
  Serial.println("Returning to Pickup Position...");
  moveSmooth(baseB, shoulderB, elbowB, 90, gripperOpen,
             baseA, shoulderA, elbowA, 90, gripperOpen, 30);
  delay(2000);
}