# High-Speed 3D LiDAR Room Scanner

This repository contains the code for a 3D room scanner I built using an Arduino, a LiDAR sensor, and a couple of motors. The system rapidly captures distance and angle data to generate a 3D point cloud of a room, which is then visualized in real-time with a Python script.

The most interesting feature of this project is its high-speed scanning technique. Standard methods of stopping the scanner to take a reading at each point are quite slow. To solve this, I modified the servo to read its precise angle directly from its internal potentiometer while it moves at full speed. This approach allows the system to sample at over 500Hz, collecting over 28,000 data points and creating a detailed scan more than four times faster than comparable projects.

### Final Scan Result
<img width="843" height="736" alt="kitchen_scan" src="https://github.com/user-attachments/assets/cceeaa2b-2d00-4dad-a0fc-d7e5550cc0bd" />


## Software and Libraries

The project is split into two main parts: the Arduino firmware that runs the scanner, and the Python script that visualizes the data.

### Arduino (`arduino_scanner/scanner.ino`)
The firmware is written in C++ for the Arduino platform. You'll need to install the following libraries from the Arduino Library Manager:
* `Servo.h`: For controlling the servo motor.
* `AccelStepper.h`: For controlling the stepper motor's movement.
* `TFMPlus.h`: For communicating with the TFMini-S LiDAR sensor.

### Python (`python_visualizer/visualizer.py`)
The real-time visualization is a Python script that reads the serial data from the Arduino and plots it. You'll need to install a few Python libraries to get it running.

You can install the dependencies by running this command in your terminal:
```bash 
pip install pyserial numpy matplotlib
```

## Hardware Assembly and Wiring
3D Print Parts: First, print the necessary housing and mounts. A stable mechanical assembly is key, as any wobbling can introduce errors in the final 3D model.

Modify the Servo: You'll need to carefully open the servo casing and solder a wire to the middle pin of the internal potentiometer. This wire will output an analog voltage corresponding to the servo's angle, allowing the server to sweep quickly while also getting accurate and fast readings.

Wire the Components:

TFMini-S LiDAR: Connect this to Serial2 on the Arduino.

Servo: Connect the main control pin to pin 6.

Stepper Motor: Connect IN1, IN2, IN3, IN4 to pins 8, 9, 10, 11.

Servo Potentiometer Wire: Connect the new wire you soldered to an analog pin, like A0.

## How to Run:
Upload the Code: Connect the Arduino to your computer and upload the scanner.ino sketch.

Open Serial Monitor (Optional): You can open the Arduino Serial Monitor at 115200 baud to see the raw r,phi,theta data being streamed.

Run the Visualizer: Execute the Python script from your terminal.

``` Bash
python visualizer.py
```
Scan: A 3D plot window will appear. The scanner will begin its 180-degree sweep, and points will be plotted in real-time.

Stop and Finalize: Once the scan is complete, press Ctrl+C in the terminal where the Python script is running. This will stop the live plotting and generate a final, high-quality plot of the complete point cloud.
