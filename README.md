# Vertex-X1: Quadruped Cybernetic Robot Core

A comprehensive engineering blueprint, firmware codebase, and modular control documentation for the **Vertex-X1** quadruped robot. This file details the structural mechanical answers required for the task guidelines, followed by advanced software-hardware integration.

---

## 🛠️ [01] Core Mechanical Design & Task Evaluation

This section directly answers the core physical and mechanical parameters required to achieve stable standing and locomotion:

### 1. Body and Chassis Shape (شكل الجسم والهيكل)
*   **Aesthetic & Structure:** Designed using a low-poly faceted cyberpunk enclosure to distribute structural stress evenly across planes while providing a shell for internal hardware.
*   **Dimensions:** Overall footprint is 163mm Length × 85mm Width × 45mm Height.
*   **Head Subsystem:** Features flush-mounted dual eye sockets designed to anchor the screens internally, completely hiding the circuit boards so only the active visual matrices are visible. The lower jaw line includes a geometric speaker cutout grid.
*   **Chest Subsystem:** The front chest plate houses the primary HC-SR04 ultrasonic sonar, securing it facing forward to measure clear boundaries without head movement interference.

### 2. Leg Design (تصميم الأرجل)
*   **Kinematics:** Implements a dual-segment articulated leg architecture split into an upper thigh link and a lower leg link. Both links rotate along parallel axes to translate circular motor sweeps into linear strides.
*   **Grip Enhancement:** The lower leg tips are embedded with custom high-friction **Rubber Traction Pads** to eliminate physical slippage during active step phases.

### 3. Number of Joints and Degrees of Freedom (عدد المفاصل ودرجات الحرية)
*   The entire mechanical platform operates on **9 Degrees of Freedom (9 DoF)**:
    *   **Locomotion Joints (8 DoF):** Consists of 4 legs, with each leg utilizing 2 active joints (1 Hip joint for forward/backward stride clearance, and 1 Knee joint for vertical lifting and leveling).
    *   **Behavioral Joints (1 DoF):** Consists of 1 independent joint located at the rear plate for active tail movement.

### 4. Motor Selection (اختيار المحركات)
*   **Leg Actuators (8x Motors):** Equipped with **MG996R High-Torque servos**. These motors utilize fully metal internal gears, running optimally at 6.0V to supply up to 11 kg·cm of holding torque to prevent joint collapsing under weight load.
*   **Tail Actuator (1x Motor):** Driven by a single **SG90 Micro Servo**, chosen for its minimal weight footprint since behavioral movements carry near-zero physical load.

### 5. Initial Joint Torque Calculation (حساب عزم مبدئي لأحد المفاصل)
To ensure the selected motors can support the robot during maximum load phases (such as balancing on only 2 legs during a high-speed stride transition):
*   Total Estimated Robot Mass ($M$) = $2.5\text{ kg}$
*   Gravitational Force ($F$):
$$F = M \times g = 2.5\text{ kg} \times 9.81\text{ m/s}^2 = 24.525\text{ N}$$
*   During a dynamic step cycle where only 2 legs maintain ground contact, each active leg carries exactly half the total gravitational force:
$$F_{\text{leg}} = \frac{24.525\text{ N}}{2} = 12.2625\text{ N}$$
*   Given a physical upper thigh lever arm length ($d$) of 10 cm ($0.1\text{ m}$), the torque required at the primary hip joint axis ($\tau$) is computed as:
$$\tau = F_{\text{leg}} \times d = 12.2625\text{ N} \times 0.1\text{ m} = 1.22625\text{ Nm}$$
*   Converting Newton-meters to standard hobby servo torque units (kg·cm):
$$1.22625\text{ Nm} \approx 12.5\text{ kg}\cdot\text{cm}$$
*   **Validation & Safety Factor:** The raw stall torque of an MG996R at 6V is between 11-13 kg·cm. To establish a secure safety margin and prevent overheating, the hip joints integrate physical **1:2 Ratio Spur Gears**, which doubles the torque delivery while keeping the motors within safe operational thresholds.

### 6. Stability and Center of Gravity (الثبات ومركز الجاذبية)
*   **Physical CoG:** The absolute Center of Gravity is physically aligned by mounting the heavy 18650 lithium battery array at the lowest point of the central abdomen floor.
*   **Dynamic Electronic Stabilization:** An **MPU6050 6-DoF Inertial Measurement Unit (IMU)** is fixed at the exact geometric Center of Mass. The firmware continually processes internal Pitch and Roll orientation streams. If external factors create tilt deviations exceeding $\pm 5^{\circ}$, closed-loop routines inject corrective angle modifications to the leg servos to stabilize the CoG.

