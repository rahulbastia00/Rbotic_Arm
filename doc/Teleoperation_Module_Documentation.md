# Teleoperation & Data Collection Interface for 5-DOF Robotic Arm

📌 **Project Overview**  
This module serves as the Data Collection & Teleoperation subsystem for the larger Autonomous Electronic Component Sorting project.  
Before applying Imitation Learning, we must first "teach" the robot how to pick and place components (IoT sensors, resistors, modules) from a cluttered, dynamic environment.  
This interface uses computer vision and hand gestures to control the robotic arm in real-time, allowing a human operator to perform tasks while the system logs state-action pairs for future model training.

---

## 🎯 Key Objectives
- **Intuitive Control:** Map human hand coordinates $(x, y, z)$ to robot joint angles $(\theta_{base}, \theta_{shoulder}, \theta_{elbow})$ in real-time.
- **Jitter Reduction:** Implement signal processing to ensure smooth servo movement, filtering out sensor noise.
- **Data Generation:** Create a robust pipeline to record successful pick-and-place trajectories.

---

## 🛠 Hardware Architecture
| Component | Description |
|----------|-------------|
| Manipulator | Custom DIY 5-DOF Robotic Arm (Servos: MG996R/SG90) |
| Controller | Arduino Uno (Serial communication) |
| Driver | PCA9685 16-Channel 12-bit PWM Servo Driver |
| Sensors | Standard USB Webcam (for hand tracking) |
| Power | 5V 5A External Power Supply (for servos) |

---

## 🏗 Software Architecture & "The Delta"
This system is not just a direct map of coordinates; it employs a multi-stage pipeline to convert noisy visual data into precise mechanical motion.

### 1. Perception Layer (MediaPipe & OpenCV)
We utilize Google MediaPipe Hands, a highly optimized graph-based framework.

**Why MediaPipe?**  
Unlike traditional color segmentation, MediaPipe infers 21 3D hand landmarks using a palm detection model followed by a hand landmark regression model. This provides robustness against lighting changes and background clutter.

**Coordinate Extraction:**  
We extract the Wrist (Landmark 0) for the arm's target position $(x, y, z)$ and calculate the Euclidean distance between the Index Tip (8) and Thumb Tip (4) to trigger the gripper.

---

### 2. Signal Processing (One Euro Filter)
Raw optical tracking data is inherently noisy. Direct mapping causes the servos to jitter violently.

**Solution:** We implemented the One Euro Filter (Casiez et al., 2012).  

**How it works:** It is a first-order low-pass filter with an adaptive cutoff frequency.

| Speed Condition | Behavior |
|----------------|-----------|
| Low speeds | Cutoff frequency decreases to heavily filter noise (reducing jitter) |
| High speeds | Cutoff frequency increases to reduce lag |

**Result:** Smooth precision when holding still, and responsive tracking when moving fast.

---

### 3. Motion Planning (Geometric Inverse Kinematics)
The core mathematical challenge is Inverse Kinematics (IK): converting a desired spatial position $(x, y, z)$ into servo angles. We use a Geometric Approach tailored for a 3-DOF planar chain (Base + 2-link Arm).

---

## 📐 Mathematical Formulation

### A. Base Rotation ($\theta_1$)
The base simply points toward the target on the XY plane.

$$\theta_{base} = \arctan2(y, x)$$

### B. Planar Projection
$$
r = \sqrt{x^2 + y^2} \quad \text{(Radial distance from base)}
$$  
$$c = \sqrt{r^2 + z^2} \quad \text{(Distance from shoulder pivot to target)}$$

### C. Joint Angles via Law of Cosines
Given link lengths $L_1$ (Shoulder-Elbow) and $L_2$ (Elbow-Wrist), we solve the triangle formed by the arm:

**Calculate $\alpha$ (Elevation of target):**  
$$\alpha = \arctan2(z, r)$$

**Calculate Internal Angles ($\angle A$, $\angle B$):**  
$$
\cos(A) = \frac{L_1^2 + c^2 - L_2^2}{2 \cdot L_1 \cdot c}
$$  
$$\cos(B) = \frac{L_1^2 + L_2^2 - c^2}{2 \cdot L_1 \cdot L_2}$$

**Final Servo Angles:**  
$$
\theta_{shoulder} = \alpha + \arccos(\cos(A))
$$  
$$\theta_{elbow} = \arccos(\cos(B))$$

(Note: These radians are converted to degrees and mapped to PWM pulses for the PCA9685).

---

## 💻 Installation & Setup

### Prerequisites
- Python 3.8+
- Arduino IDE

---

### 1. Firmware Upload
Open `firmware/controller_interface/controller_interface.ino` in Arduino IDE.  
Install the Adafruit PWMServoDriver library.  
Calibrate SERVOMIN and SERVOMAX in the code to match your specific motors.  
Upload to Arduino Uno.

---

### 2. Python Environment
```bash
# Create virtual environment
python -m venv venv
source venv/bin/activate  # or venv\Scripts\activate on Windows

# Install dependencies
pip install opencv-python mediapipe pyserial numpy
```
---

### 3. Running the Teleop
```bash
Connect the Arduino via USB.
Check the COM port (e.g., COM3 or /dev/ttyUSB0) and update it in the notebook/script.
Run the controller:
python src/robot_controller.py

# OR run the Jupyter Notebook
jupyter notebook notebooks/teleop_proof_of_concept.ipynb

Press 'q' to exit the loop.

```
---

### 📊 Libraries Used

| Library      | Purpose                                                             |
| ------------ | ------------------------------------------------------------------- |
| OpenCV (cv2) | Capturing video feed, image flipping, and drawing UI elements.      |
| MediaPipe    | High-fidelity hand tracking and landmark extraction.                |
| NumPy        | Efficient matrix operations and vector math for coordinate scaling. |
| Math         | Trigonometric functions (atan2, acos, sqrt) for IK calculations.    |
| PySerial     | Asynchronous serial communication with the Arduino firmware.        |
