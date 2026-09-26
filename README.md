# 5-DOF Robotic Arm

A personal robotics project combining custom mechanical design, 3D printing, embedded control, and arm kinematics. The goal is a compact arm that can position a gripper and eventually use camera input to help grasp objects.

> **Status:** Active prototype. The mechanical assembly and basic servo testing are underway. The MATLAB control interface, full ESP32 integration, and camera-guided grasping are works in progress.

## Design

The arm has five planned positioning axes: turret yaw, shoulder pitch, elbow pitch, elbow roll, and wrist pitch. A gripper adds a separate open/close action. The structure uses custom CAD parts, 3D-printed joints, and carbon-fiber linkages. The current design combines hobby servos with custom gear and belt transmissions; motor choices and joint packaging are still being refined.

| Area | Approach |
| --- | --- |
| Mechanical design | SolidWorks CAD, printed components, carbon-fiber links |
| Actuation | Servo-driven joints and custom transmissions; additional DC motor actuation under development |
| Controller | ESP32 with Arduino firmware |
| Desktop interface | MATLAB / App Designer for joint commands and arm visualization |
| Future perception | Camera input for assisted object detection and grasping |

## Current work

- Building and adjusting the joint assemblies, including the elbow-roll bevel gears and belt tensioning.
- Testing shoulder and elbow servos from the ESP32.
- Developing a MATLAB visualization and slider-based joint controls.
- Defining a serial command format to send joint angles from MATLAB to the ESP32.
- Evaluating shoulder and elbow loads as the wrist and gripper add mass. The unpowered joints currently drop under gravity, so load capacity and counterbalancing need testing before claiming a usable payload.

## CAD files (SOLIDWORKS)

[Google Drive Folder](https://drive.google.com/drive/folders/1rUqKTxcmgNEenckUri99Z5Um0AfbZbVK?usp=sharing)

## Control concept

The intended path is **MATLAB sliders → USB serial → ESP32 → joint actuators**. The MATLAB view represents commanded angles; without joint sensors, it cannot confirm the actual position of the physical arm.

A proposed newline-delimited JSON command is:

```json
{"shoulder_pitch":100,"elbow_pitch":90,"wrist_pitch":90,"elbow_roll":90,"turret":90,"end_effector":90}
```

The firmware should validate each field and clamp commands to tested mechanical limits before moving a joint. This format describes the interface under development, not a verified end-to-end feature.

## Next steps

1. Establish safe angle ranges and servo zero positions for each assembled joint.
2. Finish the ESP32 command parser and test one joint at a time.
3. Connect the MATLAB interface to the hardware and compare commanded versus observed motion.
4. Measure joint loads with the gripper installed and refine the counterbalance and transmissions.
5. Add camera display and explore vision-assisted grasping.

## Repository notes

The CAD, firmware, and MATLAB files are being organized as the prototype evolves. Check the project folders for the latest implementation; the features described as future work above may not yet have code in this repository.

Built by James Qin.
