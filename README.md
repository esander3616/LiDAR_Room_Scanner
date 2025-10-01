High-Speed 3D LiDAR Room Scanner
This project is a 3D room scanner built with an Arduino, a TFMini-S LiDAR sensor, a servo, and a stepper motor. It rapidly captures distance and angle data to generate a 3D point cloud model of a room, which is visualized in real-time on a computer using Python.

The key feature of this project is its high-speed scanning technique. By soldering a wire to the servo's internal potentiometer, we can sample its precise angle at over 500Hz while it moves at maximum speed. This allows for the collection of over 28,000 data points in a fraction of the time required by traditional step-and-sample methods, resulting in a scan that is over four times faster than comparable projects.

Final Result Preview
<img width="843" height="736" alt="kitchen_scan" src="https://github.com/user-attachments/assets/cceeaa2b-2d00-4dad-a0fc-d7e5550cc0bd" />


Hardware Components
Component	Description
Arduino	Any Arduino-compatible board (e.g., Uno, Nano).
TFMini-S LiDAR	A single-point ranging LiDAR sensor.
MG995 Servo	Modified to read its internal potentiometer for high-speed angle feedback.
28BYJ-48 Stepper Motor	Used for 180-degree horizontal rotation (theta axis).
Potentiometer	External pot for servo calibration (or directly use the internal one).
3D Printed Parts	Custom-designed housing for stability and clean aesthetics.

Export to Sheets
Software & Libraries
Arduino (arduino_scanner/scanner.ino)
The firmware is written in C++ for the Arduino platform. It requires the following libraries:

Servo.h: For controlling the servo motor.

AccelStepper.h: For controlling the stepper motor's movement.

TFMPlus.h: For communicating with the TFMini-S LiDAR sensor.

Python (python_visualizer/visualizer.py)
The real-time visualization is a Python script that reads serial data from the Arduino and plots it. It requires the following libraries:

pyserial: To read data from the serial port.

numpy: For efficient numerical calculations (e.g., coordinate conversion).

matplotlib: For creating the 3D scatter plot.

To install the Python dependencies, run:

Bash

pip install pyserial numpy matplotlib
Setup and Installation
1. Hardware Assembly & Wiring
3D Print Parts: Print the necessary housing and mounts to ensure the scanner is mechanically stable. Wobbling can introduce errors in the final 3D model.

Modify the Servo: This is the most critical step. Carefully open the servo casing and solder a wire to the middle pin of the internal potentiometer. This wire will output an analog voltage corresponding to the servo's angle, which should be connected to an analog pin on the Arduino (e.g., A0).

Wire the Components:

TFMini-S LiDAR: Connect to Serial2 on the Arduino.

Servo: Connect the control pin to pin 6.

Stepper Motor: Connect IN1, IN2, IN3, IN4 to pins 8, 9, 10, 11.

Servo Potentiometer Wire: Connect the soldered wire to pin A0.

2. Software Setup
Configure Arduino Code: Open scanner.ino in the Arduino IDE. Install the required libraries via the Library Manager.

Configure Python Script: Open visualizer.py. Change the PORT variable to match the serial port of your Arduino.

Windows: It will look like 'COM3' or 'COM4'.

macOS/Linux: It will look like '/dev/cu.usbmodem...' or '/dev/ttyACM0'.

How to Run
Upload the Code: Connect the Arduino to your computer and upload the scanner.ino sketch.

Open Serial Monitor (Optional): You can open the Arduino Serial Monitor at 115200 baud to see the raw r,phi,theta data being streamed.

Run the Visualizer: Execute the Python script from your terminal.

Bash

python visualizer.py
Scan: A 3D plot window will appear. The scanner will begin its 180-degree sweep, and points will be plotted in real-time.

Stop and Finalize: Once the scan is complete, press Ctrl+C in the terminal where the Python script is running. This will stop the live plotting and generate a final, high-quality plot of the complete point cloud.
