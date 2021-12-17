#include <LiquidCrystal.h>
 
// LCD ←→ Arduinoのピンの割り当て
// rs      →   D7
// rw      →   GND
// enable  →   D8
// d4      →   D9
// d5      →   D10
// d6      →   D11
// d7      →   D12
 
// [構文]LiquidCrystal(rs, rw,  enable, d0, d1, d2, d3, d4, d5, d6, d7)
 
 LiquidCrystal lcd(7, 8, 10, 16, 14, 15);
 
 void setup() {
  
   lcd.begin(16, 2);          // LCDの桁数と行数を指定する(16桁2行)
   lcd.clear();               // LCD画面をクリア
   lcd.setCursor(0, 0);       // カーソルの位置を指定
   lcd.print("Hello!");       // 文字の表示
   lcd.setCursor(0, 1);       // カーソルの位置を指定
   lcd.print("Arduinopromicro");  // 文字の表示
 
}
 
 void loop() {
      
}