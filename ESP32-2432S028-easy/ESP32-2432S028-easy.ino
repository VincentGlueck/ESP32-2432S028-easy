#include "lgfx_user.h"

#define ORIENTATION 1  // 1 = Widescreen, USB connector right

LGFX lcd;

void setup() {
  Serial.begin(115200);
  while (!Serial) yield();
  lcd.init();
  lcd.setRotation(ORIENTATION);
}

void loop() {
  lcd.setAddrWindow(0, 0, lcd.width(), lcd.height());
  lcd.startWrite();
  for (int y = 0; y < lcd.height(); ++y) {
    for (int x = 0; x < lcd.width(); ++x) {
      lcd.writeColor(lcd.color888(x >> 1, (x + y) >> 2, y >> 1), 1);
    }
  }
  lcd.endWrite();
  delay(2000);

  lcd.startWrite();
  lcd.fillScreen(TFT_BLACK);
  lcd.setFont(&fonts::DejaVu24);
  lcd.setTextDatum(textdatum_t::middle_center);
  lcd.drawString("Hello", lcd.width() >> 1, lcd.height() >> 1);
  delay(3000);
  lcd.endWrite();

  lcd.calibrate();
  
  lcd.fillScreen(TFT_BLACK);
  lcd.setFont(&fonts::DejaVu24);
  lcd.setTextDatum(textdatum_t::middle_center);
  lcd.drawString("Paint", lcd.width() >> 1, lcd.height() >> 1);
  lcd.setFont(&fonts::DejaVu12);
  lcd.drawString("(loop end, draw by touch)", lcd.width() >> 1, 32 + lcd.height() >> 1);
  uint32_t x, y;
  while(true) {
    if (lcd.getTouch(&x, &y)) {
      lcd.fillCircle(x, y, 5, TFT_WHITE);
    }
  }
}