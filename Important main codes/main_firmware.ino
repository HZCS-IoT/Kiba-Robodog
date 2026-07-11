// ===================================================================
// VERTEX-X1 QUADRUPED MAIN FIRMWARE v1.0
// ===================================================================
#include <Wire.h>
#include <ESP32Servo.h>
#include <MPU6050.h>

// تعريف محركات الأرجل (8 محركات MG996R)
Servo hip_FL, knee_FL;  // الأمامية اليسرى
Servo hip_FR, knee_FR;  // الأمامية اليمنى
Servo hip_BL, knee_BL;  // الخلفية اليسرى
Servo hip_BR, knee_BR;  // الخلفية اليمنى
Servo tail_servo;       // محرك الذيل SG90

MPU6050 imu;

void setup() {
    Serial.begin(115200);
    Wire.begin();
    
    // ربط المحركات بالدبابيس الرقمية (GPIOs) لـ ESP32
    hip_FL.attach(12); knee_FL.attach(13);
    hip_FR.attach(14); knee_FR.attach(27);
    hip_BL.attach(26); knee_BL.attach(25);
    hip_BR.attach(33); knee_BR.attach(32);
    tail_servo.attach(15);

    // تهيئة مستشعر التوازن
    imu.initialize();
    if (!imu.testConnection()) {
        Serial.println("> IMU Connection Failed.");
    }
}

void loop() {
    // 1. تشغيل حلقة التوازن الديناميكي تلقائياً
    autoAdjustBalance();
    
    // 2. الاستماع للأوامر القادمة عبر Serial (سواء وحدة الصوت أو لابتوب المعايرة)
    if (Serial.available() > 0) {
        String command = Serial.readStringUntil('\n');
        processIncomingCommand(command);
    }
    
    delay(20); // حماية المعالج وتثبيت تردد حلقة التحكم
}

// دالة تصحيح الميلان والمحافظة على مركز الجاذبية
void autoAdjustBalance() {
    int16_t ax, ay, az, gx, gy, gz;
    imu.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    
    float pitch = atan2(-ax, sqrt(ay * ay + az * az)) * 180 / 3.14159;
    
    // إذا تعدت زاوية الميلان 5 درجات، يتم تعديل مفاصل الأرجل لتعويض مركز الثقل
    if (abs(pitch) > 5.0) {
        int offset = pitch * 0.6;
        hip_FL.write(90 + offset);
        hip_FR.write(90 + offset);
        hip_BL.write(90 - offset);
        hip_BR.write(90 - offset);
    }
}

// معالجة الأوامر التشغيلية والمعايرة
void processIncomingCommand(String cmd) {
    cmd.trim();
    if (cmd == "SIT") {
        executeSitSequence();
    } else if (cmd.startsWith("CAL:")) {
        // هيكلة أمر المعايرة القادم من بايثون: CAL:SERVO_ID:ANGLE
        int firstColon = cmd.indexOf(':');
        int secondColon = cmd.indexOf(':', firstColon + 1);
        int servoId = cmd.substring(firstColon + 1, secondColon).toInt();
        int angle = cmd.substring(secondColon + 1).toInt();
        calibrateJoint(servoId, angle);
    }
}

void executeSitSequence() {
    knee_FL.write(30); knee_FR.write(30);
    knee_BL.write(150); knee_BR.write(150);
}

void calibrateJoint(int id, int angle) {
    // توجيه زاوية ميكانيكية محددة للمفصل لضبط القراءة الصفرية
    if(id == 1) hip_FL.write(angle);
    // تضاف باقي المفاصل هنا أثناء الاختبار الميداني...
}