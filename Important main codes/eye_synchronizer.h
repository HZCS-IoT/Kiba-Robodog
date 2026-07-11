// ===================================================================
// VERTEX-X1 DUAL-OLED EYE SYNCHRONIZATION SYSTEM
// ===================================================================
#ifndef EYE_SYNCHRONIZER_H
#define EYE_SYNCHRONIZER_H

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// تهيئة الشاشتين بعناوين عتادية مختلفة على نفس خطوط الـ I2C
extern Adafruit_SSD1306 displayLeft;
extern Adafruit_SSD1306 displayRight;

// المصفوفة التعبيرية الموحدة للأعين
enum EyeExpression { NEUTRAL, BLINK, LOOK_LEFT, LOOK_RIGHT, ALERT };

// الدالة الذرية المزامنة: تحدد الإحداثيات وتدفع البيانات للشاشتين معاً
void updateSynchronizedEyes(EyeExpression expression, int lookOffset = 0) {
    displayLeft.clearDisplay();
    displayRight.clearDisplay();

    // الأبعاد الهندسية للعين المربعة الغاطسة لإخفاء البورد
    int eyeW = 98;
    int eyeH = 44;
    int topY = 10;
    int baseX = 15;

    switch (expression) {
        case NEUTRAL:
            displayLeft.fillRect(baseX + lookOffset, topY, eyeW, eyeH, SSD1306_WHITE);
            displayRight.fillRect(baseX + lookOffset, topY, eyeW, eyeH, SSD1306_WHITE);
            break;
            
        case BLINK:
            // وميض متزامن (خطوط أفقية تحاكي إغلاق الجفون)
            displayLeft.fillRect(baseX, 28, eyeW, 8, SSD1306_WHITE);
            displayRight.fillRect(baseX, 28, eyeW, 8, SSD1306_WHITE);
            break;
            
        case LOOK_LEFT:
            // إزاحة الإحداثيات لليسار في نفس اللحظة لشاشتين
            displayLeft.fillRect(baseX - 10, topY, eyeW, eyeH, SSD1306_WHITE);
            displayRight.fillRect(baseX - 10, topY, eyeW, eyeH, SSD1306_WHITE);
            break;

        case LOOK_RIGHT:
            // إزاحة الإحداثيات لليمين في نفس اللحظة لشاشتين
            displayLeft.fillRect(baseX + 10, topY, eyeW, eyeH, SSD1306_WHITE);
            displayRight.fillRect(baseX + 10, topY, eyeW, eyeH, SSD1306_WHITE);
            break;

        case ALERT:
            // إطار نيوني مزدوج للأعين يظهر الحماس أو الانتباه
            displayLeft.drawRect(baseX, topY, eyeW, eyeH, SSD1306_WHITE);
            displayLeft.drawRect(baseX + 4, topY + 4, eyeW - 8, eyeH - 8, SSD1306_WHITE);
            displayRight.drawRect(baseX, topY, eyeW, eyeH, SSD1306_WHITE);
            displayRight.drawRect(baseX + 4, topY + 4, eyeW - 8, eyeH - 8, SSD1306_WHITE);
            break;
    }

    // أمر الدفع المتتالي السريع جداً لجعل الحركة تظهر متزامنة تلقائياً
    displayLeft.display();
    displayRight.display();
}

#endif