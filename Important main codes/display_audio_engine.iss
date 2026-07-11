// ===================================================================
// VERTEX-X1 DISPLAY & AUDIO ENGINE
// ===================================================================
#ifndef DISPLAY_AUDIO_ENGINE_H
#define DISPLAY_AUDIO_ENGINE_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define BUZZER_PIN 4 // دبوس مخرج الصوت الموجه للمضخم

Adafruit_SSD1306 displayLeft(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);
Adafruit_SSD1306 displayRight(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void initDisplayAndAudio() {
    // تهيئة الشاشات بالكامل وعنوان الـ I2C الافتراضي
    displayLeft.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    displayRight.begin(SSD1306_SWITCHCAPVCC, 0x3D);
    
    pinMode(BUZZER_PIN, OUTPUT);
}

// رسم أعين مربعة تملأ الشاشة النشطة وتخفي البورد
void drawSquareEyes(bool expressionState) {
    displayLeft.clearDisplay();
    displayRight.clearDisplay();
    
    if (expressionState) {
        // حالة الأعين العادية: رسم مربعات عريضة متناسقة هندسياً
        displayLeft.fillRect(15, 10, 98, 44, SSD1306_WHITE);
        displayRight.fillRect(15, 10, 98, 44, SSD1306_WHITE);
    } else {
        // حالة الوميض أو التنبيه (خطوط أفقية سايبربانك)
        displayLeft.fillRect(15, 28, 98, 8, SSD1306_WHITE);
        displayRight.fillRect(15, 28, 98, 8, SSD1306_WHITE);
    }
    
    displayLeft.display();
    displayRight.display();
}

// إطلاق نغمة تشغيل ميكانيكية عند بدء النظام
void playBootTone() {
    tone(BUZZER_PIN, 880, 150); // نغمة مرتفعة قصيرة
    delay(200);
    tone(BUZZER_PIN, 1200, 250);
}

#endif