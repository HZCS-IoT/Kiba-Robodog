# // VERTEX-X1: Quadruped Cybernetic Robot Core //

A comprehensive firmware, mechanical documentation, and control system repository for the **Vertex-X1** quadruped robot. Built with a low-poly faceted chassis architecture and clean modular code for real-time dynamic stabilization.

---

## 🛠️ [01] Engineering Specifications & Task Report

### 1. Chassis & Body Shape
*   **Mechanical Design:** Low-poly / faceted cyberpunk aesthetic with compact dimensions (163mm Length × 85mm Width × 45mm Height).
*   **Structural Layout Updates:** 
    *   **Head Assembly:** Modified faceplate featuring square, flush display sockets to completely conceal the display PCBs, exposing only the active white OLED matrices. Includes a dedicated integrated mouth grill for voice output.
    *   **Chest Plate:** Relocated the HC-SR04 Ultrasonic Sonar sensor to the front center of the chest to optimize spatial awareness and isolate the head assembly for audio/visual components.

### 2. Leg Design
*   Dual-segment articulated leg architecture consisting of an upper thigh link and a lower leg link directly actuated by servo horns.
*   Equipped with bottom **Rubber Traction Pads** to maximize friction coefficients and prevent slippage during operational state transitions.

### 3. Joint Matrix & Degrees of Freedom (9 DoF)
*   The mechanical system operates on **9 Degrees of Freedom (9 DoF)**:
    *   **Locomotion Joints:** 8 DoF across four legs (2 actuated joints per leg: Hip and Knee).
    *   **Behavioral Joints:** 1 DoF located at the rear for dynamic tail actuation.

### 4. Actuator Selection
*   **Primary Links (8x Joints):** **MG996R High-Torque** metal gear servos running at 6.0V, delivering up to 11 kg·cm of torque to handle high structural stress during dynamic load-bearing phases.
*   **Secondary Links (1x Joint):** **SG90 Micro Servo** utilized for lightweight, low-load behavioral tail actuation.

### 5. Initial Joint Torque Calculation
To determine the baseline torque requirement for a single hip joint under worst-case static load distribution (standing or transitioning on 2 legs during gate cycles):
*   Total estimated robot mass ($M$): $\approx 2.5\text{ kg}$. Total gravitational force ($F$):
$$F = M \times g = 2.5\text{ kg} \times 9.81\text{ m/s}^2 \approx 24.52\text{ N}$$
*   When balancing on 2 legs during a step transition, each active leg bears half the total load:
$$F_{\text{leg}} = \frac{24.52\text{ N}}{2} = 12.26\text{ N}$$
*   Given an upper thigh lever arm length ($d$) of 10 cm ($0.1\text{ m}$), the required torque ($\tau$) at the hip joint axis is:
$$\tau = F_{\text{leg}} \times d = 12.26\text{ N} \times 0.1\text{ m} = 1.226\text{ Nm}$$
*   Converting Newton-meters to kilograms-centimeter:
$$1.226\text{ Nm} \approx 12.5\text{ kg}\cdot\text{cm}$$
*   **Engineering Validation:** The MG996R servo provides a stall torque of 11–13 kg·cm. By integrating a mechanical **1:2 Ratio Spur Gear system** at the hip joints, the effective torque output doubles while reducing structural stress on the motor shaft, establishing an optimal mechanical safety margin.

### 6. Stability & Center of Gravity (CoG)
*   **Sensor Alignment:** Integrated an **MPU6050 6-DoF Inertial Measurement Unit (IMU)** at the exact geometric Center of Mass (CoM), positioned directly above the battery bay inside the lower chassis hull.
*   **Closed-Loop Compensation:** The core firmware samples real-time Pitch and Roll deviation vectors. If the inclination exceeds $\pm 5^{\circ}$, it applies instantaneous scaling offsets to the leg servos to dynamically stabilize the CoG.

### 7. Proposed Gait Method
*   The system executes a strictly calibrated **Static Wave Gait**.
*   Only one leg undergoes a swing phase at any given interval, while the remaining three legs maintain solid contact with the ground. This forms a continuous structural support triangle wrapped securely around the shifting Center of Gravity, preventing tipping during locomotion.

### 8. Expected Mechanical Constraints & Mitigations
*   **Gear Backlash:** Minor positional errors due to 3D printed/molded tolerances. Mitigated via software-level angular calibration constants (Offset values).
*   **Voltage Sag:** High instantaneous current draw from 8 metal gear servos can trigger ESP32 brownout loops. Mitigated using an **LM2596 Buck Converter** to drop line voltages to a stable 6V, supplemented by a **1000µF capacitor** wired in parallel across the power rail to absorb transient current spikes.

---

## 💻 [02] Repository Code Architecture

The implementation environment is decoupled into a modular, clean-code file structure:

1.  **`main_firmware.ino`:** Core C++ sketch deployed to the ESP32. Manages hardware mapping, loops the MPU6050 stabilization loop, and decodes runtime serial configurations.
2.  **`display_audio_engine.h`:** Dedicated sub-layer controlling the dual SSD1306 OLED square eye arrays via I2C, alongside generating system tone sequences via the PAM8403 amplifier.
3.  **`joint_calibration.py`:** A Python 3 desktop utility that links to the robot via a USB Serial interface, enabling precise zero-angle joint calibration before deploying autonomy scripts.

---

## 🔌 [03] Hardware Pin Configuration Mapping

*   **ESP32 I2C Interfacing:** GPIO 21 (SDA) / GPIO 22 (SCL) -> Shared parallel bus for OLED displays and MPU6050 IMU.
*   **Servo PWM Signals:** GPIO pins (12, 13, 14, 27, 26, 25, 33, 32) allocated for the 8 primary MG996R leg actuators.
*   **Audio Line Out:** GPIO 4 -> Interfaces with the PAM8403 amplifier to feed the mini speaker subsystem.