### 7. Proposed Gait Method (طريقة المشي المقترحة)
*   The locomotion engine relies on a strictly timed **Static Wave Gait**.
*   The execution sequence ensures that only 1 leg breaks contact with the ground at any given interval, while the other 3 legs remain locked in place. This forms a continuous structural triangle wrapped directly around the center of mass, allowing the robot to move forward while maintaining static equilibrium at every microstep.

### 8. Expected Mechanical Issues (المشاكل الميكانيكية المتوقعة)
*   **Gear Backlash:** Positional slack in standard servo gears can cause micro-wobbles. This is mitigated through software calibration offsets embedded in the firmware.
*   **Voltage Drops (Voltage Sag):** Sudden power draw from 8 high-torque motors can starve the ESP32 logic controller, causing brownouts or resets. This is completely resolved by splitting power routing using an **LM2596 Buck Converter** to feed the motors 6V, and wiring a large **1000µF Capacitor** in parallel across the main power rail to smooth out current spikes.

---

## 👁️ [02] Advanced Embedded & Sensory Subsystems

This section breaks down the smart computing and localized sensory behaviors added to maximize the robot's functionality:

### 1. Dual OLED Screen Synchronization Architecture
*   **Hardware Setup:** The visual framework utilizes two independent 0.96" SSD1306 OLED displays running on a single I2C data bus. To allow separate data streaming, the left screen is jumper-configured to hardware address `0x3C`, while the right screen is configured to `0x3D`.
*   **Synchronization Logic:** To prevent latency mismatches (one eye moving slower than the other), animations are controlled by a single memory state matrix in `eye_synchronizer.h`. Framebuffers for both displays are computed sequentially in the cache before a rapid back-to-back hardware `.display()` signal is sent, forcing both eyes to update graphics, blinks, and gaze shifts at the exact same millisecond.

### 2. Audio Processing & Speech Subsystem
*   **Offline Voice Recognition:** An isolated voice recognition card computes and decodes sound waves locally using a built-in neural firmware array. It accepts distinct verbal commands (e.g., "SIT", "STAND") and switches digital pin signals to notify the main ESP32 without relying on internet clouds.
*   **Voice Output Matrix:** When a system state updates, the ESP32 passes audio frequencies to a **PAM8403 Audio Amplifier** to clean line noise and amplify volume, driving a mini-speaker inside the mouth to play responsive sound cues.

### 3. Full Internal Component Inventory
*   **ESP32 Dev Board:** Main microprocessing core managing general tasks.
*   **MPU6050 IMU:** Internal sensor for calculating balance vectors.
*   **8x MG996R Servos:** Metal gear core joints for structural walking.
*   **1x SG90 Servo:** Lightweight actuator for behavioral tail expressions.
*   **Dual SSD1306 OLEDs:** Visual matrix displaying synchronized expressions.
*   **HC-SR04 Sonar:** Proximity sensor mapped to compute forward obstacle boundaries.
*   **PAM8403 Amplifier & Speaker:** Audio framework for frequency playback.
*   **Offline Voice Card:** Sound processing module for command parsing.
*   **2x 18650 Li-ion Cells:** High-drain chemical cells supplying power.
*   **LM2596 Buck Module:** DC-to-DC converter stepping battery voltage down to 6V.
*   **1000µF Capacitor:** Energy buffer stabilizing the power lines.

---

## 💻 [03] Repository Code Matrix

The codebase is split into four clean files to separate hardware logic from main loop processing:
1.  **`main_firmware.ino`:** Main script running the primary execution loop, reading the MPU6050, and sorting command states.
2.  **`display_audio_engine.h`:** Controls voice recognition input parsing and tone frequencies for the speaker.
3.  **`eye_synchronizer.h`:** Handles the dual-address screen buffer layout to synchronize eye rendering.
4.  **`joint_calibration.py`:** Desktop Python script used to connect via USB Serial to calibrate mechanical angles.

---

## 🔌 [04] Unified Hardware Pin Assignments

*   **System Bus (I2C):** GPIO 21 (SDA) / GPIO 22 (SCL) -> Shares Left OLED (`0x3C`), Right OLED (`0x3D`), and MPU6050 IMU.
*   **Actuator Array (PWM):** GPIOs (12, 13, 14, 27, 26, 25, 33, 32) -> Deployed to the 8x MG996R Leg Servos.
*   **Tail System (PWM):** GPIO 15 -> 1x SG90 Micro Servo.
*   **Audio Output:** GPIO 4 -> Connects directly to the PAM8403 Amplifier input.
