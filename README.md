## 🧠 Problem Statement

Modern electronics assembly and repair environments deal with large tables where many small components—IoT sensors, audio modules, transistors, resistors, jumper wires, etc.—are scattered or placed randomly.

A robot must identify each component type and place it into the correct bin, even when:

- Components are in random positions, overlapping or rotated
- Similar-looking components appear in wrong bins or wrong locations
- The arrangement changes dynamically over time
- New or unseen objects appear
- The robot needs to correct incorrect placements (example: if a sensor is in the wrong bucket)

Traditional static pick-and-place pipelines fail because they rely on predefined positions or simple object detection and do not adapt to real-world randomness.

---

## 🧠 Solution Approach

To solve the problem of sorting electronic components scattered randomly in a dynamic workspace, this project follows an **Imitation Learning–driven teleoperation approach**, where the robot first learns from human demonstrations and then performs autonomously.

The key idea is simple:
➡️ **Instead of manually programming every motion**, we **teach** the robot by **showing** examples of successful pick-and-place actions.

### How the Approach Works

### 1️⃣ Learning From Demonstration Instead of Predefined Rules
Traditional systems depend on fixed positions or static object detection, which breaks in real-world scenarios.  
Instead, we allow a human operator to **control the robotic arm directly** using a teleoperation interface.  
While doing so, we record:
- The **camera view** of scattered components
- The **joint movement data** of the robotic arm
- A **task instruction** (text command)

This forms a high-quality demonstration dataset.

### 2️⃣ Capturing Real Human Dexterity
Human movement naturally handles uncertainty—overlaps, occlusions, component rotation, clutter etc.  
By recording **real trajectories** of the arm while performing tasks, we capture strategies that are otherwise extremely difficult to hand-code.

### 3️⃣ Training an AI Model to Reproduce the Demonstrated Behaviors
The recorded demonstrations will be used to train an **ACT-based multi-modal model** that understands:
- What the workspace looks like (vision)
- What the user wants (language command)
- How to move (robot joint states)

Instead of predicting one step at a time, the model outputs **chunks of smooth actions**, resulting in fluid robotic motion.

### 4️⃣ Autonomous Execution With Vision + Language + Action
Once trained, given a command like **"Sort the ESP32 board"**, the system:
- Looks at the workspace image
- Identifies the target part
- Predicts the full movement sequence to grasp and place it
- Executes it on the real hardware

This enables the robot to adapt to **clutter**, **randomness**, **new positions**, and **misplaced items**, something classical pipelines cannot handle.

---

### ⭐ Why This Approach Works
✔ Mimics expert human behavior rather than rigid programming  
✔ Learns from real-world messy environments  
✔ Handles continuously changing layouts and new objects  
✔ Produces smooth actions ideal for robotic manipulation  

---

## 🛠️ Hardware Setup

You can view the detailed hardware wiring diagram and connection instructions here:

👉 **[Hardware Setup Document](/doc/hardware_&_connection_info.md)**

## Pahse 1 : 

You can view the detailed Teleoperation & Data Collection Interface for 5-DOF Robotic Arm here:

👉 **[Setup Document](/doc/Teleoperation_Module_Documentation.md)**


---

## 🧪 Software Setup & Execution

1. **Upload the controller code**
   - Open the file **[controller_interface](/firmware/controller_interface/controller_interface.ino)**
   - Upload the provided code to the Arduino board
   - Make sure all required libraries are installed in the Arduino IDE

2. **Run the Teleoperation Notebook**
   - Open the Jupyter notebook **[teleop_proof_of_concept](/notebooks/teleop_proof_of_concept.ipynb)**
   - Install all the libraries **[requirements.txt](requirements.txt)**
   - Run all the cells sequentially

3. **Important Note**
   - Before executing the Jupyter notebook, **close the Arduino IDE**
   - This ensures that the serial port is not already in use or reserved by the Arduino IDE

4. **Update Serial Port**
   - In the notebook, update the serial communication line:
     ```python
     ser = serial.Serial('COM7', 9600, timeout=1)
     ```
   - Replace `'COM7'` with your **actual Arduino port** (example: `COM3`, `COM5`, etc.)

---


